#include "StaticMesh.h"
#include <Common/Layout.h>
#include <vcglib/wrap/io_trimesh/io_mask.h>
#include <Resource/Struct/ModelMesh.h>

const flora::Wrap::GlobalSettings& flora::XStaticMesh::GetMetaData() const
{
	return Owner.lock()->GetMetaData();
}

void flora::XStaticMesh::UpdateDataMask()
{
	CurrentDataMask = MM_NONE;
	CurrentDataMask |=
		MM_VERTCOORD | MM_VERTNORMAL | MM_VERTFLAG |
		MM_VERTQUALITY | MM_VERTCOLOR;
	CurrentDataMask |=
		MM_FACEVERT | MM_FACENORMAL | MM_FACEFLAG;
	if (this->vert.IsVFAdjacencyEnabled())
		CurrentDataMask |= MM_VERTFACETOPO;
	if (this->vert.IsMarkEnabled())
		CurrentDataMask |= MM_VERTMARK;
	if (this->vert.IsTexCoordEnabled())
		CurrentDataMask |= MM_VERTTEXCOORD;
	if (this->vert.IsCurvatureDirEnabled())
		CurrentDataMask |= MM_VERTCURVDIR;
	if (this->vert.IsRadiusEnabled())
		CurrentDataMask |= MM_VERTRADIUS;
	if (this->face.IsQualityEnabled())
		CurrentDataMask |= MM_FACEQUALITY;
	if (this->face.IsMarkEnabled())
		CurrentDataMask |= MM_FACEMARK;
	if (this->face.IsColorEnabled())
		CurrentDataMask |= MM_FACECOLOR;
	if (this->face.IsFFAdjacencyEnabled())
		CurrentDataMask |= MM_FACEFACETOPO;
	if (this->face.IsVFAdjacencyEnabled())
		CurrentDataMask |= MM_VERTFACETOPO;
	if (this->face.IsCurvatureDirEnabled())
		CurrentDataMask |= MM_FACECURVDIR;
	if (this->face.IsWedgeTexCoordEnabled())
		CurrentDataMask |= MM_WEDGTEXCOORD;
}

void flora::XStaticMesh::UpdateDataMask(int InNeededDataMask)
{
	if ((InNeededDataMask & MM_FACEFACETOPO) != 0)
	{
		this->face.EnableFFAdjacency();
		vcg::tri::UpdateTopology<XStaticMesh>::FaceFace(*this);
	}
	if ((InNeededDataMask & MM_VERTFACETOPO) != 0)
	{
		this->vert.EnableVFAdjacency();
		this->face.EnableVFAdjacency();
		vcg::tri::UpdateTopology<XStaticMesh>::VertexFace(*this);
	}

	if ((InNeededDataMask & MM_WEDGTEXCOORD) != 0)
		this->face.EnableWedgeTexCoord();
	if ((InNeededDataMask & MM_FACECOLOR) != 0)
		this->face.EnableColor();
	if ((InNeededDataMask & MM_FACEQUALITY) != 0)
		this->face.EnableQuality();
	if ((InNeededDataMask & MM_FACECURVDIR) != 0)
		this->face.EnableCurvatureDir();
	if ((InNeededDataMask & MM_FACEMARK) != 0)
		this->face.EnableMark();
	if ((InNeededDataMask & MM_VERTMARK) != 0)
		this->vert.EnableMark();
	if ((InNeededDataMask & MM_VERTCURVDIR) != 0)
		this->vert.EnableCurvatureDir();
	if ((InNeededDataMask & MM_VERTRADIUS) != 0)
		this->vert.EnableRadius();
	if ((InNeededDataMask & MM_VERTTEXCOORD) != 0)
		this->vert.EnableTexCoord();

	CurrentDataMask |= InNeededDataMask;
}

void flora::XStaticMesh::ClearDataMask(int InUnneededDataMask)
{
	if (((InUnneededDataMask & MM_VERTFACETOPO) != 0) && HasDataMask(MM_VERTFACETOPO)) {
		this->face.DisableVFAdjacency();
		this->vert.DisableVFAdjacency();
	}
	if (((InUnneededDataMask & MM_FACEFACETOPO) != 0) && HasDataMask(MM_FACEFACETOPO))	this->face.DisableFFAdjacency();

	if (((InUnneededDataMask & MM_WEDGTEXCOORD) != 0) && HasDataMask(MM_WEDGTEXCOORD)) 	this->face.DisableWedgeTexCoord();
	if (((InUnneededDataMask & MM_FACECOLOR) != 0) && HasDataMask(MM_FACECOLOR))			this->face.DisableColor();
	if (((InUnneededDataMask & MM_FACEQUALITY) != 0) && HasDataMask(MM_FACEQUALITY))		this->face.DisableQuality();
	if (((InUnneededDataMask & MM_FACEMARK) != 0) && HasDataMask(MM_FACEMARK))			this->face.DisableMark();
	if (((InUnneededDataMask & MM_VERTMARK) != 0) && HasDataMask(MM_VERTMARK))			this->vert.DisableMark();
	if (((InUnneededDataMask & MM_VERTCURV) != 0) && HasDataMask(MM_VERTCURV))			this->vert.DisableCurvature();
	if (((InUnneededDataMask & MM_VERTCURVDIR) != 0) && HasDataMask(MM_VERTCURVDIR))		this->vert.DisableCurvatureDir();
	if (((InUnneededDataMask & MM_VERTRADIUS) != 0) && HasDataMask(MM_VERTRADIUS))		this->vert.DisableRadius();
	if (((InUnneededDataMask & MM_VERTTEXCOORD) != 0) && HasDataMask(MM_VERTTEXCOORD))	this->vert.DisableTexCoord();

	CurrentDataMask &= (~InUnneededDataMask);
}

int flora::XStaticMesh::DataMask() const
{
	return CurrentDataMask;
}

//void flora::XStaticMesh::SetData(float* aPos, float* aNor, float* aUV, int* Index, int numV, int numI)
//{
//	if (!Index || numV <= 0 || numI <= 0)return;
//
//	Enable(vcg::tri::io::Mask::IOM_VERTCOORD | vcg::tri::io::Mask::IOM_VERTNORMAL | vcg::tri::io::Mask::IOM_VERTTEXCOORD);
//
//	auto VertexIter = vcg::tri::Allocator<flora::XStaticMesh>::AddVertices(*this, numV);
//	for (size_t Idx = 0; Idx < numV; Idx++)
//	{
//		if (aPos && VertexIter->IsCoordEnabled())
//		{
//			VertexIter->P() = { aPos[Idx + 0],aPos[Idx + 1] ,aPos[Idx + 2] };
//		}
//
//		if (aNor && VertexIter->IsNormalEnabled())
//		{
//			VertexIter->N() = { aNor[Idx + 0],aNor[Idx + 1] ,aNor[Idx + 2] };
//		}
//
//		if (aUV && VertexIter->IsTexCoordEnabled())
//		{
//			VertexIter->T() = { aUV[Idx + 0],aUV[Idx + 1]};
//		}
//
//		++VertexIter;
//	}
//
//	for (size_t Idx = 0; Idx < numI / 3; Idx++)
//	{
//		vcg::tri::Allocator<flora::XStaticMesh>::AddFace(*this, Index[Idx + 0], Index[Idx + 1], Index[Idx + 2]);
//	};
//
//	vcg::tri::Clean<flora::XStaticMesh>::RemoveDuplicateVertex(*this);
//	vcg::tri::Allocator<flora::XStaticMesh>::CompactEveryVector(*this);
//	vcg::tri::UpdateNormal<flora::XStaticMesh>::PerVertexNormalized(*this);
//	UpdateDataMask();
//}

int flora::XStaticMesh::RemoveUnreferencedVertex()
{
	return vcg::tri::Clean<flora::XStaticMesh>::RemoveUnreferencedVertex(*this);
}

void flora::XStaticMesh::RemoveDuplicate()
{
	vcg::tri::Clean<flora::XStaticMesh>::RemoveDuplicateVertex(*this);
	vcg::tri::Clean<flora::XStaticMesh>::RemoveDuplicateFace(*this);
	vcg::tri::Clean<flora::XStaticMesh>::RemoveDuplicateEdge(*this);
}

void flora::XStaticMesh::RemoveDegenerate()
{
	vcg::tri::Clean<flora::XStaticMesh>::RemoveDegenerateVertex(*this);
	vcg::tri::Clean<flora::XStaticMesh>::RemoveDegenerateFace(*this);
	vcg::tri::Clean<flora::XStaticMesh>::RemoveDegenerateEdge(*this);
}

void flora::XStaticMesh::CompactEveryVector()
{
	vcg::tri::Allocator<flora::XStaticMesh>::CompactEveryVector(*this);
}

void flora::XStaticMesh::UpdateBouding()
{
	vcg::tri::UpdateBounding<flora::XStaticMesh>::Box(*this);
}

void flora::XStaticMesh::RegenerateFaceNormal()
{
	vcg::tri::UpdateNormal<flora::XStaticMesh>::PerFaceNormalized(*this);
}

void flora::XStaticMesh::RegenerateVertexNormal()
{
	vcg::tri::UpdateNormal<flora::XStaticMesh>::PerVertexNormalized(*this);
}

void flora::XStaticMesh::RegenerateVertexNormalFromFace()
{
	vcg::tri::UpdateNormal<flora::XStaticMesh>::PerVertexFromCurrentFaceNormal(*this);
	vcg::tri::UpdateNormal<flora::XStaticMesh>::PerVertexNormalized(*this);
}

void flora::XStaticMesh::RegenerateEdge()
{
	//generic edge
	this->face.EnableFFAdjacency();

	vcg::tri::UpdateTopology<flora::XStaticMesh>::FaceFace(*this);
	vcg::tri::UpdateTopology<flora::XStaticMesh>::AllocateEdge(*this);
}

void flora::XStaticMesh::RegenerateMesh(const std::set<unsigned int>& InDeleteFaces)
{
	for (auto FaceId : InDeleteFaces)
	{
		vcg::tri::Allocator<flora::XStaticMesh>::DeleteFace(*this, face[FaceId]);
	};

	vcg::tri::Clean<flora::XStaticMesh>::RemoveDuplicateVertex(*this);
	vcg::tri::Allocator<flora::XStaticMesh>::CompactEveryVector(*this);

}

void flora::XStaticMesh::SetData(std::byte* VertexBuffer, size_t VertexBufferSize, BufferLayout VertexLayout, unsigned int* IndexBuffer, int IndexNum)
{
	if (!VertexBuffer || !IndexBuffer || VertexBufferSize == 0 || IndexNum == 0 || IndexNum % 3 != 0)return;

	unsigned int Stride = VertexLayout.GetStride();

	size_t FaceNum = IndexNum / 3;
	size_t VertexNum = VertexBufferSize / Stride;

	auto VertexIter = vcg::tri::Allocator<flora::XStaticMesh>::AddVertices(*this, VertexNum);

	flora::BufferElement Element;
	if (VertexLayout.GetElement("aPosition", Element) && VertexIter->IsCoordEnabled())
	{
		auto TmpIter = VertexIter;
		const std::byte* pBuffer = VertexBuffer;
		for (size_t VertexIdx = 0; VertexIdx < VertexNum; VertexIdx++)
		{
			glm::vec4 P;
			memcpy(&P, pBuffer + Stride * VertexIdx + Element.Offset, Element.Size);
			TmpIter->P() = { P.x,P.y,P.z };

			++TmpIter;
		}
	}

	if (VertexLayout.GetElement("aNormal", Element) && VertexIter->IsNormalEnabled())
	{
		auto TmpIter = VertexIter;
		const std::byte* pBuffer = VertexBuffer;
		for (size_t VertexIdx = 0; VertexIdx < VertexNum; VertexIdx++)
		{
			glm::vec4 N;
			memcpy(&N, pBuffer + Stride * VertexIdx + Element.Offset, Element.Size);
			TmpIter->N() = { N.x,N.y,N.z };

			++TmpIter;
		}
	}

	if (VertexLayout.GetElement("aUV", Element) && VertexIter->IsTexCoordEnabled())
	{
		auto TmpIter = VertexIter;
		const std::byte* pBuffer = VertexBuffer;
		for (size_t VertexIdx = 0; VertexIdx < VertexNum; VertexIdx++)
		{
			glm::vec4 T;
			memcpy(&T, pBuffer + Stride * VertexIdx + Element.Offset, Element.Size);
			TmpIter->T() = { T.x,T.y };

			++TmpIter;
		}
	}

	/*if (VertexLayout.GetElement("aColor", Element) && VertexIter->IsColorEnabled())
	{
		auto TmpIter = VertexIter;
		const std::byte* pBuffer = VertexBuffer;
		for (size_t VertexIdx = 0; VertexIdx < VertexNum; VertexIdx++)
		{
			vcg::Color4b C;
			memcpy(&C, pBuffer + Stride * VertexIdx + Element.Offset, Element.Size);
			TmpIter->C() = C;

			++TmpIter;
		}
	}*/

	for (size_t Idx = 0; Idx < FaceNum; Idx++)
	{
		vcg::tri::Allocator<flora::XStaticMesh>::AddFace(*this,
			IndexBuffer[Idx * 3 + 0],
			IndexBuffer[Idx * 3 + 1],
			IndexBuffer[Idx * 3 + 2]
		);
	};

	RemoveDuplicate();
	RemoveDegenerate();
	RemoveUnreferencedVertex();

	RegenerateFaceNormal();
	RegenerateVertexNormalFromFace();
	CompactEveryVector();

}

void flora::XStaticMesh::Enable(int InOpeningFileMask)
{
	if (InOpeningFileMask & vcg::tri::io::Mask::IOM_VERTTEXCOORD)
		UpdateDataMask(MM_VERTTEXCOORD);
	if (InOpeningFileMask & vcg::tri::io::Mask::IOM_WEDGTEXCOORD)
		UpdateDataMask(MM_WEDGTEXCOORD);
	if (InOpeningFileMask & vcg::tri::io::Mask::IOM_VERTCOLOR)
		UpdateDataMask(MM_VERTCOLOR);
	if (InOpeningFileMask & vcg::tri::io::Mask::IOM_FACECOLOR)
		UpdateDataMask(MM_FACECOLOR);
	if (InOpeningFileMask & vcg::tri::io::Mask::IOM_VERTRADIUS)
		UpdateDataMask(MM_VERTRADIUS);
	if (InOpeningFileMask & vcg::tri::io::Mask::IOM_CAMERA)
		UpdateDataMask(MM_CAMERA);
	if (InOpeningFileMask & vcg::tri::io::Mask::IOM_VERTQUALITY)
		UpdateDataMask(MM_VERTQUALITY);
	if (InOpeningFileMask & vcg::tri::io::Mask::IOM_FACEQUALITY)
		UpdateDataMask(MM_FACEQUALITY);
	if (InOpeningFileMask & vcg::tri::io::Mask::IOM_BITPOLYGONAL)
		UpdateDataMask(MM_POLYGONAL);
}
