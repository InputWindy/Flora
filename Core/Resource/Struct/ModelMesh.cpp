#include <Common/Log.h>
#include <Resource/Struct/ModelMesh.h>
#include <Resource/ResourceManager.h>
#include <Geometry/StaticMesh.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Scene/Struct/Transform.h>
#include <iostream>

using namespace std;
flora::XModelMesh::XModelMesh(const std::string& InFilePath,const flora::Wrap::GlobalSettings InSettings, std::vector<std::shared_ptr<XStaticMesh>>InMeshes, std::vector<std::weak_ptr<MeshNode>> InMeshNodes, std::shared_ptr<XModelMesh::MeshNode> InNode)
	:BaseType(InFilePath),IOSettings(InSettings),Meshes(InMeshes),Root(InNode),MeshNodes(InMeshNodes)
{
	
}


void flora::XModelMesh::UpdateFaceFaceAdj()
{
	for (size_t Idx = 0; Idx < Meshes.size(); Idx++)
	{
		Meshes[Idx]->face.EnableFFAdjacency();
		vcg::tri::UpdateTopology<XStaticMesh>::FaceFace(*Meshes[Idx]);
	}
}


void flora::XModelMesh::UpdateStaticMeshes()
{
	for (size_t Idx = 0; Idx < Meshes.size(); Idx++)
	{
		Meshes[Idx]->Owner = DynamicPointerCast<XModelMesh>();

		Meshes[Idx]->RemoveDuplicate();
		Meshes[Idx]->RemoveDegenerate();
		Meshes[Idx]->RemoveUnreferencedVertex();

		Meshes[Idx]->RegenerateFaceNormal();
		Meshes[Idx]->RegenerateVertexNormalFromFace();
		Meshes[Idx]->CompactEveryVector();

		Meshes[Idx]->UpdateBouding();
	}

	UpdateStaticMeshesOnlyNode();
}

void flora::XModelMesh::UpdateStaticMeshesOnlyNode()
{
	for (size_t Idx = 0; Idx < MeshNodes.size(); Idx++)
	{
		for (auto M : MeshNodes[Idx].lock()->NodeMeshes)
		{
			M->SetNodeName(MeshNodes[Idx].lock()->Name);
		};
	};
}



#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <memory>
#include <iostream>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/code/AssetLib/FBX/FBXExporter.h>
#include <assimp/Exporter.hpp>


#include <cstdint>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream> 
#include <fstream> 
#include <memory>
#include <string> 
#include <sstream>
#include <filesystem>

#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/clean.h>
#include <vcglib/wrap/io_trimesh/io_mask.h>

static void ProcessMesh(const aiMesh* InMesh, std::shared_ptr<flora::XStaticMesh> InoutMesh)
{
	InoutMesh->Enable(vcg::tri::io::Mask::IOM_VERTCOORD | vcg::tri::io::Mask::IOM_VERTNORMAL | vcg::tri::io::Mask::IOM_VERTTEXCOORD | vcg::tri::io::Mask::IOM_VERTCOLOR);

	auto VertexIter = vcg::tri::Allocator<flora::XStaticMesh>::AddVertices(*InoutMesh, InMesh->mNumVertices);
	for (size_t Idx = 0; Idx < InMesh->mNumVertices; Idx++)
	{
		if (InMesh->HasPositions() && VertexIter->IsCoordEnabled())
		{
			VertexIter->P() = { InMesh->mVertices[Idx].x, InMesh->mVertices[Idx].y, InMesh->mVertices[Idx].z };
		}

		if (InMesh->HasNormals() && VertexIter->IsNormalEnabled())
		{
			VertexIter->N() = { InMesh->mNormals[Idx].x, InMesh->mNormals[Idx].y, InMesh->mNormals[Idx].z };
		}

		if (InMesh->HasTextureCoords(0) && VertexIter->IsTexCoordEnabled())
		{
			VertexIter->T() = { InMesh->mTextureCoords[0][Idx].x, InMesh->mTextureCoords[0][Idx].y };
		}

		if (InMesh->HasVertexColors(0) && VertexIter->IsColorEnabled())
		{
			vcg::Color4b c = (InMesh->mColors[0][Idx].r, InMesh->mColors[0][Idx].g, InMesh->mColors[0][Idx].b, InMesh->mColors[0][Idx].a);
			VertexIter->C() = c;
		}

		++VertexIter;
	}

	for (size_t Idx = 0; Idx < InMesh->mNumFaces; Idx++)
	{
		//Face Must Be Triangle
		assert(InMesh->mFaces[Idx].mNumIndices == 3);

		const aiFace& Face = InMesh->mFaces[Idx];

		vcg::tri::Allocator<flora::XStaticMesh>::AddFace(*InoutMesh, Face.mIndices[0], Face.mIndices[1], Face.mIndices[2]);
	};
}

static int MeshCnt = 0;

static void ProcessNode(const aiScene* InScene, aiNode* InNode,
	std::shared_ptr<flora::XModelMesh::MeshNode> InoutTopology, const std::shared_ptr<flora::XModelMesh> InOwner, glm::mat4 InCoordMatrix,
	glm::mat4 InTransformToRoot = glm::identity<glm::mat4>(),
	std::shared_ptr<flora::XModelMesh::MeshNode> InParentNode = nullptr)
{
	//  ȡNode  Ϣ
	InoutTopology->Name = InNode->mName.C_Str();
	InoutTopology->Parent = InParentNode;
	InoutTopology->TransformToParent = glm::mat4(
		InNode->mTransformation[0][0], InNode->mTransformation[1][0], InNode->mTransformation[2][0], InNode->mTransformation[3][0],
		InNode->mTransformation[0][1], InNode->mTransformation[1][1], InNode->mTransformation[2][1], InNode->mTransformation[3][1],
		InNode->mTransformation[0][2], InNode->mTransformation[1][2], InNode->mTransformation[2][2], InNode->mTransformation[3][2],
		InNode->mTransformation[0][3], InNode->mTransformation[1][3], InNode->mTransformation[2][3], InNode->mTransformation[3][3]
	);

	InTransformToRoot = InTransformToRoot * InoutTopology->TransformToParent;

	aiMatrix4x4 transform_to_parent = InNode->mTransformation;
	aiNode* parent_node = InNode->mParent;
	while (parent_node)
	{
		transform_to_parent = parent_node->mTransformation * transform_to_parent;
		parent_node = parent_node->mParent;
	}

	InTransformToRoot = glm::mat4
	(
		transform_to_parent[0][0], transform_to_parent[1][0], transform_to_parent[2][0], transform_to_parent[3][0],
		transform_to_parent[0][1], transform_to_parent[1][1], transform_to_parent[2][1], transform_to_parent[3][1],
		transform_to_parent[0][2], transform_to_parent[1][2], transform_to_parent[2][2], transform_to_parent[3][2],
		transform_to_parent[0][3], transform_to_parent[1][3], transform_to_parent[2][3], transform_to_parent[3][3]
	);

	//   ص ǰNode µ     Mesh
	for (size_t Idx = 0; Idx < InNode->mNumMeshes; Idx++)
	{
		aiMesh* MeshItem = InScene->mMeshes[InNode->mMeshes[Idx]];

		std::shared_ptr<flora::XStaticMesh> InoutMesh =
			std::make_shared<flora::XStaticMesh>(InOwner, (std::string(MeshItem->mName.C_Str()) + "_" + std::to_string(MeshCnt)).c_str(), InCoordMatrix, InTransformToRoot);
		
		++MeshCnt;
		ProcessMesh(MeshItem, InoutMesh);

		InoutTopology->NodeMeshes.push_back(InoutMesh);
	}

	// ݹ     ӽڵ 
	for (size_t Idx = 0; Idx < InNode->mNumChildren; Idx++)
	{
		//push_back
		std::shared_ptr<flora::XModelMesh::MeshNode> Child = std::make_shared<flora::XModelMesh::MeshNode>();

		InoutTopology->Children.push_back(Child);

		ProcessNode(InScene, InNode->mChildren[Idx], Child, InOwner, InCoordMatrix, InTransformToRoot, InoutTopology);
	}
}

static void ProcessMetaData(const aiScene* InScene, flora::Wrap::GlobalSettings& InoutSettings, glm::mat4& InoutCoordMatrix)
{
	InScene->mMetaData->Get<int>("UpAxis", InoutSettings.UpAxis);
	InScene->mMetaData->Get<int>("UpAxisSign", InoutSettings.UpAxisSign);
	InScene->mMetaData->Get<int>("FrontAxis", InoutSettings.FrontAxis);
	InScene->mMetaData->Get<int>("FrontAxisSign", InoutSettings.FrontAxisSign);
	InScene->mMetaData->Get<int>("CoordAxis", InoutSettings.CoordAxis);
	InScene->mMetaData->Get<int>("CoordAxisSign", InoutSettings.CoordAxisSign);
	InScene->mMetaData->Get<int>("OriginalUpAxis", InoutSettings.OriginalUpAxis);
	InScene->mMetaData->Get<int>("OriginalUpAxisSign", InoutSettings.OriginalUpAxisSign);
	InScene->mMetaData->Get<double>("UnitScaleFactor", InoutSettings.UnitScaleFactor);
	InScene->mMetaData->Get<double>("OriginalUnitScaleFactor", InoutSettings.OriginalUnitScaleFactor);

	aiString s1, s2, s3;

	InScene->mMetaData->Get<aiString>("SourceAsset_FormatVersion", s1);
	InScene->mMetaData->Get<aiString>("SourceAsset_Generator", s2);
	InScene->mMetaData->Get<aiString>("SourceAsset_Format", s3);

	InoutSettings.SourceAssetFormatVersion = s1.C_Str();
	InoutSettings.SourceAssetGenerator = s2.C_Str();
	InoutSettings.SourceAssetFormat = s3.C_Str();

	FLORA_CORE_INFO("SourceAsset_FormatVersion:{0}", s1.C_Str());
	FLORA_CORE_INFO("SourceAsset_Generator:{0}", s2.C_Str());
	FLORA_CORE_INFO("SourceAsset_Format:{0}", s3.C_Str());

	//CoordSytem
	aiVector3D UpAxis;// = glm::identity<glm::vec3>();
	aiVector3D FrontAxis;// = glm::identity<glm::vec3>();
	aiVector3D RightAxis;// = glm::identity<glm::vec3>();

	switch (InoutSettings.UpAxis)
	{
	case 0:UpAxis = aiVector3D(InoutSettings.UpAxisSign, 0, 0); break;
	case 1:UpAxis = aiVector3D(0, InoutSettings.UpAxisSign, 0); break;
	case 2:UpAxis = aiVector3D(0, 0, InoutSettings.UpAxisSign); break;
	default:
		break;
	}

	switch (InoutSettings.FrontAxis)
	{
	case 0:FrontAxis = aiVector3D(InoutSettings.FrontAxisSign, 0, 0); break;
	case 1:FrontAxis = aiVector3D(0, InoutSettings.FrontAxisSign, 0); break;
	case 2:FrontAxis = aiVector3D(0, 0, InoutSettings.FrontAxisSign); break;
	default:
		break;
	}

	switch (InoutSettings.CoordAxis)
	{
	case 0:RightAxis = aiVector3D(InoutSettings.CoordAxisSign, 0, 0); break;
	case 1:RightAxis = aiVector3D(0, InoutSettings.CoordAxisSign, 0); break;
	case 2:RightAxis = aiVector3D(0, 0, InoutSettings.CoordAxisSign); break;
	default:
		break;
	}

	InoutCoordMatrix = glm::mat4
	{
		RightAxis.x,RightAxis.y,RightAxis.z,0,
		UpAxis.x,UpAxis.y,UpAxis.z,0,
		FrontAxis.x,FrontAxis.y,FrontAxis.z,0,
		0,0,0,1
	};

	//assimp to ue coord
	glm::mat4 m
	{
		1,0,0,0,
		0,0,1,0,
		0,-1,0,0,
		0,0,0,1
	};

	InoutCoordMatrix = m * InoutCoordMatrix;
}

template<>
std::shared_ptr<flora::XModelMesh::BaseType> flora::XImporter<flora::AssimpImportSettings, flora::XModelMesh::BaseType>::Exec(const std::string& Filepath, const flora::AssimpImportSettings& Settings)
{
	MeshCnt = 0;

	std::shared_ptr<flora::XModelMesh> Ret = nullptr;

	unsigned int PostprocessFlags = 
		aiProcess_Triangulate 
		/*| aiProcess_PreTransformVertices*/ 
		| aiProcess_ConvertToLeftHanded 
		| aiProcess_GenSmoothNormals;
	// | aiProcessPreset_TargetRealtime_MaxQuality;// | aiProcess_ConvertToLeftHanded;// | aiProcessPreset_TargetRealtime_Quality;

	if (Settings.bPreTransformVertices)PostprocessFlags |= aiProcess_PreTransformVertices;
	if (Settings.bJoinIdenticalVertices)PostprocessFlags |= aiProcess_JoinIdenticalVertices;
	if (Settings.bOptimizeMeshes)PostprocessFlags |= aiProcess_OptimizeMeshes;
	if (Settings.bRemoveRedundantMaterials)PostprocessFlags |= aiProcess_RemoveRedundantMaterials;
	if (Settings.bSortByPType)PostprocessFlags |= aiProcess_SortByPType;

	FLORA_CORE_TRACE("Open model : {0}", Filepath);

	Assimp::Importer AssImporter;
	if (const aiScene* Scene = AssImporter.ReadFile(Filepath, PostprocessFlags))
	{
		//      ͬ    ֮      ϵ  ͬ ľ   
		glm::mat4 CoordMatrix = glm::identity<glm::mat4>();

		//    MetaData  ģ   ļ  汾 ţ     ռ䣬  λ   ȣ 
		flora::Wrap::GlobalSettings IOSettings;
		ProcessMetaData(Scene, IOSettings, CoordMatrix);

		//           ݣ ģ   ļ  ڵ   Ϣ   Լ    ӽڵ ı任    
		std::shared_ptr<XModelMesh::MeshNode> Topology = std::make_shared<XModelMesh::MeshNode>();
		ProcessNode(Scene, Scene->mRootNode, Topology, Ret, CoordMatrix);

		//             ݣ   ȡ         飬      Ⱦ  
		std::vector<std::shared_ptr<XStaticMesh>> Meshes;
		std::vector<std::weak_ptr<flora::XModelMesh::MeshNode>> MeshNodes;
		Topology->GetMeshes(Topology, Meshes, MeshNodes);

		// ½ һ  ModelMesh  Դ  StaticMesh Ѿ  Ƴ       Դ   ͣ 
		Ret = std::make_shared<flora::XModelMesh>(Filepath, IOSettings, Meshes, MeshNodes, Topology);

		Ret->UpdateStaticMeshes();
	}

	return Ret;
};

namespace fs = std::filesystem;
std::string GetFileExtensionWithoutDot(const fs::path& Path)
{
	std::string ext = Path.extension().generic_string();
	if (!ext.empty() && ext[0] == '.') {
		ext = ext.substr(1); // 去掉点
	}
	return ext;
}

template<>
bool flora::XExporter<flora::AssimpExportSettings, flora::XModelMesh>::Exec(flora::XModelMesh& Mesh, const std::string& Filepath, const flora::AssimpExportSettings& Settings)
{
	bool bRet = false;

	FLORA_CORE_TRACE("Save model:{0}", Filepath);

	std::filesystem::path Path(Filepath);
	//     ļ   
	if (std::filesystem::exists(Path.parent_path()) == false)flora::GlobalUtils::CreateDirectoryRecursive(Path.parent_path().generic_string());

	if (std::filesystem::exists(Path.parent_path()))
	{
		//        
		int ExportFlags = aiProcess_GenSmoothNormals | aiProcess_ConvertToLeftHanded/* | aiProcess_JoinIdenticalVertices*/;

		//    aiScene
		aiScene* Scene = new aiScene;

		//    Ҫ  һ  Material  Ҫ  Ȼ         Խ  
		Scene->mMaterials = new aiMaterial * [1];
		Scene->mMaterials[0] = new aiMaterial();
		Scene->mNumMaterials = 1;
		//          
		std::vector<aiMesh*> aiMeshes;
		for (size_t Idx = 0; Idx < Mesh.Meshes.size(); Idx++)
		{
			std::shared_ptr<XStaticMesh> StaticMesh = Mesh.Meshes[Idx];

			//   ⵼  û       ε       
			if (StaticMesh->FN() > 0)
			{
				//    aiMesh ṹ
				aiMesh* pMesh = new aiMesh;
				aiMeshes.push_back(pMesh);

				//              
				pMesh->mName.Set(StaticMesh->GetStaticMeshName());

				std::vector<aiFace> Faces;
				std::vector<aiVector3D> Positions;
				std::vector<aiVector3D> Normals;
				/*std::vector<aiVector3D> Tangent;
				std::vector<aiVector3D> Bitangent;*/
				std::vector<std::vector<aiVector3D>> UVs(1, std::vector<aiVector3D>());
				std::vector<std::vector<aiColor4D>> Colors(1, std::vector<aiColor4D>());
				{
					for (size_t VertexIdx = 0; VertexIdx < StaticMesh->VN(); VertexIdx++)
					{
						if (StaticMesh->HasDataMask(flora::XStaticMesh::MeshElement::MM_VERTCOORD))
						{
							const flora::XVertex& Vertex = StaticMesh->vert[VertexIdx];
							aiVector3D Position;
							Position.x = Vertex.P().X();
							Position.y = Vertex.P().Y();
							Position.z = Vertex.P().Z();
							Positions.push_back(Position);
						}

						if (StaticMesh->HasDataMask(flora::XStaticMesh::MeshElement::MM_VERTNORMAL))
						{
							const flora::XVertex& Vertex = StaticMesh->vert[VertexIdx];
							aiVector3D Normal;
							Normal.x = Vertex.N().X();
							Normal.y = Vertex.N().Y();
							Normal.z = Vertex.N().Z();
							Normals.push_back(Normal);
						}
						
						if (StaticMesh->HasDataMask(flora::XStaticMesh::MeshElement::MM_VERTTEXCOORD))
						{
							const flora::XVertex& Vertex = StaticMesh->vert[VertexIdx];
							aiVector3D UV;
							UV.x = Vertex.T().U();
							UV.y = Vertex.T().V();
							UV.z = 0;
							UVs[0].push_back(UV);
						}
					}

					//              
					for (int FaceIndex = 0; FaceIndex < StaticMesh->FN(); ++FaceIndex) 
					{
						const flora::XFace& VcgFace = StaticMesh->face[FaceIndex];

						aiFace Face;
						Face.mIndices = new unsigned int[3];
						Face.mNumIndices = 3;

						Face.mIndices[0] = VcgFace.V(0)->Index();
						Face.mIndices[1] = VcgFace.V(1)->Index();
						Face.mIndices[2] = VcgFace.V(2)->Index();
						Faces.push_back(Face);
					}
				}
				pMesh->mPrimitiveTypes = aiPrimitiveType_TRIANGLE;
				pMesh->mNumVertices = StaticMesh->VN();
				pMesh->mNumFaces = StaticMesh->FN();

				pMesh->mVertices = new aiVector3D[pMesh->mNumVertices];
				pMesh->mNormals = new aiVector3D[pMesh->mNumVertices];
				pMesh->mFaces = new aiFace[pMesh->mNumFaces];

				std::copy(Faces.begin(), Faces.end(), pMesh->mFaces);
				std::copy(Positions.begin(), Positions.end(), pMesh->mVertices);
				std::copy(Normals.begin(), Normals.end(), pMesh->mNormals);

				if (Settings.bExportUV0 && UVs.size() > 0)
				{
					pMesh->mTextureCoords[0] = new aiVector3D[pMesh->mNumVertices];
					std::copy(UVs[0].begin(), UVs[0].end(), pMesh->mTextureCoords[0]);
				}

				/*std::copy(Tangent.begin(), Tangent.end(), pMesh->mTangents);
				std::copy(Bitangent.begin(), Bitangent.end(), pMesh->mBitangents);
				
				if (UVs.size() > 1) std::copy(UVs[1].begin(), UVs[1].end(), pMesh->mTextureCoords[1]);
				if (UVs.size() > 2) std::copy(UVs[2].begin(), UVs[2].end(), pMesh->mTextureCoords[2]);
				if (UVs.size() > 3) std::copy(UVs[3].begin(), UVs[3].end(), pMesh->mTextureCoords[3]);
				if (Colors.size() > 0) std::copy(Colors[0].begin(), Colors[0].end(), pMesh->mColors[0]);
				if (Colors.size() > 1) std::copy(Colors[1].begin(), Colors[1].end(), pMesh->mColors[1]);
				if (Colors.size() > 2) std::copy(Colors[2].begin(), Colors[2].end(), pMesh->mColors[2]);
				if (Colors.size() > 3) std::copy(Colors[3].begin(), Colors[3].end(), pMesh->mColors[3]);*/
			}
		}
		Scene->mNumMeshes = aiMeshes.size();
		Scene->mMeshes = new aiMesh * [Scene->mNumMeshes];
		std::copy(aiMeshes.begin(), aiMeshes.end(), Scene->mMeshes);
		//     ڵ ṹ
		std::function<aiNode* (decltype(Mesh.Root), aiNode*, const aiScene*)> ConstructNodeGraph = [&ConstructNodeGraph](auto MeshNode, aiNode* Parent, const aiScene* Scene)->aiNode*
			{
				if (MeshNode == nullptr)return nullptr;

				aiNode* NewNode = new aiNode;
				{
					NewNode->mParent = Parent;
					NewNode->mName = MeshNode->Name;
					NewNode->mTransformation =
					{
						MeshNode->TransformToParent[0][0],MeshNode->TransformToParent[1][0],MeshNode->TransformToParent[2][0],MeshNode->TransformToParent[3][0],
						MeshNode->TransformToParent[0][1],MeshNode->TransformToParent[1][1],MeshNode->TransformToParent[2][1],MeshNode->TransformToParent[3][1],
						MeshNode->TransformToParent[0][2],MeshNode->TransformToParent[1][2],MeshNode->TransformToParent[2][2],MeshNode->TransformToParent[3][2],
						MeshNode->TransformToParent[0][3],MeshNode->TransformToParent[1][3],MeshNode->TransformToParent[2][3],MeshNode->TransformToParent[3][3],
					};

					// ռ MeshNode             
					std::vector<unsigned int> Meshes;
					for (size_t NodeMeshIdx = 0; NodeMeshIdx < MeshNode->NodeMeshes.size(); NodeMeshIdx++)
					{
						//  ȫ  Mesh   ұ    ҵ ͬ          
						for (size_t SceneMeshIdx = 0; SceneMeshIdx < Scene->mNumMeshes; SceneMeshIdx++)
						{
							if (std::string(Scene->mMeshes[SceneMeshIdx]->mName.C_Str()) == MeshNode->NodeMeshes[NodeMeshIdx]->GetStaticMeshName())
							{
								Meshes.push_back(SceneMeshIdx);
							}
						}
					}
					NewNode->mNumMeshes = Meshes.size();
					NewNode->mMeshes = new unsigned int[Meshes.size()];
					std::copy(Meshes.begin(), Meshes.end(), NewNode->mMeshes);

					//     ӽڵ   Ϣ
					std::vector<aiNode*> aiNodes;
					for (size_t MeshNodeIdx = 0; MeshNodeIdx < MeshNode->Children.size(); MeshNodeIdx++)
					{
						aiNodes.push_back(ConstructNodeGraph(MeshNode->Children[MeshNodeIdx], NewNode, Scene));
					}
					NewNode->addChildren(aiNodes.size(), aiNodes.data());
				}
				return NewNode;
			};
		Scene->mRootNode = ConstructNodeGraph(Mesh.Root, nullptr, Scene);

		if (Scene->mRootNode == nullptr)
		{
			Scene->mRootNode = new aiNode();
		}

		//   õ   MetaData ļ ͷ
		getFbxMetaUpAxis() = Mesh.IOSettings.UpAxis;
		getFbxMetaUpAxisSign() = Mesh.IOSettings.UpAxisSign;
		getFbxMetaFrontAxis() = Mesh.IOSettings.FrontAxis;
		getFbxMetaFrontAxisSign() = Mesh.IOSettings.FrontAxisSign;
		getFbxMetaCoordAxis() = Mesh.IOSettings.CoordAxis;
		getFbxMetaCoordAxisSign() = Mesh.IOSettings.CoordAxisSign;
		getFbxMetaOriginalUpAxis() = Mesh.IOSettings.OriginalUpAxis;
		getFbxMetaOriginalUpAxisSign() = Mesh.IOSettings.OriginalUpAxisSign;
		getFbxMetaUnitScaleFactor() = Mesh.IOSettings.UnitScaleFactor;
		getFbxMetaOriginalUnitScaleFactor() = Mesh.IOSettings.OriginalUnitScaleFactor;

		Assimp::Exporter exp;
		bRet = exp.Export(Scene, GetFileExtensionWithoutDot(Path), Filepath, ExportFlags) == aiReturn_SUCCESS;

		if (!bRet)
		{
			FLORA_CORE_ERROR("Failed To Export Model {0}", Filepath);
		}
	}

	return bRet;
}
