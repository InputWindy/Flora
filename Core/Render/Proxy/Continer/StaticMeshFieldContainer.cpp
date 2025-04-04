#include <Geometry/StaticMesh.h>
#include <Render/Proxy/Continer/StaticMeshFieldContainer.h>
#include <Render/Proxy/ProxyBase.h>

#include <Render/RHI/RHI.h>

//flora::XStaticMeshFieldContainer* flora::XStaticMeshProxy::GetContainer()
//{
//	return Container.get();
//}

void flora::XStaticMeshFieldContainer::DrawElement(EDrawMode InDrawMode)
{
	XRHI* Rhi = GetRHI();
	Rhi->DrawElement(InDrawMode, MeshRhiResource, IndexBulkData.size());
}

void flora::XStaticMeshFieldContainer::DrawElementInstanced(EDrawMode InDrawMode,uint32_t InstanceNum)
{
	XRHI* Rhi = GetRHI();
	Rhi->DrawElementInstanced(InDrawMode, MeshRhiResource, IndexBulkData.size(), InstanceNum);
}

void flora::XStaticMeshFieldContainer::RegenerateMesh(const std::set<unsigned int>& InPrimitiveArray)
{
	std::vector<uint32_t> NewIndices;
	for (uint32_t PrimitiveId : InPrimitiveArray)
	{
		NewIndices.push_back(IndexBulkData[PrimitiveId * 3 + 0]);
		NewIndices.push_back(IndexBulkData[PrimitiveId * 3 + 1]);
		NewIndices.push_back(IndexBulkData[PrimitiveId * 3 + 2]);
	};
	IndexBulkData = NewIndices;
}
