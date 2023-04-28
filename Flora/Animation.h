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

class aiAnimation;

struct FLORA_API FBoneTransformTrack
{
	std::vector<float> positionTimestamps;
	std::vector<float> rotationTimestamps;
	std::vector<float> scaleTimestamps;

	std::vector<glm::vec3> positions;
	std::vector<glm::quat> rotations;
	std::vector<glm::vec3> scales;
};

class FLORA_API FAnimation :public std::enable_shared_from_this<FAnimation>, public IResource,public ISerialization
{
protected:
	FAnimation();
	void Reload();
public:
	virtual  ~FAnimation() = default;
	static Ref<FAnimation> Generate();
	static Ref<FAnimation> Generate(const char* name, const aiAnimation* anim);

	inline std::string GetName()const { return AnimationName; };
	inline float       GetDuration()const { return Duration; };
	inline float       GetTicksPerSecond()const { return TicksPerSecond; };
	inline const auto& GetBoneTransforms() const { return BoneTransforms; };
public:
	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;

	virtual void Register() final;
protected:
	std::string AnimationName;
	float       Duration		= 0.0f;/*seconds*/
	float       TicksPerSecond	= 1.0f;												
	std::unordered_map<std::string, FBoneTransformTrack> BoneTransforms;
private:
	static inline uint32_t DefaultAnimationId = 0;
};

