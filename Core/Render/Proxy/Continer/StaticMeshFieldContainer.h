#pragma once 
#include <Common/Layout.h>
#include <vector>
#include <memory>
#include <set>

#include <Render/RHI/RHI.h>

namespace XVerse
{
	class XRHIVertexArray;
	class XRHIVertexBuffer;
	class XRHIIndexBuffer;

	class XRHIShaderStorageBuffer;

	struct XStaticMeshFieldContainer
	{
		XStaticMeshFieldContainer() = default;

		std::vector<std::byte> VertexBulkData = {};
		std::vector<uint32_t> IndexBulkData = {};
		XVerse::BufferLayout VertexBufferLayout = {};

		std::shared_ptr<XRHIVertexArray> MeshRhiResource;
		std::shared_ptr<XRHIVertexBuffer> VertexRhiResource;
		std::shared_ptr<XRHIIndexBuffer> IndexRhiResource;
		std::shared_ptr<XRHIShaderStorageBuffer> MeshBufferRhiResource;

		void DrawElement(EDrawMode);

		void DrawElementInstanced(EDrawMode, uint32_t = 1);

		void RegenerateMesh(const std::set<unsigned int>& InFaces);
	public:
		size_t GetVertexNum()const { return VertexBulkData.size() / VertexBufferLayout.GetStride(); }
		size_t GetTriangleNum()const { return IndexBulkData.size() / 3; }
	};
};