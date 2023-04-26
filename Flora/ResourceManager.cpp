#include "ResourceManager.h"
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