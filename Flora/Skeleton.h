#pragma once
#include "Core.h"
#include "Resource.h"
#include "Serialization.h"
#include <memory>
#include <vector>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
class aiScene;

struct FLORA_API FBone:public ISerialization
{
	int Id = 0;
	std::string Name;
	glm::mat4   OffsetMatrix = glm::identity<glm::mat4>();
	glm::mat4   TransformMatrix = glm::identity<glm::mat4>();
	std::vector<FBone> Children;
public:
	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;
};

class FLORA_API FSkeleton :public std::enable_shared_from_this<FSkeleton>, public IResource, public ISerialization
{
public:
	FSkeleton(const char*);
	virtual ~FSkeleton() = default;

	inline std::string	 GetName()	  const { return SkeletonName; };
	inline const FBone&	 GetBoneTree()const { return RootBone; };
	inline uint32_t		 GetBoneNum() const { return BoneNum; };

	void GetBoneTransforms(std::vector<glm::mat4>& InOut);
public:
	virtual void SetData(const aiScene*);
public:
	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;

	virtual void Register() final;
	virtual void Rename(const string& name) final;
protected:
	std::string	SkeletonName;

	FBone		RootBone;
	uint32_t	BoneNum;
	glm::mat4	GlobalInverseMatrix = glm::identity<glm::mat4>();
};

