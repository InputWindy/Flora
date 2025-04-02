#include <CoreMinimal.h>
#include "Voxelization.h"

void GetTargetN(int& TargetN, int MaxTriArea, int MaxTargetNum, int TotalTriArea, float MaxAreaExcceedTolerance)
{
	if (MaxAreaExcceedTolerance < 0 || MaxAreaExcceedTolerance >1)
	{
		TargetN = floor(TotalTriArea / MaxTriArea) + 1;
		TargetN = glm::max(1, glm::min(TargetN, MaxTargetNum));

		return;
	}

	int M, R;
	M = floor(TotalTriArea / MaxTriArea);
	R = TotalTriArea - M * MaxTriArea;

	/*
		向上对齐
	*/
	if ((float(R) / float(MaxTriArea)) > MaxAreaExcceedTolerance)
	{
		TargetN = floor(TotalTriArea / MaxTriArea) + 1;
	}
	else
	{
		TargetN = floor(TotalTriArea / MaxTriArea);
	}

	TargetN = glm::max(1, glm::min(TargetN, MaxTargetNum));

	XVERSE_CORE_INFO("TriAreaExcceed:{0}", (float(R) / float(MaxTriArea)))
	XVERSE_CORE_INFO("TotalTriArea:{0}", TotalTriArea)
	XVERSE_CORE_INFO("MaxAreaExcceedTolerance:{0}", MaxAreaExcceedTolerance)

}

void XVerse::FTriCollector::Do()
{
	int TargetN;
	GetTargetN(TargetN, Param.MaxTriArea, Param.MaxTargetNum, TotalTriArea, Param.MaxAreaExcceedTolerance);
	Param.MaxTriArea = TotalTriArea / TargetN;

	XVERSE_CORE_ERROR("TargetN:{0} , MaxArea:{1}",TargetN,Param.MaxTriArea)

	int Start = 0, End;

	for (; !Voxels->IsProcessed() && Start < Voxels->GetNodeNum();)
	{
		FTriCollectProcessor* Collector = new FTriCollectProcessor(this);

		Voxels->BFS(Start, End, Collector);
		Start = End == -1 ? Start + 1 : End;
		Collections.push_back(Collector);
	}

	std::stable_sort(Collections.begin(), Collections.end(), [](FTriCollectProcessor* R1, FTriCollectProcessor* R2)
		{
			return R1->TotalTriArea > R2->TotalTriArea;
		});


	// 把BFS边角料加进大块Collection里
	std::vector<FTriCollectProcessor*> FinishedCollections;
	for (int i = Collections.size() - 1; i >= 0; --i)
	{
		if (i < TargetN)
		{
			FinishedCollections.emplace_back(Collections[i]);
		}
		else
		{
			delete Collections[i];
		}
	}

	Collections.clear();
	Collections = FinishedCollections;

	for (FTriCollectProcessor* Collection : FinishedCollections)
	{
		std::queue<vxl::TLinkGraph<3>::GraphNode*> Q;

		// 先把自己聚类内的node全部标为finished，以免重复遍历
		for (int NodeID : Collection->NodeIDs)
		{
			auto [Node, ID] = Voxels->GetNode(NodeID);
			if (Node)
			{
				Node->MarkFinished();
			}
		}

		// 找到集合边界处的细碎体素
		for (int NodeID : Collection->NodeIDs)
		{
			auto [Node, ID] = Voxels->GetNode(NodeID);
			if (Node)
			{
				for (int NeiID : Node->GetNeighbors())
				{
					auto [NeiNode, DummyID] = Voxels->GetNode(NeiID);
					if (NeiNode && !NeiNode->IsFinished())
					{
						NeiNode->MarkFinished();
						Q.push(NeiNode);
					}
				}
			}
		}

		while (!Q.empty())
		{
			auto Front = Q.front(); Q.pop(); int CurNodeID = Front->GetID(); 
			for (int TriID : VXInfos[CurNodeID].TriIDs)
			{
				Collection->CollectedTriIDs.insert(TriID);
			}
			Collection->TotalTriArea += VXInfos[CurNodeID].Area;
			Collection->NodeIDs.push_back(CurNodeID);
		
			for (int NeiID : Front->GetNeighbors())
			{
				auto [NeiNode, DummyID] = Voxels->GetNode(NeiID);
				if (NeiNode && !NeiNode->IsFinished())
				{
					NeiNode->MarkFinished();
					Q.push(NeiNode);
				}
			}
		}
	}
}



std::shared_ptr<XVerse::XStaticMesh> XVerse::FTriCollector::GenericDebugMesh()
{
	std::string MeshName = "DebugMesh";

	std::shared_ptr<XStaticMesh> NewMesh = std::make_shared<XVerse::XStaticMesh>(nullptr, MeshName, glm::identity<glm::mat4>(), glm::identity<mat4>());

	NewMesh->Enable(vcg::tri::io::Mask::IOM_VERTCOORD | vcg::tri::io::Mask::IOM_VERTNORMAL | vcg::tri::io::Mask::IOM_VERTTEXCOORD | vcg::tri::io::Mask::IOM_VERTCOLOR);

	for (int i = 0; i < Voxels->GetSlice()[0]; ++i)
	{
		for (int j = 0; j < Voxels->GetSlice()[1]; ++j)
		{
			for (int k = 0; k < Voxels->GetSlice()[2]; ++k)
			{
				auto [Node, ID] = Voxels->GetNode({ i,j,k });
				if (Node && Node->HasMask(CameraRegion))
				{
					AppendDebugCube(*NewMesh, i, j, k);
				}
			}
		}
	}

	XVERSE_CORE_ERROR("BoundMin:{0},{1},{2}", Param.BoundMin.x, Param.BoundMin.y, Param.BoundMin.z)
	XVERSE_CORE_ERROR("BoundMax:{0},{1},{2}", Param.BoundMax.x, Param.BoundMax.y, Param.BoundMax.z)

	return NewMesh;
}

void XVerse::FTriCollector::AppendDebugCube(XStaticMesh& Mesh, int IdxX, int IdxY, int IdxZ)
{
	const static float vertices[] = 
	{
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	auto Center = Param.BoundMin;
	Center.x += (IdxX + 0.5f) * Param.VoxelSize;
	Center.y += (IdxY + 0.5f) * Param.VoxelSize;
	Center.z += (IdxZ + 0.5f) * Param.VoxelSize;

	for (int i = 0; i < 24; ++i)
	{
		const float* pTri1 = vertices + i * 3;

		vcg::tri::Allocator<XStaticMesh>::AddFace
		(
			Mesh,
			XPoint3{ Center.x + pTri1[0] * Param.VoxelSize,Center.y + pTri1[1] * Param.VoxelSize,Center.z + pTri1[2] * Param.VoxelSize },
			XPoint3{ Center.x + pTri1[3] * Param.VoxelSize,Center.y + pTri1[4] * Param.VoxelSize,Center.z + pTri1[5] * Param.VoxelSize },
			XPoint3{ Center.x + pTri1[6] * Param.VoxelSize,Center.y + pTri1[7] * Param.VoxelSize,Center.z + pTri1[8] * Param.VoxelSize }
		);
	}
}

//void XVerse::FTriCollector::Finalize(FinalizeFunc&& Func)
//{
//	for (int i = 0; i < Collections.size(); ++i)
//	{
//		FTriCollectProcessor* Collection = Collections[i];
//		Func(Collections[i], i);
//	}
//}

//void XVerse::FTriCollector::Finalize(int* CollectionBuffer)
//{
//	for (int i = 0; i < Collections.size(); ++i)
//	{
//		FTriCollectProcessor* Collection = Collections[i];
//		for (int TriID : Collection->CollectedTriIDs)
//		{
//			CollectionBuffer[TriID] = i + 1;
//		}
//	}
//}
