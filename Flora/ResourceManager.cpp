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
}
