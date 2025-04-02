#pragma once
#include <type_traits>
#include <array>
#include <vector>

namespace vxl
{
	template<int Dim>
	class TLinkGraph;

	class ILinkGraphProcessor
	{
		template<int Dim>
		friend class TLinkGraph;
	public:
		ILinkGraphProcessor() {}
		virtual ~ILinkGraphProcessor() {}
	protected:

		// core interface 
		virtual bool IsFinished()const { return false; }
		
		// core interface 
		virtual bool Process(unsigned int ID) { return true; }
	};

	template<int Dim>
	class TLinkGraph
	{
		static_assert(Dim == 1 || Dim == 2 || Dim == 3, "Dim must be 1, 2, or 3!");
	public:
		typedef unsigned int GraphNodeBitMask;
		enum GraphNodeBitMask_
		{
			Visited = 1 << 21,
			Processed = 1 << 20,
			Finished = 1 << 19,
		};

		template<int N>
		class TGraphNode
		{
		public:
			template<int OtherDim>
			friend class TLinkGraph;

			TGraphNode()
			{
				for (int i = 0; i < N; ++i)
				{
					Neighbors[i] = -1;
				}
			}

			virtual ~TGraphNode() {}

			void SetMask(unsigned int InBitMask) { BitMask = BitMask | InBitMask; }
			void ClearMask(unsigned int InBitMask) { BitMask = BitMask & (~InBitMask); }
			bool HasMask(unsigned int InBitMask)const { return BitMask & InBitMask; }

			void MarkVisited() { SetMask(Visited); }
			void MarkProcessed() { SetMask(Processed); }
			void MarkFinished() { SetMask(Finished); }

			void UnmarkVisited() { ClearMask(Visited); }
			void UnmarkProcessed() { ClearMask(Processed); }
			void UnmarkFinished() { ClearMask(Finished); }

			bool IsBorder()const
			{
				for (int i = 0; i < N; ++i)
				{
					if (Neighbors[i] == -1) return true;
				}

				return false;
			}

			bool IsVisited()const { return HasMask(Visited); }
			bool IsProcessed()const { return HasMask(Processed); }
			bool IsFinished()const { return HasMask(Finished); }

			const std::array<int, N>& GetNeighbors()const { return Neighbors; }
			const unsigned int GetID()const { return ID; }
			const unsigned int GetMask()const { return BitMask; }
		protected:
			unsigned int BitMask = 0;

			unsigned int ID = 0;
			std::array<int, N> Neighbors;
		};

	protected:
		TLinkGraph() {}
	public:
		typedef std::array<unsigned int, Dim> InitializerType;

		TLinkGraph(const std::array<unsigned int,Dim>& InSlice) :Slice(InSlice)
		{
			int TotalSize = 0;
			{

				if (Slice.size())
				{
					int Tmp = 1;

					for (unsigned int Size : InSlice)
					{
						Tmp *= Size;
					}

					TotalSize = Tmp;
				}

			}

			Graph.clear();
			Graph.resize(TotalSize);
			
			if (!Graph.size()) return;

			std::array<int, Dim> TraverseIdx = { 0 };
			std::array<std::array<int, Dim>, Dim * 2> NeigIdx;

			if (Dim == 1)
			{
				for (int i = 0; i < Slice[0]; ++i)
				{
					TraverseIdx[0] = i;

					for (int m = 0; m < Dim * 2; ++m)
						for (int n = 0; n < Dim; ++n)
							NeigIdx[m][n] = TraverseIdx[n];


					NeigIdx[0][0] -= 1; NeigIdx[1][0] += 1;

					auto [Node, ID] = GetNode(TraverseIdx);
					if (Node)
					{
						Node->ID = ID;
						auto [NeiNodeNX, NXID] = GetNode(NeigIdx[0]);
						auto [NeiNodePX, PXID] = GetNode(NeigIdx[1]);

						Node->Neighbors[0] = NeiNodeNX ? NXID : -1;
						Node->Neighbors[1] = NeiNodePX ? PXID : -1;
					};
				}
			}
			else if (Dim == 2)
			{
				for (int i = 0; i < Slice[0]; ++i)
				{
					TraverseIdx[0] = i;
					for (int j = 0; j < Slice[1]; ++j)
					{
						TraverseIdx[1] = j;

						for (int m = 0; m < Dim * 2; ++m)
							for (int n = 0; n < Dim; ++n)
								NeigIdx[m][n] = TraverseIdx[n];


						NeigIdx[0][0] -= 1; NeigIdx[1][0] += 1;
						NeigIdx[2][1] -= 1; NeigIdx[3][1] += 1;

						auto [Node, ID] = GetNode(TraverseIdx);
						if (Node)
						{
							Node->ID = ID;
							auto [NeiNodeNX, NXID] = GetNode(NeigIdx[0]);
							auto [NeiNodePX, PXID] = GetNode(NeigIdx[1]);
							auto [NeiNodeNY, NYID] = GetNode(NeigIdx[2]);
							auto [NeiNodePY, PYID] = GetNode(NeigIdx[3]);

							Node->Neighbors[0] = NeiNodeNX ? NXID : -1;
							Node->Neighbors[1] = NeiNodePX ? PXID : -1;
							Node->Neighbors[2] = NeiNodeNY ? NYID : -1;
							Node->Neighbors[3] = NeiNodePY ? PYID : -1;
						};
					}

				}
			}
			else if (Dim == 3)
			{
				for (int i = 0; i < Slice[2]; ++i)
				{
					TraverseIdx[2] = i;
					for (int j = 0; j < Slice[1]; ++j)
					{
						TraverseIdx[1] = j;
						for (int k = 0; k < Slice[0]; ++k)
						{
							TraverseIdx[0] = k;

							for (int m = 0; m < Dim * 2; ++m)
								for (int n = 0; n < Dim; ++n)
									NeigIdx[m][n] = TraverseIdx[n];
								

							NeigIdx[0][0] -= 1; NeigIdx[1][0] += 1;
							NeigIdx[2][1] -= 1; NeigIdx[3][1] += 1;
							NeigIdx[4][2] -= 1; NeigIdx[5][2] += 1;

							auto [Node, ID] = GetNode(TraverseIdx);
							if (Node)
							{
								Node->ID = ID;
								auto [NeiNodeNX, NXID] = GetNode(NeigIdx[0]);
								auto [NeiNodePX, PXID] = GetNode(NeigIdx[1]);
								auto [NeiNodeNY, NYID] = GetNode(NeigIdx[2]);
								auto [NeiNodePY, PYID] = GetNode(NeigIdx[3]);
								auto [NeiNodeNZ, NZID] = GetNode(NeigIdx[4]);
								auto [NeiNodePZ, PZID] = GetNode(NeigIdx[5]);

								Node->Neighbors[0] = NeiNodeNX ? NXID : -1;
								Node->Neighbors[1] = NeiNodePX ? PXID : -1;
								Node->Neighbors[2] = NeiNodeNY ? NYID : -1;
								Node->Neighbors[3] = NeiNodePY ? PYID : -1;
								Node->Neighbors[4] = NeiNodeNZ ? NZID : -1;
								Node->Neighbors[5] = NeiNodePZ ? PZID : -1;
							};
						}
					}
				}
			}
			else
			{
				//static_assert(0)
			}
		};
	public:
		virtual ~TLinkGraph() {}

		typedef TGraphNode<Dim * 2> GraphNode;

		template<typename TProcessor = ILinkGraphProcessor>
			//requires std::is_base_of<ILinkGraphProcessor, TProcessor>::value
		bool BFS(unsigned int Start, int& End, TProcessor* Processor)
		{
			if (!Processor || Start >= Graph.size())return false;

			constexpr int N = Dim * 2;

			End = -1;

			std::vector<GraphNode*> ProcessedNodes;
			std::queue<GraphNode*> Q;

			Graph[Start].SetMask(Visited);
			Q.push(&Graph[Start]);

			while (!Q.empty() && !Processor->IsFinished())
			{
				GraphNode* CurNode = Q.front(); Q.pop();

				Processor->Process(CurNode->ID);
				CurNode->SetMask(Processed);
				ProcessedNodes.push_back(CurNode);

				for (int i = 0; i < N; ++i)
				{
					int NodeID = CurNode->Neighbors[i];

					// add valid neighbor node
					if (NodeID >= 0 && NodeID < Graph.size() && !Graph[NodeID].HasMask(Visited))
					{
						Graph[NodeID].SetMask(Visited);
						Q.push(&Graph[NodeID]);
					}
				}
			}

			if (Processor->IsFinished())
			{
				for (GraphNode* Node : ProcessedNodes)
				{
					Node->SetMask(Finished);
				}
			}

			if (!Q.empty())
			{
				End = Q.front()->ID;
				while (!Q.empty())
				{
					GraphNode* CurNode = Q.front(); Q.pop();
					CurNode->ClearMask(Visited | Processed);
				}
			}

			return true;
		}

		std::pair<GraphNode*, int> GetNode(const std::array<int, Dim>& CurIdx)
		{
			for (int i = 0; i < Dim;++i)
			{
				if (CurIdx[i] < 0 || CurIdx[i] >= Slice[i])return std::pair<GraphNode*, int>{ nullptr, -1 };
			}

			int ID = -1;

			if (Dim == 1)ID = CurIdx[0];
			else
			{
				ID = 0;
				int Base = 1;

				for (int i = 0; i < Dim; ++i)
				{
					ID += Base * CurIdx[i];

					Base *= Slice[i];

				}
			}

			if (ID >= Graph.size() || ID < 0)return std::pair<GraphNode*, int>{ nullptr, -1 };

			return std::pair<GraphNode*, int>{ &Graph[ID], ID };
		}

		std::pair<GraphNode*, int> GetNode(const int& CurIdx)
		{
			if (CurIdx < 0 || CurIdx >= Graph.size())return { nullptr,-1 };

			return { &Graph[CurIdx],CurIdx };
		}

		std::vector<GraphNode*> GetNodesWithMask(unsigned int InMask)
		{
			std::vector<GraphNode*> Ret;

			for (GraphNode& Node : Graph)
			{
				if (Node.HasMask(InMask))Ret.push_back(&Node);
			}

			return Ret;
		}

		std::vector<GraphNode*> GetNodesWithoutMask(unsigned int InMask)
		{
			std::vector<GraphNode*> Ret;

			for (GraphNode& Node : Graph)
			{
				if (!Node.HasMask(InMask))Ret.push_back(&Node);
			}

			return Ret;
		}

		void ClearMask(unsigned int InMask)
		{
			for (GraphNode& Node : Graph)
			{
				Node.ClearMask(InMask);
			}
		}

		int GetNodeNum()const { return Graph.size(); }

		bool IsProcessed()const
		{
			for (const GraphNode& Node : Graph)
			{
				if (!Node.IsProcessed())return false;
			}
			return true;
		}

		auto& GetSlice() { return Slice; }

	protected:
		std::vector<GraphNode> Graph;

		std::array<unsigned int, Dim> Slice;
	};

}

#include <set>
#include <memory>
#include <functional>

#include <glm/glm.hpp>

namespace XVerse
{
	struct FVoxelInfo
	{
		std::vector<int > TriIDs = {};
		float Area = 0;
	};

	struct FTriCollectorParam
	{
		glm::vec4 BoundMin, BoundMax;
		float VoxelSize = 0.5f;
		float MaxTriArea = 200.f;
		float MaxAreaExcceedTolerance = 0.2f;
		int MaxTargetNum = 100;
	};
	
	class FTriCollectProcessor;

	class FTriCollector
	{
		enum
		{
			CameraRegion = 1 << 0
		};

		friend class FTriCollectProcessor;

		typedef vxl::TLinkGraph<3>::InitializerType InitializerType;
		typedef std::function<void(FTriCollectProcessor*,int)> FinalizeFunc;

	public:
		FTriCollector(const FTriCollectorParam& InParam)
			:Param(InParam)
		{
			auto Bound = Param.BoundMax - Param.BoundMin;
			unsigned int SliceX = glm::floor(Bound.x / Param.VoxelSize) + 1;
			unsigned int SliceY = glm::floor(Bound.y / Param.VoxelSize) + 1;
			unsigned int SliceZ = glm::floor(Bound.z / Param.VoxelSize) + 1;

			Voxels = std::make_unique<vxl::TLinkGraph<3>>(InitializerType{ SliceX,SliceY,SliceZ });

			VXInfos.resize(Voxels->GetNodeNum());
		}

		virtual ~FTriCollector()
		{
			for (auto Collector : Collections)
			{
				if (Collector)
				{
					delete Collector;
					Collector = nullptr;
				}
			}
		}

		bool AddTri(glm::vec3 Tri[3], int TriID,glm::vec4 Sphere)
		{
			auto TriArea = [](const glm::vec3& V0, const glm::vec3& V1, const glm::vec3& V2)
				{
					// 计算两个边的向量
					glm::vec3 edge1 = V1 - V0;
					glm::vec3 edge2 = V2 - V0;

					// 计算叉积
					glm::vec3 crossProduct = glm::cross(edge1, edge2);

					// 计算面积（叉积的模的一半）
					float area = 0.5f * glm::length(crossProduct);

					return area;
				};

			auto Center = (Tri[0] + Tri[1] + Tri[2]) / glm::vec3(3, 3, 3); 

			if (glm::distance(Center, vec3(Sphere)) > Sphere.w)
			{
				return false;
			}

			Center -= glm::vec3(Param.BoundMin);
			auto Area = TriArea(Tri[0], Tri[1], Tri[2]);

			int IdxX = glm::floor(Center.x / Param.VoxelSize);
			int IdxY = glm::floor(Center.y / Param.VoxelSize);
			int IdxZ = glm::floor(Center.z / Param.VoxelSize);

			auto [Node, Idx] = Voxels->GetNode({ IdxX ,IdxY ,IdxZ });

			if (Node->HasMask(CameraRegion))
			{
				VXInfos[Idx].Area += Area;
				VXInfos[Idx].TriIDs.push_back(TriID);

				TotalTriNum++;
				TotalTriArea += Area;
			
				return true;
			}

			return false;
		}

		void AddCameraPoint(const glm::vec3& Point,float Radius)
		{
			vec3 BoxMin = { Point.x - Radius, Point.y - Radius, Point.z - Radius }; BoxMin -= vec3(Param.BoundMin);
			vec3 BoxMax = { Point.x + Radius, Point.y + Radius, Point.z + Radius }; BoxMax -= vec3(Param.BoundMin);

			int IdxMinX = glm::floor(BoxMin.x / Param.VoxelSize);
			int IdxMinY = glm::floor(BoxMin.y / Param.VoxelSize);
			int IdxMinZ = glm::floor(BoxMin.z / Param.VoxelSize);

			int IdxMaxX = glm::floor(BoxMax.x / Param.VoxelSize);
			int IdxMaxY = glm::floor(BoxMax.y / Param.VoxelSize);
			int IdxMaxZ = glm::floor(BoxMax.z / Param.VoxelSize);

			for (int i = IdxMinX; i <= IdxMaxX; ++i)
			{
				for (int j = IdxMinY; j <= IdxMaxY; ++j)
				{
					for (int k = IdxMinZ; k <= IdxMaxZ; ++k)
					{
						auto [Node, ID] = Voxels->GetNode({ i,j,k });
						if (Node)
						{
							Node->SetMask(CameraRegion);
							//XVERSE_CORE_ERROR("Idx Max:{0},{1},{2}", i,j,k)
						}
					}
				}
			}
		}

		void Do() ;

		template<typename TLambda>
		void Finalize(TLambda&& Lambda)
		{
			for (int i = 0; i < Collections.size(); ++i)
			{
				FTriCollectProcessor* Collection = Collections[i];
				Lambda(Collections[i], i);
			}
		};

		std::shared_ptr<XStaticMesh> GenericDebugMesh();

	private:
		void AppendDebugCube(XStaticMesh& Mesh, int IdxX, int IdxY, int IdxZ);

	private:

		std::vector<FTriCollectProcessor*> Collections;
		std::unique_ptr<vxl::TLinkGraph<3>> Voxels;

		FTriCollectorParam Param;

		std::vector<FVoxelInfo> VXInfos;

	private:
		/*
			debug
		*/
		int TotalTriNum = 0;
		float TotalTriArea = 0;
	};

	class FTriCollectProcessor :public vxl::ILinkGraphProcessor
	{
		friend class FTriCollector;
	public:
		FTriCollectProcessor(FTriCollector* InCollector) :Collector(InCollector) {}

		// core interface 
		virtual bool IsFinished()const
		{
			return TotalTriArea > Collector->Param.MaxTriArea;
		};

		// core interface 
		virtual bool Process(unsigned int ID)
		{
			FVoxelInfo& VX = Collector->VXInfos[ID];

			for (auto i : VX.TriIDs)
			{
				CollectedTriIDs.insert(i);
			}

			TotalTriArea += VX.Area;
			NodeIDs.push_back(ID);

			return true;
		};

	protected:
		FTriCollector* Collector = nullptr;

	public:
		float TotalTriArea = 0;
		std::set<int> CollectedTriIDs;
		std::vector<int> NodeIDs;
	};
}
