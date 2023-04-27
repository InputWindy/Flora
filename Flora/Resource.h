#pragma once
#include "Core.h"
#include "Serialization.h"
enum class EResourceType
{
	None,
	Material,
	Texture,
	Mesh,
	Skeleton,
	Animation,
	Model
};
class FLORA_API IResource
{
public:
	virtual void Register() = 0;

	inline EResourceType GetResourceType()const { return Type; };

	inline std::string GetRootPath()	const { return RootPath; };
	inline std::string GetDirectory()	const { return Directory; };
	inline std::string GetRelativePath()const { return RelativePath; };
	inline std::string GetExtension()	const { return Extension; };
	inline std::string GetCachePath()	const { return CachePath; };
protected:
	EResourceType Type = EResourceType::None;

	std::string RootPath;
	std::string Directory;
	std::string RelativePath;
	std::string Extension;
	std::string CachePath;
};