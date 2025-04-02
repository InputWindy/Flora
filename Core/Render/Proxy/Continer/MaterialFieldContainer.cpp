#include <Render/Proxy/Continer/MaterialFieldContainer.h>
#include <Resource/Struct/Material.h>
#include <Render/RHI/RHI.h>
#include <Render/Proxy/ProxyBase.h>

void XVerse::XMaterialFieldContainer::SetupMaterialState()
{
	XRHI* Rhi = GetRHI();
	Rhi->SetupMaterialState(MaterialState);
}

//XVerse::XMaterialFieldContainer* XVerse::XMaterialProxy::GetContainer()
//{
//	return Container.get();
//}