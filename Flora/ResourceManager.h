#pragma once
#include "Core.h"
#include "Texture.h"
#include "Material.h"
#include "Animation.h"
#include <unordered_map>
class FLORA_API FResourceManager
{
protected:
	FResourceManager() = default;
public:
	virtual ~FResourceManager();
	static inline FResourceManager& Get() { static FResourceManager msManager; return msManager; };

	template<typename T>
	Ref<T> Register(Ref<T> Resource);

	template<typename T>
	Ref<T> FindObject(const string& Hash);

	void Save();
public:
	inline const auto& GetTextures()	const { return Textures; };
	inline const auto& GetMaterials()	const { return Materials; };
	inline const auto& GetAnimations()	const { return Animations; };
private:
	std::unordered_map<std::string, Ref<FTexture>>   Textures;
	std::unordered_map<std::string, Ref<FMaterial>>  Materials;
	std::unordered_map<std::string, Ref<FAnimation>> Animations;
};

//template<>
//inline Ref<FTexture> FResourceManager::Register(Ref<FTexture> Texture)
//{
//	Textures[Texture->GetName()] = Texture;
//	return Texture;
//}
//template<>
//inline Ref<FTexture> FResourceManager::FindObject(const string& hash)
//{
//	auto find = Textures.find(hash);
//	if (find != Textures.end())
//	{
//		return find->second->shared_from_this();
//	}
//	return nullptr;
//}



