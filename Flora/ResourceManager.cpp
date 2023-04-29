#include "ResourceManager.h"
#include "ContentBrowser.h"
#include "Application.h"
#include "Console.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#pragma comment(lib,"assimp-vc142-mtd.lib")
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
		PUSH_ERROR_MSG("Load Animation", "Unimpl Animation loader");
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
}
