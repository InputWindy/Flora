#pragma once 
#include <Render/Renderer.h>

namespace XVerse
{
	struct XExampleRendererSettings:public IRendererSettings
	{

	};

	struct XExampleRenderScene
	{

	};

	class XExampleRenderer :public IRenderer<>
	{
	public:
		XExampleRenderer() = default;
		XExampleRenderer(IRendererSettings* InSettings) :IRenderer<>(InSettings) {}
		virtual ~XExampleRenderer() = default;

	public:
		virtual void Init() {}
		virtual void BeginRender() {}
		virtual void EndRender() {}

		virtual void CreateContainer(XScene*);
		virtual void UpdateScene();

	protected:
		XSceneRenderProxy<XExampleRenderScene> SceneRenderProxy;
	};
}