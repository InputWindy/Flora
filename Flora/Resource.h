#pragma once
#include "Core.h"
#include "Serialization.h"
#include <string>
using namespace std;
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
	virtual void Rename(const string& name) = 0;

	inline EResourceType GetResourceType()const { return Type; };
	inline std::string	 GetCachePath()	  const { return CachePath; };
protected:
	EResourceType Type = EResourceType::None;

	std::string CachePath;
};