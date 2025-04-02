#pragma once
#include <Render/Proxy/ProxyBase.h>
#include <Scene/Scene.h>
#include <Common/Log.h>
#include <Common/Common.h>
#include <memory>

namespace flora
{
	class IPass;

	struct IRenderBase :public INoneCopyable 
	{
		void SetOwner(IPass* InPass);
	};

	//ProjectSettings/Renderer
	struct IRendererSettings {};
	
	template<typename TCustomRenderScene>
	using XSceneRenderProxy = flora::XProxy<flora::XScene, TCustomRenderScene>;

	class IPass :public INoneCopyable, public std::enable_shared_from_this<IPass>
	{
		friend class IRenderBase;
	protected:
		IPass() = default;
	public:
		virtual ~IPass() = default;

		/// <summary>
		/// init rendering resources
		/// </summary>
		virtual bool Init() = 0;

		/// <summary>
		/// check if rendering resources are ready.
		/// </summary>
		/// <returns></returns>
		virtual bool IsValid()const = 0;

		/// <summary>
		/// read data from renderer(render scene & renderer settings)
		/// </summary>
		/// <param name="renderer"></param>
		virtual void Draw(IRenderBase* = nullptr) = 0;
	private:
		void SetOwner(IRenderBase* InOwner) { Owner = InOwner; }
	protected:
		IRenderBase* Owner = nullptr;
	};

	struct IRenderGraph
	{
	protected:
		/// <summary>
		/// is valid render graph ?
		/// </summary>
		/// <returns></returns>
		virtual bool IsValid()const = 0;

		/// <summary>
		/// get linear render pipeline
		/// </summary>
		/// <returns></returns>
		virtual std::vector<std::shared_ptr<IPass>> GetSortedRenderPipeline() = 0;

		/// <summary>
		/// add a pass to the graph
		/// </summary>
		/// <param name="dependencies"></param>
		/// <param name="pass instance"></param>
		/// <param name="info log"></param>
		/// <returns></returns>
		virtual bool AddPass(const std::vector<std::string>&, const std::string&, std::shared_ptr<IPass>, std::string&) = 0;
	};

	struct LinearRenderGraph :public IRenderGraph
	{
	protected:
		virtual bool IsValid()const { return true; }
		virtual std::vector<std::shared_ptr<IPass>> GetSortedRenderPipeline() { return Passes; }
		virtual bool AddPass(const std::vector<std::string>&, const std::string&, std::shared_ptr<IPass> InPass, std::string& InfoLog)
		{
			if (InPass == nullptr)
			{
				InfoLog = "Empty Pass!";
				return false;
			}
			Passes.push_back(InPass); 
			return true;
		}
	protected:
		std::vector<std::shared_ptr<IPass>> Passes = {};
	};

	struct DAGRenderGraph : public IRenderGraph
	{
	protected:
		struct GraphNode
		{
			std::shared_ptr<IPass> Pass = nullptr;

			std::string PassName;

			std::vector<std::weak_ptr<GraphNode>> Dependencies;
			std::vector<std::shared_ptr<GraphNode>> Nexts = {};

		}RenderGraph;

		virtual bool IsValid()const { return true; }
		virtual std::vector<std::shared_ptr<IPass>> GetSortedRenderPipeline() { return {}; }
		virtual bool AddPass(const std::vector<std::string>&, const std::string&, std::shared_ptr<IPass> InPass, std::string& InfoLog) { return true; }
	};

	/// <summary>
	/// application render pipeline
	/// </summary>
	template<typename TRenderGraph = LinearRenderGraph>
	class IRenderer :public IRenderBase,public TRenderGraph, public std::enable_shared_from_this<IRenderer<TRenderGraph>>
	{
		friend class IPass;

		using RenderGraphType = TRenderGraph;
	protected:
		IRenderer() = default;
		IRenderer(IRendererSettings* InSettings) :Settings(InSettings) {}
	public:
		virtual ~IRenderer() = default;
	public:
		/// <summary>
		/// init render pipeline
		/// </summary>
		virtual void Init() = 0;

		/// <summary>
		/// pre render operations(check resources and validate this renderer)
		/// </summary>
		virtual void BeginRender() = 0;

		/// <summary>
		/// post render operation 
		/// </summary>
		virtual void EndRender() = 0;

		/// <summary>
		/// check if renderer resources are ready (ensure that backbuffer is ready)
		/// </summary>
		/// <returns></returns>
		virtual bool IsValid()const { return RenderGraphType::IsValid(); }
	public:
		/// <summary>
		/// execute render pipeline
		/// </summary>
		void Render()
		{
			if (!IsValid())return;

			std::vector<std::shared_ptr<IPass>> Pipeline = RenderGraphType::GetSortedRenderPipeline();

			bool bValidPipeline = true;

			for (std::shared_ptr<IPass> Pass : Pipeline)
			{
				bValidPipeline &= Pass->IsValid();
			}

			if (!bValidPipeline)return;

			for (std::shared_ptr<IPass> Pass : Pipeline)
			{
				Pass->Draw(this);
			}
		};

		/// <summary>
		/// add a pass to render graph
		/// </summary>
		/// <param name="InDependencies"></param>
		/// <param name="InPassName"></param>
		/// <param name="InPass"></param>
		/// <returns></returns>
		bool AddPass(const std::vector<std::string>& InDependencies, const std::string& InPassName, std::shared_ptr<IPass>InPass)
		{
			std::string InfoLog;
			if (!RenderGraphType::AddPass(InDependencies, InPassName, InPass, InfoLog))
			{
				//FLORA_CORE_ERROR("Fail to add pass : {0},{1}", InPassName, InfoLog);
				return false;
			}
			SetOwner(InPass.get());
			InPass->Init();
			return true;
		}
	public:
		template<typename TSettings>
		inline TSettings* GetSettings() { return reinterpret_cast<TSettings*>(Settings.get()); }
	protected:
		std::unique_ptr<IRendererSettings> Settings;
	};
}