#include "ResourceManager.h"
#include "ContentBrowser.h"
#include "Application.h"
#include "Console.h"
#include "AABB.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#pragma comment(lib,"assimp-vc142-mtd.lib")

//load vertex & indices for single mesh (inlucding bone id & bone weight)
void LoadMeshData(
	IN const aiScene* scene,		/*global model data*/
	IN const aiMesh* mesh			/*mesh to load*/,
	IN std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfo/*global bone array*/,

	OUT std::vector<FVertex>& verticesOutput, /*out vertex raw data*/
	OUT std::vector<uint32_t>& indicesOutput,/*out indices raw dara*/
	OUT FAABB& Bounding
)
{
	verticesOutput.clear();
	indicesOutput.clear();

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		FVertex vertex;
		if (mesh->HasPositions())
		{
			glm::vec3 vector;
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
		}

		if (mesh->HasNormals())
		{
			glm::vec3 vector;
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}

		if (mesh->HasTangentsAndBitangents())
		{
			glm::vec3 vector;
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}

		if (mesh->HasTextureCoords(0))
		{
			glm::vec3 vector;
			vector.x = mesh->mTextureCoords[0][i].x;
			vector.y = mesh->mTextureCoords[0][i].y;
			vector.z = mesh->mTextureCoords[0][i].z;
			vertex.UV0 = vector;
		}
		if (mesh->HasTextureCoords(1))
		{
			glm::vec3 vector;
			vector.x = mesh->mTextureCoords[1][i].x;
			vector.y = mesh->mTextureCoords[1][i].y;
			vector.z = mesh->mTextureCoords[1][i].z;
			vertex.UV1 = vector;
		}
		if (mesh->HasTextureCoords(2))
		{
			glm::vec3 vector;
			vector.x = mesh->mTextureCoords[2][i].x;
			vector.y = mesh->mTextureCoords[2][i].y;
			vector.z = mesh->mTextureCoords[2][i].z;
			vertex.UV2 = vector;
		}
		if (mesh->HasTextureCoords(3))
		{
			glm::vec3 vector;
			vector.x = mesh->mTextureCoords[3][i].x;
			vector.y = mesh->mTextureCoords[3][i].y;
			vector.z = mesh->mTextureCoords[3][i].z;
			vertex.UV3 = vector;
		}

		vertex.BoneIds = glm::ivec4(0);
		vertex.BoneWeights = glm::vec4(0.0f);

		verticesOutput.push_back(vertex);
		Bounding.AddPoint(vertex.Position);
	}

	std::vector<unsigned int> boneCounts(verticesOutput.size(), 0);

	for (unsigned int boneIdx = 0; boneIdx < mesh->mNumBones; boneIdx++)
	{
		aiBone* bone = mesh->mBones[boneIdx];

		for (int j = 0; j < bone->mNumWeights; j++) {
			unsigned int id = bone->mWeights[j].mVertexId;
			float weight = bone->mWeights[j].mWeight;
			boneCounts[id]++;
			switch (boneCounts[id]) {
			case 1:
				verticesOutput[id].BoneIds.x = boneInfo[bone->mName.C_Str()].first;
				verticesOutput[id].BoneWeights.x = weight;
				break;
			case 2:
				verticesOutput[id].BoneIds.y = boneInfo[bone->mName.C_Str()].first;
				verticesOutput[id].BoneWeights.y = weight;
				break;
			case 3:
				verticesOutput[id].BoneIds.z = boneInfo[bone->mName.C_Str()].first;
				verticesOutput[id].BoneWeights.z = weight;
				break;
			case 4:
				verticesOutput[id].BoneIds.w = boneInfo[bone->mName.C_Str()].first;
				verticesOutput[id].BoneWeights.w = weight;
				break;
			default:
				break;

			}
		}
	}

	//normalize weights to make all weights sum 1
	for (int i = 0; i < verticesOutput.size(); i++) {
		glm::vec4& boneWeights = verticesOutput[i].BoneWeights;
		float totalWeight = boneWeights.x + boneWeights.y + boneWeights.z + boneWeights.w;
		if (totalWeight > 0.0f) {
			verticesOutput[i].BoneWeights = glm::vec4(
				boneWeights.x / totalWeight,
				boneWeights.y / totalWeight,
				boneWeights.z / totalWeight,
				boneWeights.w / totalWeight
			);
		}
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indicesOutput.push_back(face.mIndices[j]);
	}

}

template<>
Ref<FTexture> FLORA_API FResourceManager::Register(Ref<FTexture> Texture)
{
	Textures[Texture->GetName()] = Texture;
	return Texture;
}
template<>
Ref<FMaterial> FLORA_API FResourceManager::Register(Ref<FMaterial> Material)
{
	Materials[Material->GetName()] = Material;
	return Material;
}
template<>
Ref<FAnimation> FLORA_API FResourceManager::Register(Ref<FAnimation> Animation)
{
	Animations[Animation->GetName()] = Animation;
	return Animation;
}
template<>
Ref<FSkeleton> FLORA_API FResourceManager::Register(Ref<FSkeleton> Skeleton)
{
	Skeletons[Skeleton->GetName()] = Skeleton;
	return Skeleton;
}

template<>
Ref<FMesh> FLORA_API FResourceManager::Register(Ref<FMesh> Mesh)
{
	Meshes[Mesh->GetName()] = Mesh;
	return Mesh;
}

template<>
Ref<FTexture> FLORA_API FResourceManager::FindObject(const string& hash)
{
	auto find = Textures.find(hash);
	if (find != Textures.end())
	{
		return find->second->shared_from_this();
	}
	return nullptr;
};

template<>
Ref<FMaterial> FLORA_API FResourceManager::FindObject(const string& hash)
{
	auto find = Materials.find(hash);
	if (find != Materials.end())
	{
		return find->second->shared_from_this();
	}
	return nullptr;
};
template<>
Ref<FAnimation> FLORA_API FResourceManager::FindObject(const string& hash)
{
	auto find = Animations.find(hash);
	if (find != Animations.end())
	{
		return find->second->shared_from_this();
	}
	return nullptr;
};
template<>
Ref<FSkeleton> FLORA_API FResourceManager::FindObject(const string& hash)
{
	auto find = Skeletons.find(hash);
	if (find != Skeletons.end())
	{
		return find->second->shared_from_this();
	}
	return nullptr;
};

template<>
Ref<FMesh> FLORA_API FResourceManager::FindObject(const string& hash)
{
	auto find = Meshes.find(hash);
	if (find != Meshes.end())
	{
		return find->second->shared_from_this();
	}
	return nullptr;
};

template<>
Ref<FTexture> FLORA_API FResourceManager::RemoveObject(const string& hash)
{
	Ref<FTexture> Res = nullptr;
	auto find = Textures.find(hash);
	if (find != Textures.end())
	{
		Res = find->second->shared_from_this();
		Textures.erase(find);
	}
	return Res;
}
template<>
Ref<FTexture> FLORA_API FResourceManager::LoadObject(const string& FilePath)
{
	Ref<FTexture> Tex = nullptr;
	std::filesystem::path path(FilePath);
	string extension = path.extension().generic_string();
	if (extension == ".ftexture")
	{
		Tex = FApplication::GetRHI()->GenerateTexture(path.stem().generic_string().c_str(), 100, 100, 0, ETextureTarget_2D, EInternalFormat_RGBA32F);
		Tex->LoadFromFile(FilePath);
		Tex->Register();
	}
	else if 
	(
		extension == ".bmp" or extension == ".BMP" or
		extension == ".jpg" or extension == ".JPG" or
		extension == ".png" or extension == ".PNG" or
		extension == ".tif" or extension == ".TIF" or
		extension == ".tga" or extension == ".TGA" or
		extension == ".jpeg" or extension == ".JPEG" or
		extension == ".hdr" or extension == ".HDR" or
		extension == ".exr" or extension == ".EXR"
	)
	{
		FContentBrowserContext& Content = FContentBrowserContext::Get();
		string relative_path = FilePath.substr(Content.RootPath.size(), FilePath.size() - 1);

		Tex = FApplication::GetRHI()->GenerateTexture(path.stem().generic_string().c_str(), 100, 100, 0, ETextureTarget_2D, EInternalFormat_RGBA32F);
		Tex->SetImageData(std::make_shared<FImage>(Content.RootPath, relative_path));
		Tex->Register();
	}
	return Tex;
}
;

template<>
Ref<FMaterial> FLORA_API FResourceManager::LoadObject(const string& FilePath)
{
	Ref<FMaterial> Material = nullptr;
	std::filesystem::path path(FilePath);
	string extension = path.extension().generic_string();
	if (extension == ".fmaterial")
	{
		Material = FApplication::GetRHI()->GenerateMaterial(path.stem().generic_string().c_str());
		Material->LoadFromFile(FilePath);
		Material->Register();
	}
	else if (extension == ".glsl" or extension == ".hlsl")
	{
		PUSH_ERROR_MSG("Load Material","Unimpl glsl and hlsl loader");
	}
	return Material;
}
;

template<>
Ref<FSkeleton> FLORA_API FResourceManager::LoadObject(const string& FilePath)
{
	Ref<FSkeleton> Skeleton = nullptr;
	std::filesystem::path path(FilePath);
	string extension = path.extension().generic_string();
	if (extension == ".fskeleton")
	{
		Skeleton = make_shared<FSkeleton>(path.stem().generic_string().c_str());
		Skeleton->LoadFromFile(FilePath);
		Skeleton->Register();
	}
	else if
	(
		extension == ".obj" or extension == ".OBJ" or
		extension == ".dae" or extension == ".DAE" or
		extension == ".3ds" or extension == ".3DS" or
		extension == ".fbx" or extension == ".FBX" or
		extension == ".stl" or extension == ".STL" or
		extension == ".ase" or extension == ".ASE" or
		extension == ".blend" or extension == ".BLEND" or
		extension == ".mdl" or extension == ".MDL" or
		extension == ".glb" or extension == ".GLB" or
		extension == ".pmx" or extension == ".PMX" or
		extension == ".gltf" or extension == ".GLTF" 
	)
	{
		//PUSH_ERROR_MSG("Load Skeleton", "Unimpl Skeleton loader");
		Assimp::Importer Importer;
		Importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY,0.01f);

		const aiScene* Scene = Importer.ReadFile(
			FilePath.c_str(),
			aiProcess_Triangulate | 
			aiProcess_LimitBoneWeights |
			aiProcess_GenSmoothNormals | 
			aiProcess_GenUVCoords |
			aiProcess_CalcTangentSpace |
			aiProcess_ValidateDataStructure
			);
		Skeleton = make_shared<FSkeleton>(path.stem().generic_string().c_str());
		Skeleton->SetData(Scene);
		Skeleton->Register();
	}
	return Skeleton;
}
;
extern FJson ReadJsonFile(const std::string& filename);
template<>
Ref<FMesh> FLORA_API FResourceManager::LoadObject(const string& FilePath)
{
	Ref<FMesh> Mesh = nullptr;
	std::filesystem::path path(FilePath);
	string extension = path.extension().generic_string();
	if (extension == ".fmesh")
	{
		FJson out;
		out = ReadJsonFile(FilePath);
		std::string MeshName = out["MeshName"].asString();
		std::string ModelResourcePath = out["ModelResourcePath"].asString();
		std::string ModelMeshName = out["ModelMeshName"].asString();
		Mesh = FResourceManager::Get().FindObject<FMesh>(MeshName);
		if (!Mesh)
		{
			Mesh = FResourceManager::Get().FindObject<FMesh>(ModelMeshName);
			if (Mesh)
			{
				Mesh->Rename(MeshName);
				Mesh->Register();
			}
			else
			{
				//PUSH_ERROR_MSG("Load Mesh", "Unimpl Mesh loader");
				Assimp::Importer Importer;
				Importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 0.01f);

				const aiScene* Scene = Importer.ReadFile(
					FContentBrowserContext::Get().RootPath + ModelResourcePath,
					aiProcess_Triangulate |
					aiProcess_LimitBoneWeights |
					aiProcess_GenSmoothNormals |
					aiProcess_GenUVCoords |
					aiProcess_CalcTangentSpace |
					aiProcess_ValidateDataStructure
				);

				Ref<FSkeleton> Skeleton = make_shared<FSkeleton>(path.stem().generic_string().c_str());
				Skeleton->SetData(Scene);

				std::filesystem::path model_path(ModelResourcePath);

				for (uint32_t MeshIdx = 0; MeshIdx < Scene->mNumMeshes; MeshIdx++)
				{
					aiMesh* MeshData = Scene->mMeshes[MeshIdx];

					string name = model_path.stem().generic_string().c_str();
					name += '_';

					if (!FResourceManager::Get().FindObject<FMesh>(name += to_string(MeshIdx)))
					{
						std::vector<FVertex> verticesOutput;
						std::vector<uint32_t>indicesOutput;

						FAABB OBB;
						LoadMeshData(Scene, MeshData, Skeleton->GetBoneInfoTable(), verticesOutput, indicesOutput, OBB);

						Ref<FMesh> LocalMesh = FApplication::GetRHI()->GenerateMesh(name.c_str());
						LocalMesh->SetData(verticesOutput, indicesOutput, OBB);
						LocalMesh->ModelMeshName = name;
						LocalMesh->ModelResourcePath = ModelResourcePath;
						LocalMesh->Register();
					}
				}
			}
		}
	}
	else if
	(
		extension == ".obj" or extension == ".OBJ" or
		extension == ".dae" or extension == ".DAE" or
		extension == ".3ds" or extension == ".3DS" or
		extension == ".fbx" or extension == ".FBX" or
		extension == ".stl" or extension == ".STL" or
		extension == ".ase" or extension == ".ASE" or
		extension == ".blend" or extension == ".BLEND" or
		extension == ".mdl" or extension == ".MDL" or
		extension == ".glb" or extension == ".GLB" or
		extension == ".pmx" or extension == ".PMX" or
		extension == ".gltf" or extension == ".GLTF"
	)
	{
		//PUSH_ERROR_MSG("Load Mesh", "Unimpl Mesh loader");
		Assimp::Importer Importer;
		Importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 0.01f);

		const aiScene* Scene = Importer.ReadFile(
			FilePath.c_str(),
			aiProcess_Triangulate |
			aiProcess_LimitBoneWeights |
			aiProcess_GenSmoothNormals |
			aiProcess_GenUVCoords |
			aiProcess_CalcTangentSpace |
			aiProcess_ValidateDataStructure
		);

		Ref<FSkeleton> Skeleton = make_shared<FSkeleton>(path.stem().generic_string().c_str());
		Skeleton->SetData(Scene);

		string MeshName = path.stem().generic_string().c_str();
		MeshName += '_';

		for (uint32_t MeshIdx = 0; MeshIdx < Scene->mNumMeshes; MeshIdx++)
		{
			aiMesh* MeshData = Scene->mMeshes[MeshIdx];

			if (!FResourceManager::Get().FindObject<FMesh>(MeshName + to_string(MeshIdx)))
			{
				std::vector<FVertex> verticesOutput;
				std::vector<uint32_t>indicesOutput;

				FAABB OBB;
				LoadMeshData(Scene, MeshData, Skeleton->GetBoneInfoTable(), verticesOutput, indicesOutput, OBB);

				Mesh = FApplication::GetRHI()->GenerateMesh((MeshName + to_string(MeshIdx)).c_str());
				Mesh->SetData(verticesOutput, indicesOutput, OBB);
				Mesh->ModelMeshName = MeshName + to_string(MeshIdx);
				Mesh->ModelResourcePath = FilePath.substr(
					FContentBrowserContext::Get().RootPath.size(), FilePath.size() - 1);
				Mesh->Register();
			}
		}
	}
	return Mesh;
}
;

template<>
Ref<FAnimation> FLORA_API FResourceManager::LoadObject(const string& FilePath)
{
	Ref<FAnimation> Animation = nullptr;
	std::filesystem::path path(FilePath);
	string extension = path.extension().generic_string();
	if (extension == ".fanimation")
	{
		Animation = make_shared<FAnimation>(path.stem().generic_string().c_str());
		Animation->LoadFromFile(FilePath);
		Animation->Register();
	}
	else if
	(
		extension == ".obj" or extension == ".OBJ" or
		extension == ".dae" or extension == ".DAE" or
		extension == ".3ds" or extension == ".3DS" or
		extension == ".fbx" or extension == ".FBX" or
		extension == ".stl" or extension == ".STL" or
		extension == ".ase" or extension == ".ASE" or
		extension == ".blend" or extension == ".BLEND" or
		extension == ".mdl" or extension == ".MDL" or
		extension == ".glb" or extension == ".GLB" or
		extension == ".pmx" or extension == ".PMX" or
		extension == ".gltf" or extension == ".GLTF"
	)
	{
		Assimp::Importer Importer;
		Importer.SetPropertyFloat(AI_CONFIG_GLOBAL_SCALE_FACTOR_KEY, 0.01f);

		const aiScene* Scene = Importer.ReadFile(
			FilePath.c_str(),
			aiProcess_Triangulate |
			aiProcess_LimitBoneWeights |
			aiProcess_GenSmoothNormals |
			aiProcess_GenUVCoords |
			aiProcess_CalcTangentSpace |
			aiProcess_ValidateDataStructure
		);
		if (Scene->HasAnimations())
		{
			for (size_t AnimIdx = 0; AnimIdx < Scene->mNumAnimations; ++AnimIdx)
			{
				Ref<FAnimation> LocalAnimation = make_shared<FAnimation>(path.stem().generic_string().c_str());
				LocalAnimation->SetData(Scene->mAnimations[AnimIdx]);
				LocalAnimation->Register();
			}
		}
	}
	return Animation;
}
;

template<>
Ref<FMaterial> FLORA_API FResourceManager::RemoveObject(const string& hash)
{
	Ref<FMaterial> Res = nullptr;
	auto find = Materials.find(hash);
	if (find != Materials.end())
	{
		Res = find->second->shared_from_this();
		Materials.erase(find);
	}
	return Res;
};
template<>
Ref<FAnimation> FLORA_API FResourceManager::RemoveObject(const string& hash)
{
	Ref<FAnimation> Res = nullptr;
	auto find = Animations.find(hash);
	if (find != Animations.end())
	{
		Res = find->second->shared_from_this();
		Animations.erase(find);
	}
	return Res;
};
template<>
Ref<FSkeleton> FLORA_API FResourceManager::RemoveObject(const string& hash)
{
	Ref<FSkeleton> Res = nullptr;
	auto find = Skeletons.find(hash);
	if (find != Skeletons.end())
	{
		Res = find->second->shared_from_this();
		Skeletons.erase(find);
	}
	return Res;
};

template<>
Ref<FMesh> FLORA_API FResourceManager::RemoveObject(const string& hash)
{
	Ref<FMesh> Res = nullptr;
	auto find = Meshes.find(hash);
	if (find != Meshes.end())
	{
		Res = find->second->shared_from_this();
		Meshes.erase(find);
	}
	return Res;
};

FResourceManager::~FResourceManager()
{
}

void FResourceManager::Save()
{
	string root_path = FContentBrowserContext::Get().RootPath;
	for (auto& Texture : Textures)
	{
		Texture.second->SaveToFile(root_path + Texture.second->GetCachePath());
	}
	for (auto& Material : Materials)
	{
		Material.second->SaveToFile(root_path + Material.second->GetCachePath());
	}
	for (auto& Animation : Animations)
	{
		Animation.second->SaveToFile(root_path + Animation.second->GetCachePath());
	}
	for (auto& Skeleton : Skeletons)
	{
		Skeleton.second->SaveToFile(root_path + Skeleton.second->GetCachePath());
	}
	for (auto& Mesh : Meshes)
	{
		Mesh.second->SaveToFile(root_path + Mesh.second->GetCachePath());
	}
}
