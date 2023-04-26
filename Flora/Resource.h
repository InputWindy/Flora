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

	inline EResourceType GetType()const { return Type; };
protected:
	EResourceType Type = EResourceType::None;
};