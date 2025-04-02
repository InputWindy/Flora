#include <Render/Proxy/ProxyBase.h>
#include <Geometry/StaticMesh.h>

#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/clean.h>
#include <Render/RHI/RHI.h>

#include <Resource/Struct/Material.h>
#include <Resource/Struct/Texture.h>

#include <glad/glad.h>

#include <iostream>

template<>
void XVerse::XStaticMeshProxy::CreateContainer()
{
	if (Container == nullptr || Object == nullptr)return;

	Container->VertexBulkData.clear();
	Container->IndexBulkData.clear();
	Container->VertexBufferLayout = {};

	XStaticMesh& InoutStaticMesh = *Object;

	bool bHasPosition = InoutStaticMesh.HasDataMask(XVerse::XStaticMesh::MM_VERTCOORD);
	bool bHasVertexNormal = InoutStaticMesh.HasDataMask(XVerse::XStaticMesh::MM_VERTNORMAL);
	bool bHasVertexCoord = InoutStaticMesh.HasDataMask(XVerse::XStaticMesh::MM_VERTTEXCOORD);
	bool bHasVertexColor = InoutStaticMesh.HasDataMask(XVerse::XStaticMesh::MM_VERTCOLOR);

	if (bHasPosition)
	{
		Container->VertexBufferLayout.AddElement({ XVerse::BufferElementDataType::Float3,"aPosition" });
	}

	if (bHasVertexNormal)
	{
		Container->VertexBufferLayout.AddElement({ XVerse::BufferElementDataType::Float3,"aNormal",true });
	}

	if (bHasVertexCoord)
	{
		Container->VertexBufferLayout.AddElement({ XVerse::BufferElementDataType::Float2,"aUV",true });
	}

	if (bHasVertexColor)
	{
		Container->VertexBufferLayout.AddElement({ XVerse::BufferElementDataType::Float4,"aColor" });
	}

	Container->VertexBufferLayout.CalculateOffsetAndStride();

	Container->VertexBulkData.resize(Container->VertexBufferLayout.GetStride() * InoutStaticMesh.VN());

	uint32_t Stride = Container->VertexBufferLayout.GetStride();

	for (size_t Idx = 0; Idx < InoutStaticMesh.VN(); Idx++)
	{
		const XVerse::XVertex& Vertex = InoutStaticMesh.vert[Idx];
		std::byte* pBuffer = Container->VertexBulkData.data();

		XVerse::BufferElement Element;
		if (bHasPosition && Container->VertexBufferLayout.GetElement("aPosition", Element))
		{
			memcpy(pBuffer + Idx * Stride + Element.Offset + 0 * sizeof(Vertex.cP().X()), &(Vertex.cP().X()), sizeof(Vertex.cP().X()));
			memcpy(pBuffer + Idx * Stride + Element.Offset + 1 * sizeof(Vertex.cP().Y()), &(Vertex.cP().Y()), sizeof(Vertex.cP().Y()));
			memcpy(pBuffer + Idx * Stride + Element.Offset + 2 * sizeof(Vertex.cP().Z()), &(Vertex.cP().Z()), sizeof(Vertex.cP().Z()));
		}
		//normal
		if (bHasVertexNormal && Container->VertexBufferLayout.GetElement("aNormal", Element))
		{
			memcpy(pBuffer + Idx * Stride + Element.Offset + 0 * sizeof(Vertex.cN().X()), &(Vertex.cN().X()), sizeof(Vertex.cN().X()));
			memcpy(pBuffer + Idx * Stride + Element.Offset + 1 * sizeof(Vertex.cN().Y()), &(Vertex.cN().Y()), sizeof(Vertex.cN().Y()));
			memcpy(pBuffer + Idx * Stride + Element.Offset + 2 * sizeof(Vertex.cN().Z()), &(Vertex.cN().Z()), sizeof(Vertex.cN().Z()));
		}

		//uv0
		if (bHasVertexCoord && Container->VertexBufferLayout.GetElement("aUV0", Element))
		{
			memcpy(pBuffer + Idx * Stride + Element.Offset + 0 * sizeof(Vertex.cT().U()), &(Vertex.cT().U()), sizeof(Vertex.cT().U()));
			memcpy(pBuffer + Idx * Stride + Element.Offset + 1 * sizeof(Vertex.cT().V()), &(Vertex.cT().V()), sizeof(Vertex.cT().V()));
		}

		//color0
		if (bHasVertexColor && Container->VertexBufferLayout.GetElement("aColor0", Element))
		{
			memcpy(pBuffer + Idx * Stride + Element.Offset + 0 * sizeof(Vertex.cC()[0]), &(Vertex.cC()[0]), sizeof(Vertex.cC()[0]));
			memcpy(pBuffer + Idx * Stride + Element.Offset + 1 * sizeof(Vertex.cC()[1]), &(Vertex.cC()[1]), sizeof(Vertex.cC()[1]));
			memcpy(pBuffer + Idx * Stride + Element.Offset + 2 * sizeof(Vertex.cC()[2]), &(Vertex.cC()[2]), sizeof(Vertex.cC()[2]));
			memcpy(pBuffer + Idx * Stride + Element.Offset + 3 * sizeof(Vertex.cC()[3]), &(Vertex.cC()[3]), sizeof(Vertex.cC()[3]));
		}
	}

	for (size_t Idx = 0; Idx < InoutStaticMesh.FN(); Idx++)
	{
		const XVerse::XFace& Face = InoutStaticMesh.face[Idx];
		Container->IndexBulkData.push_back(Face.cV(0)->Index());
		Container->IndexBulkData.push_back(Face.cV(1)->Index());
		Container->IndexBulkData.push_back(Face.cV(2)->Index());
	}
	
	/*
		Create rhi resource
	*/
	Container->MeshRhiResource = nullptr;
	Container->VertexRhiResource = nullptr;
	Container->IndexRhiResource = nullptr;
	
	XRHIBufferCreateInfo VertexBufferInfo;
	VertexBufferInfo.BufferTarget = EBufferTarget::BT_ARRAY_BUFFER;
	Container->VertexRhiResource = GetRHI()->CreateBuffer(VertexBufferInfo)->DynamicPointerCast<XRHIVertexBuffer>();
	Container->VertexRhiResource->SetData(Container->VertexBulkData.size(), Container->VertexBulkData.data(),EBufferUsage::BU_DYNAMIC_DRAW);
	
	XRHIBufferCreateInfo IndexBufferInfo;
	IndexBufferInfo.BufferTarget = EBufferTarget::BT_ELEMENT_ARRAY_BUFFER;
	Container->IndexRhiResource = GetRHI()->CreateBuffer(IndexBufferInfo)->DynamicPointerCast<XRHIIndexBuffer>();
	Container->IndexRhiResource->SetData(Container->IndexBulkData.size() * sizeof(unsigned int), Container->IndexBulkData.data(), EBufferUsage::BU_DYNAMIC_DRAW);
	
	XRHIVertexArrayCreateInfo VertexArrayInfo;
	VertexArrayInfo.VertexBufferLayout = Container->VertexBufferLayout;
	VertexArrayInfo.VertexBuffer = Container->VertexRhiResource;
	VertexArrayInfo.IndexBuffer = Container->IndexRhiResource;
	Container->MeshRhiResource = GetRHI()->CreateVertexArray(VertexArrayInfo);
}
template<>
void XVerse::XStaticMeshProxy::UpdateObject()
{
	if (Object == nullptr || Container == nullptr)return;

	Object->Clear();
	Object->UpdateDataMask();

	const auto& VertexBulkData = Container->VertexBulkData;
	const auto& IndexBulkData = Container->IndexBulkData;
	const auto& VertexBufferLayout = Container->VertexBufferLayout;

	size_t FaceNum = Container->GetTriangleNum();
	size_t VertexNum = Container->GetVertexNum();
	unsigned int Stride = VertexBufferLayout.GetStride();

	auto VertexIter = vcg::tri::Allocator<XVerse::XStaticMesh>::AddVertices(*Object, VertexNum);

	XVerse::BufferElement Element;
	if (VertexBufferLayout.GetElement("aPosition",Element) && VertexIter->IsCoordEnabled())
	{
		auto TmpIter = VertexIter;
		const std::byte* pBuffer = VertexBulkData.data();
		for (size_t VertexIdx = 0; VertexIdx < VertexNum; VertexIdx++)
		{
			XVerse::XPoint3 P;
			memcpy(&P, pBuffer + Stride * VertexIdx + Element.Offset, Element.Size);
			TmpIter->P() = P;

			++TmpIter;
		}
	}

	if (VertexBufferLayout.GetElement("aNormal", Element))
	{
		auto TmpIter = VertexIter;
		const std::byte* pBuffer = VertexBulkData.data();
		for (size_t VertexIdx = 0; VertexIdx < VertexNum; VertexIdx++)
		{
			XVerse::XPoint3 N;
			memcpy(&N, pBuffer + Stride * VertexIdx + Element.Offset, Element.Size);
			TmpIter->N() = N;

			++TmpIter;
		}
	}

	for (size_t Idx = 0; Idx < FaceNum; Idx++)
	{
		vcg::tri::Allocator<XVerse::XStaticMesh>::AddFace(*Object,
			IndexBulkData[Idx * 3 + 0], 
			IndexBulkData[Idx * 3 + 1],
			IndexBulkData[Idx * 3 + 2]
		);
	};

	vcg::tri::Clean<XVerse::XStaticMesh>::RemoveDuplicateVertex(*Object);
	vcg::tri::Allocator<XVerse::XStaticMesh>::CompactEveryVector(*Object);

}
template<>
void XVerse::XMaterialProxy::CreateContainer()
{
	if (Container == nullptr || Object == nullptr)return;

	Container->ShaderProgramResource = nullptr;

	XVerse::XRHIShaderProgramCreateInfo ShaderProgramInfo;
	for (auto Iter : Object->GetShaderMap())
	{
		XVerse::XRHIShaderCreateInfo ShaderInfo;
		ShaderInfo.ShaderType = Iter.first;
		ShaderInfo.ShaderCode = Iter.second;
		std::shared_ptr<XVerse::XRHIShader> Shader = GetRHI()->CreateShader(ShaderInfo);
		if (Shader->IsCompiled() == false)
		{
			std::string log;
			Shader->GetShaderInfoLog(log);
			std::cout << log << std::endl;
		}
		
		switch (Iter.first)
		{
		case XVerse::EShaderType::ST_VERTEX_SHADER:ShaderProgramInfo.VertexShader = Shader; break;
		case XVerse::EShaderType::ST_TESS_CONTROL_SHADER:ShaderProgramInfo.TessControlShader = Shader; break;
		case XVerse::EShaderType::ST_TESS_EVALUATION_SHADER:ShaderProgramInfo.TessEvaluationShader = Shader; break;
		case XVerse::EShaderType::ST_GEOMETRY_SHADER:ShaderProgramInfo.GeometryShader = Shader; break;
		case XVerse::EShaderType::ST_FRAGMENT_SHADER:ShaderProgramInfo.FragmentShader = Shader; break;
		case XVerse::EShaderType::ST_COMPUTE_SHADER:ShaderProgramInfo.ComputeShader = Shader; break;
		case XVerse::EShaderType::ST_MAX_COUNT:break;
		default:
			break;
		}
	}

	Container->ShaderProgramResource = GetRHI()->CreateShaderProgram(ShaderProgramInfo);

	if (Container->ShaderProgramResource->IsLinked() == false)
	{
		std::string log;
		Container->ShaderProgramResource->GetShaderProgramInfoLog(log);

		XVERSE_CORE_ERROR(log);
	}

	Container->MaterialState = Object->GetMaterialState();
}
template<>
void XVerse::XMaterialProxy::UpdateObject()
{
	if (Object == nullptr || Container == nullptr)return;
}

template<>
void XVerse::XTextureProxy::CreateContainer()
{
	if (Container == nullptr || Object == nullptr)return;

	Container->TextureResource = nullptr;

	if (Object->GetTypeString() == "Texture2D")
	{
		XTexture2D* Texture2D = reinterpret_cast<XTexture2D*>(Object);

		XRHITextureCreateInfo Info;
		Info.SizeX = Texture2D->GetSizeX();
		Info.SizeY = Texture2D->GetSizeY();

		Info.InternalFormat = MatchFormat(Texture2D->GetFormat(), Texture2D->GetDataType());
		Info.MipLevels = 1;
		Info.TextureType = ETextureTarget::TT_TEXTURE_2D;
		Info.DefaultSamplerInfo = {};

		auto Texture2DRhi = GetRHI()->CreateTexture(Info)->DynamicPointerCast<XRHITexture2D>();
		Texture2DRhi->SetPixels(0, Texture2D->GetFormat(), Texture2D->GetDataType(), Texture2D->GetImageData());
		Container->TextureResource = Texture2DRhi;
	}
	else if (Object->GetTypeString() == "VolumeTexture")
	{

	}
	else if (Object->GetTypeString() == "TextureCube")
	{

	}
	else assert(0);
}

template<>
void XVerse::XTextureProxy::UpdateObject()
{
	
}