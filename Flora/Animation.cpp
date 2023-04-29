#include "Animation.h"
#include "ResourceManager.h"
#include "Console.h"
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#pragma comment(lib,"assimp-vc142-mtd.lib")

bool FAnimation::Parse(IN FJson& In)
{
	AnimationName = In["AnimationName"].asString();

	CachePath = "/Cache/Animation/" + AnimationName + ".fanimation";

	Duration = In["Duration"].asFloat();
	TicksPerSecond = In["TicksPerSecond"].asFloat();
	FJson& bone_transforms = In["BoneTransforms"];
	for (auto& transform : bone_transforms)
	{
		string BoneName = transform["BoneName"].asString();
		FBoneTransformTrack Transform;

		FJson& positions = transform["Positions"];
		FJson& rotations = transform["Rotations"];
		FJson& scales = transform["Scales"];

		FJson& position_timestamps = transform["PositionTimestamps"];
		FJson& rotation_timestamps = transform["RotationTimestamps"];
		FJson& scale_timestamps = transform["ScaleTimestamps"];

		for (auto& position : positions)
		{
			Transform.positions.push_back({ 
				position["x"].asFloat() ,
				position["y"].asFloat() ,
				position["z"].asFloat() });
		}
		for (auto& rotation : rotations)
		{
			Transform.rotations.push_back({
				rotation["x"].asFloat() ,
				rotation["y"].asFloat() ,
				rotation["z"].asFloat() ,
				rotation["w"].asFloat() });
		}
		for (auto& scale : scales)
		{
			Transform.scales.push_back({
				scale["x"].asFloat() ,
				scale["y"].asFloat() ,
				scale["z"].asFloat() });
		}

		for (auto& pos_timesptamp : position_timestamps)
			Transform.positionTimestamps.push_back(pos_timesptamp.asFloat());
		for (auto& rot_timesptamp : rotation_timestamps)
			Transform.rotationTimestamps.push_back(rot_timesptamp.asFloat());
		for (auto& scl_timesptamp : scale_timestamps)
			Transform.scaleTimestamps.push_back(scl_timesptamp.asFloat());

		BoneTransforms.insert({BoneName,std::move(Transform)});
	}
	return true;
}

bool FAnimation::Serialize(OUT FJson& Out)
{
	Out["AnimationName"] = AnimationName;
	Out["Duration"] = Duration;
	Out["TicksPerSecond"] = TicksPerSecond;
	FJson& bone_transforms = Out["BoneTransforms"];
	for (auto& Transform : BoneTransforms)
	{
		FJson bone_transform;

		FJson& bone_name = bone_transform["BoneName"];

		FJson& positions = bone_transform["Positions"];
		FJson& rotations = bone_transform["Rotations"];
		FJson& scales	 = bone_transform["Scales"];

		FJson& position_timestamps = bone_transform["PositionTimestamps"];
		FJson& rotation_timestamps = bone_transform["RotationTimestamps"];
		FJson& scale_timestamps	   = bone_transform["ScaleTimestamps"];

		bone_name = Transform.first;

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

		bone_transforms.append(bone_transform);
	}
	return true;
}

FAnimation::FAnimation(const char* name)
{
	Type = EResourceType::Animation;
	Rename(name);
}

void FAnimation::SetData(const aiAnimation* anim)
{
	if (!anim)return;

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

	Duration = anim->mDuration;
	TicksPerSecond = anim->mTicksPerSecond;
	BoneTransforms.clear();

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
		BoneTransforms[Channel->mNodeName.C_Str()] = Track;
	}

}

void FAnimation::Register()
{
	FResourceManager& ResourceManager = FResourceManager::Get();
	if (ResourceManager.FindObject<FAnimation>(LastName))
	{
		ResourceManager.RemoveObject<FAnimation>(LastName);
	}
	ResourceManager.Register<FAnimation>(shared_from_this());
}

void FAnimation::Rename(const string& name)
{
	//PUSH_TRACE_MSG("Animation", "Rename From %s To %s", AnimationName.c_str(), name.c_str());
	LastName = AnimationName;
	AnimationName = name;
	CachePath = "/Cache/Animation/" + AnimationName + ".fanimation";
}
