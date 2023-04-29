#pragma once
#include "Core.h"
#include "Texture.h"
#include "Material.h"
#include "Animation.h"
#include "Skeleton.h"
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

	template<typename T>
	Ref<T> RemoveObject(const string& Hash);

	template<typename T>
	Ref<T> LoadObject(const string& FilePath);

	void Save();
public:
	inline const auto& GetTextures()	const { return Textures; };
	inline const auto& GetMaterials()	const { return Materials; };
	inline const auto& GetAnimations()	const { return Animations; };
	inline const auto& GetSkeletons()	const { return Skeletons; };
private:
	std::unordered_map<std::string, Ref<FTexture>>   Textures;
	std::unordered_map<std::string, Ref<FMaterial>>  Materials;
	std::unordered_map<std::string, Ref<FAnimation>> Animations;
	std::unordered_map<std::string, Ref<FSkeleton>>  Skeletons;
};



