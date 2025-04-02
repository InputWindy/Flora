#include <Render/Proxy/Continer/MaterialFieldContainer.h>
#include <Resource/Struct/Material.h>
#include <Render/RHI/RHI.h>
#include <Render/Proxy/ProxyBase.h>

void flora::XMaterialFieldContainer::SetupMaterialState()
{
	XRHI* Rhi = GetRHI();
	Rhi->SetupMaterialState(MaterialState);
}

//flora::XMaterialFieldContainer* flora::XMaterialProxy::GetContainer()
//{
//	return Container.get();
//}