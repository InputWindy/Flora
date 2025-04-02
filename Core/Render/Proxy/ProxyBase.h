#pragma once
#include <memory>
#include <Common/ForwardDeclare.h>
#include <Render/Proxy/Continer/StaticMeshFieldContainer.h>
#include <Render/Proxy/Continer/MaterialFieldContainer.h>
#include <Render/Proxy/Continer/TextureFieldContainer.h>

namespace XVerse
{
	class XStaticMesh;
	class XMaterial;
	class XTexture;

    template<typename TBase,typename TFieldContainer>
	class XProxy
	{
		using BaseType = TBase;
		using ContainerType = TFieldContainer;
	public:
		virtual ~XProxy() { if (bAutoUpdate) { UpdateObject(); } Container.reset(); }

		XProxy() :Object(nullptr), Container(nullptr) {}
		XProxy(TBase* BaseObject) :Object(BaseObject), Container(new TFieldContainer()) { CreateContainer(); }
		XProxy(std::shared_ptr<TBase> BaseObject) :Object(BaseObject.get()), Container(new TFieldContainer()) { CreateContainer(); }

		//Please use std::move()
		XProxy(const XProxy&) = delete;
		XProxy(XProxy&& Other)
		{
			if (bAutoUpdate)UpdateObject();

			Object = Other.Object;
			Container.swap(Other.Container);
			bAutoUpdate = Other.bAutoUpdate;

			Other.Object = nullptr;
			Other.Container.reset();
		}

		//Please use std::move()
		void operator=(const XProxy&) = delete;
		void operator=(XProxy&& Other)
		{
			if (bAutoUpdate)UpdateObject();

			Object = Other.Object;
			Container.swap(Other.Container);
			bAutoUpdate = Other.bAutoUpdate;

			Other.Object = nullptr;
			Other.Container.reset();
		}
	public:
		void CreateContainer();
		void UpdateContainer();
		void UpdateObject();

		inline TBase* GetObject() { return Object; }
		TFieldContainer* GetContainer() { return Container.get(); }
	private:
		mutable TBase* Object;
		std::unique_ptr<TFieldContainer> Container;
	public:
		bool bAutoUpdate = true;
	};

	using XStaticMeshProxy = XProxy<XStaticMesh, XStaticMeshFieldContainer>;
	using XMaterialProxy = XProxy<XMaterial, XMaterialFieldContainer>;
	using XTextureProxy = XProxy<XTexture, XTextureFieldContainer>;
}

#include <Resource/ResourceManager.h>

// shader utils
namespace XVerse
{
#define DECLARE_SURFACE_SHADER(ShaderName,VertexShaderName,FragmentShaderName) \
struct ShaderName\
{\
	static Guid GetOrCreateShader()\
	{\
		static Guid sShader = 0;\
		if (sShader == 0)\
		{\
			MaterialImportSettings Settings;\
			Settings.Files[EShaderType::ST_VERTEX_SHADER] = VertexShaderName;\
			Settings.Files[EShaderType::ST_FRAGMENT_SHADER] = FragmentShaderName;\
			auto Resource = IResource<>::Open<XImporter>(SHADER_ROOT_DIR, Settings);\
			if (!Resource)\
			{\
				Resource = IResource<>::Open<XImporter>(ExcutePath + "/Shaders/", Settings);\
			}\
			if (Resource)\
			{\
				std::shared_ptr<XMaterial> Material = Resource->DynamicPointerCast<XMaterial>(); \
				Material->Register(); \
				sShader = Material->GetID(); \
			}\
		}\
		return sShader;\
	}\
};

#define DECLARE_COMPUTE_SHADER(ShaderName,ComputeShaderName) \
struct ShaderName\
{\
	static Guid GetOrCreateShader()\
	{\
		static Guid sShader = 0;\
		if (sShader == 0)\
		{\
			MaterialImportSettings Settings;\
			Settings.Files[EShaderType::ST_COMPUTE_SHADER] = ComputeShaderName;\
			auto Resource = IResource<>::Open<XImporter>(SHADER_ROOT_DIR, Settings);\
			if (!Resource)\
			{\
				Resource = IResource<>::Open<XImporter>(ExcutePath + "/Shaders/", Settings);\
			}\
			if (Resource)\
			{\
				std::shared_ptr<XMaterial> Material = Resource->DynamicPointerCast<XMaterial>(); \
				Material->Register(); \
				sShader = Material->GetID(); \
			}\
		}\
		return sShader;\
	}\
};

	template<typename ShaderType>
	struct FScopeShader
	{
	public:
		FScopeShader()
		{
			Guid UID = ShaderType::GetOrCreateShader();

			auto& ResourceManager = IResource<>::ResourceManagerType::Get();

			if (auto Resource = ResourceManager.Find(UID))
			{
				if (Resource)
				{
					MaterialProxy = Resource->DynamicPointerCast<XMaterial>();
				}
			};
		}

		bool IsValid()
		{
			return MaterialProxy.GetContainer() && MaterialProxy.GetObject();
		}

		std::shared_ptr<XRHIShaderProgram> GetProgram()
		{
			if (IsValid())
			{
				return MaterialProxy.GetContainer()->ShaderProgramResource;
			}

			return nullptr;
		}

		XMaterialState& GetRenderState()
		{
			if (IsValid())
			{
				return 	MaterialProxy.GetContainer()->MaterialState;
			}
		}

		void SetupRenderState()
		{
			if (IsValid())
			{
				MaterialProxy.GetContainer()->SetupMaterialState();
			}
		}


	private:
		XMaterialProxy MaterialProxy;
	};
}
