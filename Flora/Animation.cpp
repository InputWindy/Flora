#include "Animation.h"
#include "ResourceManager.h"
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#pragma comment(lib,"assimp-vc142-mtd.lib")

struct FAnimationEnabler :public FAnimation {};

FAnimation::FAnimation()
{
    Type = EResourceType::Animation;
}

void FAnimation::Reload()
{

}

Ref<FAnimation> FAnimation::Generate(const aiAnimation* anim)
{
    if (!anim)return nullptr;

	auto assimpToGlmVec3 = [](aiVector3D vec)->glm::vec3
	{
		return glm::vec3(vec.x, vec.y, vec.z);
	};
	auto assimpToGlmQuat = [](aiQuaternion quat)->glm::quat 
	{
		glm::quat q;
		q.x = quat.x;
		q.y = quat.y;
		q.z = quat.z;
		q.w = quat.w;

		return q;
	};

    Ref<FAnimation> Res = std::make_shared<FAnimationEnabler>();

    Res->AnimationName = anim->mName.C_Str();
	Res->Duration = anim->mDuration;
	Res->TicksPerSecond = anim->mTicksPerSecond;
	Res->BoneTransforms.clear();

	Res->CachePath = "/Cache/Animation/" + Res->AnimationName + ".fanimation";

	for (int Idx = 0; Idx < anim->mNumChannels; ++Idx)
	{
		aiNodeAnim* Channel = anim->mChannels[Idx];
		FBoneTransformTrack Track;
		for (int TrackIdx = 0; TrackIdx < Channel->mNumPositionKeys; ++TrackIdx)
		{
			Track.positionTimestamps.push_back(Channel->mPositionKeys[TrackIdx].mTime);
			Track.positions.push_back(assimpToGlmVec3(Channel->mPositionKeys[TrackIdx].mValue));
		}
		for (int TrackIdx = 0; TrackIdx < Channel->mNumRotationKeys; ++TrackIdx)
		{
			Track.rotationTimestamps.push_back(Channel->mRotationKeys[TrackIdx].mTime);
			Track.rotations.push_back(assimpToGlmQuat(Channel->mRotationKeys[TrackIdx].mValue));
		}
		for (int TrackIdx = 0; TrackIdx < Channel->mNumScalingKeys; ++TrackIdx)
		{
			Track.scaleTimestamps.push_back(Channel->mScalingKeys[TrackIdx].mTime);
			Track.scales.push_back(assimpToGlmVec3(Channel->mScalingKeys[TrackIdx].mValue));
		}
		Res->BoneTransforms[Channel->mNodeName.C_Str()] = Track;
	}

    return Res;
}

bool FAnimation::Parse(IN FJson& In)
{
	AnimationName = In["AnimationName"].asString();

	RootPath = In["RootPath"].asString();
	Directory = In["Directory"].asString();
	RelativePath = In["RelativePath"].asString();
	Extension = In["Extension"].asString();

	CachePath = "/Cache/Animation/" + AnimationName + ".fanimation";

	return true;
}

bool FAnimation::Serialize(OUT FJson& Out)
{
	Out["RootPath"] = RootPath;
	Out["Directory"] = Directory;
	Out["RelativePath"] = RelativePath;
	Out["Extension"] = Extension;

	Out["AnimationName"] = AnimationName;
	Out["Duration"] = Duration;
	Out["TicksPerSecond"] = TicksPerSecond;
	FJson bone_transforms = Out["BoneTransforms"];
	for (auto& Transform : BoneTransforms)
	{
		bone_transforms["BoneName"] = Transform.first;

		FJson positions = bone_transforms["Positions"];
		FJson rotations = bone_transforms["Rotations"];
		FJson scales	= bone_transforms["Scales"];

		FJson position_timestamps = bone_transforms["PositionTimestamps"];
		FJson rotation_timestamps = bone_transforms["RotationTimestamps"];
		FJson scale_timestamps	  = bone_transforms["ScaleTimestamps"];

		for (auto& Position : Transform.second.positions)
		{
			FJson pos;
			pos["x"] = Position.x;
			pos["y"] = Position.y;
			pos["z"] = Position.z;
			positions.append(pos);
		}
		for (auto& Rotation : Transform.second.rotations)
		{
			FJson rot;
			rot["x"] = Rotation.x;
			rot["y"] = Rotation.y;
			rot["z"] = Rotation.z;
			rot["w"] = Rotation.w;
			rotations.append(rot);
		}
		for (auto& Scale : Transform.second.scales)
		{
			FJson scl;
			scl["x"] = Scale.x;
			scl["y"] = Scale.y;
			scl["z"] = Scale.z;
			scales.append(scl);
		}

		for (auto& pos_timesptamp : Transform.second.positionTimestamps)
			position_timestamps.append(pos_timesptamp);
		for (auto& rot_timesptamp : Transform.second.rotationTimestamps)
			rotation_timestamps.append(rot_timesptamp);
		for (auto& scl_timesptamp : Transform.second.scaleTimestamps)
			scale_timestamps.append(scl_timesptamp);
	}
	return true;
}

void FAnimation::Register()
{
	FResourceManager& ResourceManager = FResourceManager::Get();
	if (!ResourceManager.FindObject<FAnimation>(AnimationName))
	{
		ResourceManager.Register<FAnimation>(shared_from_this());
	};
}
