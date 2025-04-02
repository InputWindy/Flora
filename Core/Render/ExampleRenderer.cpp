#include <Render/ExampleRenderer.h>

template<>
void flora::XSceneRenderProxy<flora::XExampleRenderScene>::CreateContainer()
{
	if (Container == nullptr || Object == nullptr)return;


}
template<>
void flora::XSceneRenderProxy<flora::XExampleRenderScene>::UpdateObject()
{
	if (Object == nullptr || Container == nullptr)return;


}
void flora::XExampleRenderer::CreateContainer(XScene* InScene)
{
	XSceneRenderProxy<XExampleRenderScene> Proxy(InScene);
	SceneRenderProxy = std::move(Proxy);
}
void flora::XExampleRenderer::UpdateScene()
{
	SceneRenderProxy.UpdateObject();
}
;
