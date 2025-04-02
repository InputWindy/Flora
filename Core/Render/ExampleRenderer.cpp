#include <Render/ExampleRenderer.h>

template<>
void XVerse::XSceneRenderProxy<XVerse::XExampleRenderScene>::CreateContainer()
{
	if (Container == nullptr || Object == nullptr)return;


}
template<>
void XVerse::XSceneRenderProxy<XVerse::XExampleRenderScene>::UpdateObject()
{
	if (Object == nullptr || Container == nullptr)return;


}
void XVerse::XExampleRenderer::CreateContainer(XScene* InScene)
{
	XSceneRenderProxy<XExampleRenderScene> Proxy(InScene);
	SceneRenderProxy = std::move(Proxy);
}
void XVerse::XExampleRenderer::UpdateScene()
{
	SceneRenderProxy.UpdateObject();
}
;
