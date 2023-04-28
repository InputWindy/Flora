#include "ResourceManager.h"
#include "ContentBrowser.h"
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
};

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
