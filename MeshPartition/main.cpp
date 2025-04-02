#include <CoreMinimal.h>
#include "Voxelization.h"
#include "Tessellation.h"
using namespace flora;

template<>
std::shared_ptr<flora::IEditor> flora::IEditor::InitEditor<EEditorType::ET_ImGui>(void* InWindowHandle)
{
	return nullptr;
};

using namespace glm;

#include "Shaders/ShaderDefines.h"
#include "Shaders/ShaderStructs.h"

#define STR_CAT(Str1,Str2) Str1##Str2

std::string flora::ExcutePath;


struct FBakeParam:public ISerialization
{
	struct RoamingArea
	{
		std::vector<glm::vec3> CameraPoints;
	};

	virtual bool Parse(const json11::Json& In)
	{
		HighModelPath = In["highModelPath"].string_value();
		ModelPath = In["modelPath"].string_value();
		ViewDistance = In["viewDistance"].number_value();
		MaxTriArea = In["maxTriArea"].number_value();
		MaxBlockNumInside = In["maxBlockNumInside"].int_value();
		MaxBlockNumOutside = In["maxBlockNumOutside"].int_value();
		MaxTriAreaPerBaseColor = In["maxTriAreaPerBaseColor"].number_value();
		MaxAreaExcceedTolerance = In["maxAreaExcceedTolerance"].number_value();

		for (auto Item : In["pathRefImg"].array_items())
		{
			RoamingArea Area;

			for (auto PointItem : Item["cameraPoints"].array_items())
			{
				Area.CameraPoints.emplace_back(glm::vec3{
					PointItem["x"].number_value(),
					PointItem["y"].number_value(),
					PointItem["z"].number_value(),
					});
			}
			PathRefImg.emplace_back(Area);
		}
		return true;
	};

	virtual bool Serialize(json11::Json& Out)
	{
		return true;
	};

	// ������ֻ������1����������ĵ�ͼ
	bool IsValid()
	{
		return PathRefImg.size() == 1;
	}

	std::pair< bool,glm::vec4> TryGetBoudingSphere()
	{
		if (IsValid())
		{
			glm::vec4 Bounding;

			vcg::XBox3 Box;

			for (const glm::vec3& Point : PathRefImg[0].CameraPoints)
			{
				Box.Add({ Point.x, Point.y, Point.z });
			}

			return { true ,{ Box.Center().X() / 100.f, Box.Center().Z() / 100.f, -Box.Center().Y() / 100.f,Box.Diag() / 200.f + ViewDistance } };
		}
		return { false,{} };
	}

	std::string HighModelPath;
	std::string ModelPath;

	std::vector<RoamingArea> PathRefImg;

	float ViewDistance = 5.0f;
	float MaxTriArea = 0.1f;
	float MaxTriAreaPerBaseColor = 200.f;
	float MaxAreaExcceedTolerance = 0.2f;
	int MaxBlockNumInside = 100;
	int MaxBlockNumOutside = 4;
};

struct FProfilingResult :public ISerialization
{
	struct MeshInfo
	{
		int   TriNum = 0;
		float TriNumPercent = 0;
		
		float TriArea = 0;
		float TriAreaPercent = 0;
	};

	virtual bool Parse(const json11::Json& In)
	{
		return true;
	};

	virtual bool Serialize(json11::Json& Out)
	{
		std::map<std::string, json11::Json> JsonMap;

		int Idx = 1;

		for (auto& Info : MeshInfos)
		{
			auto obj = json11::Json::object
			{
				{"TriNum",Info.second.TriNum},
				{"TriNumPercent",Info.second.TriNumPercent},
				{"TriArea",Info.second.TriArea},
				{"TriAreaPercent",Info.second.TriAreaPercent}
			};

			if (Info.first < LO0Num)
			{
				std::string Title = "0_" + std::to_string(Info.first);

				JsonMap.insert(std::pair< std::string, json11::Json >{ Title, obj });
			}
			else
			{
				JsonMap.insert(std::pair< std::string, json11::Json >{ std::to_string(Idx), obj });
				++Idx;
			}

		}

		Out = json11::Json::object
		{
			{"TotalTriNum",TotalTriNum},
			{"TotalTriArea",TotalTriArea},
			{"LO0Num",LO0Num},
			{"MeshInfos",JsonMap}
		};

		return true;
	};

	std::map<int,MeshInfo> MeshInfos;

	int TotalTriNum = 0;
	float TotalTriArea = 0;
	int LO0Num = 0;
};

struct MeshPartitionApp : public flora::IApp
{
	typedef FStructuredBuffer<VERTEX_BUFFER_SLOT, FVertex, EBufferUsage::BU_STATIC_COPY> FVertexBuffer;
	typedef FStructuredBuffer<TRIANGLE_BUFFER_SLOT, ivec3, EBufferUsage::BU_STATIC_COPY> FTriangleBuffer;
	typedef FStructuredBuffer<MESH_BUFFER_SLOT, FMesh, EBufferUsage::BU_STATIC_COPY> FMeshBuffer;
	typedef FStructuredBuffer<SCENE_BUFFER_SLOT, FScene, EBufferUsage::BU_STATIC_COPY> FSceneBuffer;
	typedef FStructuredBuffer<TRIANGLE_COLLECTION_BUFFER_SLOT, int, EBufferUsage::BU_DYNAMIC_COPY> FTriangleCollectionBuffer;
	typedef FStructuredBuffer<TRIANGLE_COLLECTION_PROFILING_SLOT, FResults, EBufferUsage::BU_DYNAMIC_COPY> FTriangleCollectionProfilingResultsBuffer;
	typedef FStructuredBuffer<DEBUG_SLOT, vec4, EBufferUsage::BU_DYNAMIC_COPY> FDebugTriangleCollectionBuffer;

	MeshPartitionApp(int argc, char* argv[])
	{
		bUseGPU = false;

		WindowDesc.Name = "MeshPartition";
		WindowDesc.Width = 1024;
		WindowDesc.Height = 1024;
		WindowDesc.bHideWindow = true;

		ExcutePath = std::filesystem::canonical(std::filesystem::current_path()).generic_string();
		LogPath = "log.txt";

		ParseCmdline(argc, argv);
	}
	virtual void StartUp()override
	{
		IApp::StartUp();
		BakeParam.LoadFromFile(BakeParamPath);

		auto [bValid, Sphere] = BakeParam.TryGetBoudingSphere();

		if (!bValid)throw std::runtime_error("roaming area must be one .");

		Param.MaxTargetNum = BakeParam.MaxBlockNumInside;
		Param.MaxTriArea = BakeParam.MaxTriAreaPerBaseColor;
		Param.MaxAreaExcceedTolerance = BakeParam.MaxAreaExcceedTolerance;
		Param.VoxelSize = 0.5f;
		Param.BoundMin = { Sphere.x - Sphere.w,Sphere.y - Sphere.w ,Sphere.z - Sphere.w,1 };
		Param.BoundMax = { Sphere.x + Sphere.w,Sphere.y + Sphere.w ,Sphere.z + Sphere.w,1 };

		LoadModel(BakeParam.HighModelPath, Sphere, BakeParam.MaxTriArea);
	};

	virtual void Run()override
	{
		auto [bValid, Sphere] = BakeParam.TryGetBoudingSphere();

		int LOD0Num = 0;
		std::vector<int> OutsideTriangles;

		FTriCollector Collector(Param);
		{
			for (int Idx = 0; Idx < BakeParam.PathRefImg[0].CameraPoints.size(); Idx += 6)
			{
				vec3 Point = BakeParam.PathRefImg[0].CameraPoints[Idx];
				Collector.AddCameraPoint(vec3{ Point.x,Point.z,-Point.y } / 100.f, BakeParam.ViewDistance);
			}

			//DebugMesh = Collector.GenericDebugMesh();


			for (int i = 0; i < TriangleBuffer.GetNum(); ++i)
			{
				FVertex* V0 = &VertexBuffer[TriangleBuffer[i].x];
				FVertex* V1 = &VertexBuffer[TriangleBuffer[i].y];
				FVertex* V2 = &VertexBuffer[TriangleBuffer[i].z];

				glm::vec3 Tri[3];
				Tri[0] = { V0->Position.x,V0->Position.y,V0->Position.z };
				Tri[1] = { V1->Position.x,V1->Position.y,V1->Position.z };
				Tri[2] = { V2->Position.x,V2->Position.y,V2->Position.z };

				if (!Collector.AddTri(Tri, i, Sphere))
				{
					OutsideTriangles.push_back(i);
				};
			}

			int* pCollectionBuffer = TriangleCollectionBuffer.GetData();

			Collector.Do();
			Collector.Finalize([&LOD0Num,pCollectionBuffer](FTriCollectProcessor* Processor, int ID)
				{
					for (int Tri : Processor->CollectedTriIDs)
					{
						pCollectionBuffer[Tri] = ID;
					}

					LOD0Num = ID + 1;
				});

		}

		for (int i = 0; i < OutsideTriangles.size(); ++i)
		{
			FVertex* V0 = &VertexBuffer[TriangleBuffer[OutsideTriangles[i]].x];
			FVertex* V1 = &VertexBuffer[TriangleBuffer[OutsideTriangles[i]].y];
			FVertex* V2 = &VertexBuffer[TriangleBuffer[OutsideTriangles[i]].z];

			glm::vec3 Tri[3];
			Tri[0] = { V0->Position.x,V0->Position.y,V0->Position.z };
			Tri[1] = { V1->Position.x,V1->Position.y,V1->Position.z };
			Tri[2] = { V2->Position.x,V2->Position.y,V2->Position.z };

			auto Center = (Tri[0] + Tri[1] + Tri[2]) / glm::vec3(3, 3, 3);

			float Dist = sqrt((Center.x - Sphere.x) * (Center.x - Sphere.x) + (Center.y - Sphere.y) * (Center.y - Sphere.y) + (Center.z - Sphere.z) * (Center.z - Sphere.z));

			TriangleCollectionBuffer[OutsideTriangles[i]] = glm::min(BakeParam.MaxBlockNumOutside - 1, int(floor(Dist / (2 * Sphere.w))));
			TriangleCollectionBuffer[OutsideTriangles[i]] += LOD0Num;
		}

		NumRegionInside = LOD0Num;
	}

	virtual void Shutdown()override
	{
		for (int i = 0; i < TriangleCollectionBuffer.GetNum(); ++i)
		{
			TriangleCollections[TriangleCollectionBuffer[i]].insert(i);
		}

		SaveModel(BakeParam.HighModelPath, BakeParam.ModelPath);
		SaveProfilingResults(BakeParam.ModelPath + "/results.json", TriangleCollections);

		IApp::Shutdown();

		FLORA_CORE_INFO("Done .")
	}

	private:

		void LoadModel(const std::string& Filepath,vec4 Sphere,float MaxTriArea)
		{
			// ��ȡ��������
			if (auto RH = IResource<>::Open<XImporter>(Filepath, AssimpImportSettings()))
			{
				std::shared_ptr<flora::XModelMesh> HighModel = RH->DynamicPointerCast<XModelMesh>();
				HighModel->Register();

				for (int i = 0; i < HighModel->GetMeshes().size(); ++i)
				{
					auto Mesh = HighModel->GetMeshes()[i];

					int T = 3;
					while (T--)
					{
						tsl::Tessellation<tsl::SimpleSubdivision>(Mesh.get(), vec4(vec3(Sphere), (Sphere.w + BakeParam.ViewDistance) * 1.414), MaxTriArea);
					}
				}

				Meta = HighModel->GetMetaData();

				int NumVertex = 0;
				int NumTriangles = 0;
				int NumMeshes = 0;

				NumMeshes = HighModel->GetMeshes().size();

				MeshBuffer.Resize(NumMeshes);

				for (int i = 0; i < NumMeshes; ++i)
				{
					MeshBuffer[i].GlobalTriOffset = NumTriangles;

					auto Mesh = HighModel->GetMeshes()[i];

					NumTriangles += Mesh->FN();
					NumVertex += Mesh->VN();
				}

				SceneBuffer.Resize(1);
				SceneBuffer[0].NumMeshes = NumMeshes;
				SceneBuffer[0].NumTriangles = NumTriangles;

				VertexBuffer.Resize(NumVertex);
				TriangleBuffer.Resize(NumTriangles);
				TriangleCollectionBuffer.Resize(NumTriangles, -1);
				TriangleCollectionProfilingResultsBuffer.Resize(NumTriangles);
				DebugTriangleCollectionBuffer.Resize(NumTriangles);
				{
					int VertId = 0;
					int TriId = 0;

					vcg::XBox3 SceneBox;

					for (int i = 0; i < NumMeshes; ++i)
					{
						auto Mesh = HighModel->GetMeshes()[i];

						for (auto& f : Mesh->face)
						{
							TriangleBuffer[TriId] =
							{
								f.cV(0)->Index(),
								f.cV(1)->Index(),
								f.cV(2)->Index()
							};

							TriangleCollectionBuffer[TriId] = 0;

							++TriId;
						}

						vcg::XBox3 Box;

						for (auto& v : Mesh->vert)
						{
							FVertex V;
							V.Position =
							{
								v.cP().X(),
								v.cP().Y(),
								v.cP().Z(),
								1
							};

							V.Normal =
							{
								v.cN().X(),
								v.cN().Y(),
								v.cN().Z(),
								0
							};

							V.UV =
							{
								v.cT().U(),
								v.cT().V(),
								0,0
							};

							VertexBuffer[VertId++] = V;

							Box.Add({
									v.cP().X(),
									v.cP().Y(),
									v.cP().Z()
								});
						}

						MeshBuffer[i].BoundingSphere =
						{
							Box.Center().X(),
							Box.Center().Y(),
							Box.Center().Z(),
							Box.Diag() / 2.0f
						};

						SceneBox.Add(Box);
					}

					SceneBuffer[0].BoundingSphere =
					{
						SceneBox.Center().X(),
						SceneBox.Center().Y(),
						SceneBox.Center().Z(),
						SceneBox.Diag() / 2.0f
					};
				}
			}
		}
		void SaveModel(const std::string& OriginPath, const std::string& Folder)
		{
			GlobalUtils::CreateDirectoryRecursive(Folder);

			for (int i = 0; i < NumRegionInside; ++i)
			{
				const auto& Collection = TriangleCollections[i];

				if (Collection.size())
				{
					std::filesystem::path p(OriginPath);

					std::string SavePath = Folder;
					SavePath += "/";
					SavePath += p.filename().replace_extension().generic_string();
					SavePath += "_0_";
					SavePath += std::to_string(i);
					SavePath += p.filename().extension().generic_string();

					std::shared_ptr<XModelMesh> NewModel = std::make_shared<XModelMesh>();
					NewModel->SetMetaData(Meta);

					// ����һ��RootNode����RootNode�¹���һ��StaticMesh
					if (auto RootNode = XModelMesh::AllocateNode("RootNode", nullptr))
					{
						auto StaticMesh = RootNode->AllocateStaticMesh();

						auto [VtxBuffer, IdxBuffer, Layout] = PartitionInternal(VertexBuffer.GetData(), TriangleBuffer.GetData(), Collection);
						StaticMesh->SetData(VtxBuffer.data(), VtxBuffer.size(), Layout, IdxBuffer.data(), IdxBuffer.size());

						NewModel->SetTopology(RootNode);
					}

					AssimpExportSettings ExportSettings;
					ExportSettings.bExportUV0 = true;
					IResource<>::Save<XExporter>(*NewModel, SavePath, ExportSettings);
				}
			}

			int Idx = 1;

			for (int i = NumRegionInside; i < TriangleCollections.size(); ++i)
			{
				const auto& Collection = TriangleCollections[i];

				std::filesystem::path p(OriginPath);

				std::string SavePath = Folder;
				SavePath += "/";
				SavePath += p.filename().replace_extension().generic_string();
				SavePath += "_";
				SavePath += std::to_string(Idx);
				SavePath += p.filename().extension().generic_string();

				std::shared_ptr<XModelMesh> NewModel = std::make_shared<XModelMesh>();
				NewModel->SetMetaData(Meta);

				// ����һ��RootNode����RootNode�¹���һ��StaticMesh
				if (auto RootNode = XModelMesh::AllocateNode("RootNode", nullptr))
				{
					auto StaticMesh = RootNode->AllocateStaticMesh();

					auto [VtxBuffer, IdxBuffer, Layout] = PartitionInternal(VertexBuffer.GetData(), TriangleBuffer.GetData(), Collection);
					StaticMesh->SetData(VtxBuffer.data(), VtxBuffer.size(), Layout, IdxBuffer.data(), IdxBuffer.size());

					NewModel->SetTopology(RootNode);
				}

				AssimpExportSettings ExportSettings;
				ExportSettings.bExportUV0 = true;
				IResource<>::Save<XExporter>(*NewModel, SavePath, ExportSettings);

				++Idx;
			}

			//{
			//	std::string SavePath = Folder;
			//	SavePath += "/DebugMesh.obj";

			//	std::shared_ptr<XModelMesh> NewModel = std::make_shared<XModelMesh>();
			//	NewModel->SetMetaData(Meta);

			//	// ����һ��RootNode����RootNode�¹���һ��StaticMesh
			//	if (auto RootNode = XModelMesh::AllocateNode("RootNode", nullptr))
			//	{
			//		RootNode->AppendStaticMesh(DebugMesh);

			//		NewModel->SetTopology(RootNode);
			//	}

			//	AssimpExportSettings ExportSettings;
			//	ExportSettings.bExportUV0 = true;
			//	IResource<>::Save<XExporter>(*NewModel, SavePath, ExportSettings);
			//}


		}

		void SaveProfilingResults(const std::string& SavePath, const std::map<int, std::set<int>>& TriangleCollections)
		{
			Result.LO0Num = NumRegionInside;

			// profiling
			for (auto& Collection : TriangleCollections)
			{
				int CollectionID = Collection.first;

				if (Collection.second.size())
				{
					for (const int Tri : Collection.second)
					{
						FVertex* V0 = &VertexBuffer[TriangleBuffer[Tri].x];
						FVertex* V1 = &VertexBuffer[TriangleBuffer[Tri].y];
						FVertex* V2 = &VertexBuffer[TriangleBuffer[Tri].z];

						float Area = TriArea(V0->Position, V1->Position, V2->Position);

						if (Result.MeshInfos.find(CollectionID) == Result.MeshInfos.end())Result.MeshInfos.insert({ CollectionID,{} });

						Result.TotalTriNum++;
						Result.TotalTriArea += Area;
						Result.MeshInfos[CollectionID].TriArea += Area;
						Result.MeshInfos[CollectionID].TriNum++;
					}
				}
			}

			for (auto& Info : Result.MeshInfos)
			{
				Info.second.TriAreaPercent = Info.second.TriArea / Result.TotalTriArea;
				Info.second.TriNumPercent = Info.second.TriNum / float(Result.TotalTriNum);
			}

			Result.SaveToFile(SavePath);
		}

	private:

		float TriArea(const glm::vec3& V0, const glm::vec3& V1, const glm::vec3& V2)
		{
			// ���������ߵ�����
			glm::vec3 edge1 = V1 - V0;
			glm::vec3 edge2 = V2 - V0;

			// ������
			glm::vec3 crossProduct = glm::cross(edge1, edge2);

			// ��������������ģ��һ�룩
			float area = 0.5f * glm::length(crossProduct);

			return area;
		};

		template<typename TVertex>
		BufferLayout GetLayoutFromVertexType();

		template<typename TVertex>
		std::tuple<std::vector<std::byte>, std::vector<uint32_t>, BufferLayout> PartitionInternal(TVertex*VertexBuffer,ivec3* TriangleBuffer, const std::set<int>& Collection)
			{
				// key : vert addr, value :vertid
				std::map<TVertex*, int> CollectionLUT;// ��ӳ�䶥������

				for (int TriId : Collection)
				{
					TVertex* V0 = &VertexBuffer[TriangleBuffer[TriId].x];
					TVertex* V1 = &VertexBuffer[TriangleBuffer[TriId].y];
					TVertex* V2 = &VertexBuffer[TriangleBuffer[TriId].z];

					if (CollectionLUT.find(V0) == CollectionLUT.end())
					{
						int NewId = CollectionLUT.size();
						CollectionLUT.insert({ V0,NewId });
					}

					if (CollectionLUT.find(V1) == CollectionLUT.end())
					{
						int NewId = CollectionLUT.size();
						CollectionLUT.insert({ V1,NewId });
					}

					if (CollectionLUT.find(V2) == CollectionLUT.end())
					{
						int NewId = CollectionLUT.size();
						CollectionLUT.insert({ V2,NewId });
					}
				}

				std::vector<std::byte>VtxBuffer(CollectionLUT.size() * sizeof(TVertex));
				std::vector<uint32_t> IdxBuffer(Collection.size() * 3, 0);
				{
					int TriIdx = 0;

					for (int TriId : Collection)
					{
						TVertex* V0 = &VertexBuffer[TriangleBuffer[TriId].x];
						TVertex* V1 = &VertexBuffer[TriangleBuffer[TriId].y];
						TVertex* V2 = &VertexBuffer[TriangleBuffer[TriId].z];

						int iV0 = CollectionLUT[V0];
						int iV1 = CollectionLUT[V1];
						int iV2 = CollectionLUT[V2];

						IdxBuffer[TriIdx * 3 + 0] = iV0;
						IdxBuffer[TriIdx * 3 + 1] = iV1;
						IdxBuffer[TriIdx * 3 + 2] = iV2;

						memcpy(&VtxBuffer[iV0 * sizeof(TVertex)], V0, sizeof(TVertex));
						memcpy(&VtxBuffer[iV1 * sizeof(TVertex)], V1, sizeof(TVertex));
						memcpy(&VtxBuffer[iV2 * sizeof(TVertex)], V2, sizeof(TVertex));

						++TriIdx;
					}
				}

				return
				{
					std::move(VtxBuffer),std::move(IdxBuffer),GetLayoutFromVertexType<TVertex>()
				};
			};
	
	private:

		FVertexBuffer VertexBuffer;
		FTriangleBuffer TriangleBuffer;
		FMeshBuffer MeshBuffer;
		FSceneBuffer SceneBuffer;

		FTriangleCollectionBuffer TriangleCollectionBuffer;
		FTriangleCollectionProfilingResultsBuffer TriangleCollectionProfilingResultsBuffer;
		FDebugTriangleCollectionBuffer DebugTriangleCollectionBuffer;

		std::map<int, std::set<int>> TriangleCollections;

	private:

		Wrap::GlobalSettings Meta;

		FProfilingResult Result;
		FBakeParam BakeParam; 

		FTriCollectorParam Param;

		int NumRegionInside = 0;
		std::shared_ptr<XStaticMesh> DebugMesh;
	private:

		virtual void ParseCmdline(int argc, char* argv[])
		{
			argparse::ArgumentParser program("mesh partitioner", "1.0.0");

			//optional args: 
			program.add_argument("--config_path")
				.required()
				.help("bake param path .");

			program.parse_args(argc, argv);

			BakeParamPath = program.get<std::string>("--config_path");
		};

		std::string BakeParamPath;
};

#ifdef PY_MODULE_NAME
#include "PyRegister.h"
PY_REGISTER(MeshPartition,

	.def(py::init<>())

	.def("start_up", &MeshPartitionApp::StartUp)
	.def("run", &MeshPartitionApp::Run)
	.def("shutdown", &MeshPartitionApp::Shutdown)
	, MeshPartitionApp
)
PYBIND11_MODULE(PY_MODULE_NAME, m) 
{
    m.doc() =
        R"(
            xpydec - xverse model decimator python library .    
        )";

    PyRegister::Get().Exec(m);
}
#else
#include <EntryPoint.h>
IMPL_APP(MeshPartitionApp)
#endif

template<>
flora::BufferLayout MeshPartitionApp::GetLayoutFromVertexType<FVertex>()
{
	return 
	{
		{ flora::BufferElementDataType::Float4,"aPosition" },
		{ flora::BufferElementDataType::Float4,"aNormal" },
		{ flora::BufferElementDataType::Float4,"aUV" },
	};
}
