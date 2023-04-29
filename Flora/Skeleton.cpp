#include "Skeleton.h"
#include <functional>
#include "ResourceManager.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
struct FSkeletonEnabler :public FSkeleton {};
FSkeleton::FSkeleton(const char* name)
{
	Type = EResourceType::Skeleton;
	Rename(name);
};

bool FSkeleton::Parse(IN FJson& In)
{
	Rename(In["SkeletonName"].asString());
	BoneNum = In["BoneNum"].asUInt();

	FJson& g_inv_mat = In["GlobalInverseMatrix"];
	GlobalInverseMatrix[0][0] = g_inv_mat["00"].asFloat();
	GlobalInverseMatrix[0][1] = g_inv_mat["01"].asFloat();
	GlobalInverseMatrix[0][2] = g_inv_mat["02"].asFloat();
	GlobalInverseMatrix[0][3] = g_inv_mat["03"].asFloat();

	GlobalInverseMatrix[1][0] = g_inv_mat["10"].asFloat();
	GlobalInverseMatrix[1][1] = g_inv_mat["11"].asFloat();
	GlobalInverseMatrix[1][2] = g_inv_mat["12"].asFloat();
	GlobalInverseMatrix[1][3] = g_inv_mat["13"].asFloat();

	GlobalInverseMatrix[2][0] = g_inv_mat["20"].asFloat();
	GlobalInverseMatrix[2][1] = g_inv_mat["21"].asFloat();
	GlobalInverseMatrix[2][2] = g_inv_mat["22"].asFloat();
	GlobalInverseMatrix[2][3] = g_inv_mat["23"].asFloat();

	GlobalInverseMatrix[3][0] = g_inv_mat["30"].asFloat();
	GlobalInverseMatrix[3][1] = g_inv_mat["31"].asFloat();
	GlobalInverseMatrix[3][2] = g_inv_mat["32"].asFloat();
	GlobalInverseMatrix[3][3] = g_inv_mat["33"].asFloat();

	RootBone.Parse(In["BoneTree"]);
	return true;
}

bool FSkeleton::Serialize(OUT FJson& Out)
{
	Out["SkeletonName"] = SkeletonName;
	Out["BoneNum"] = BoneNum;

	FJson& g_inv_mat = Out["GlobalInverseMatrix"];
	g_inv_mat["00"] = GlobalInverseMatrix[0][0];
	g_inv_mat["01"] = GlobalInverseMatrix[0][1];
	g_inv_mat["02"] = GlobalInverseMatrix[0][2];
	g_inv_mat["03"] = GlobalInverseMatrix[0][3];

	g_inv_mat["10"] = GlobalInverseMatrix[1][0];
	g_inv_mat["11"] = GlobalInverseMatrix[1][1];
	g_inv_mat["12"] = GlobalInverseMatrix[1][2];
	g_inv_mat["13"] = GlobalInverseMatrix[1][3];

	g_inv_mat["20"] = GlobalInverseMatrix[2][0];
	g_inv_mat["21"] = GlobalInverseMatrix[2][1];
	g_inv_mat["22"] = GlobalInverseMatrix[2][2];
	g_inv_mat["23"] = GlobalInverseMatrix[2][3];

	g_inv_mat["30"] = GlobalInverseMatrix[3][0];
	g_inv_mat["31"] = GlobalInverseMatrix[3][1];
	g_inv_mat["32"] = GlobalInverseMatrix[3][2];
	g_inv_mat["33"] = GlobalInverseMatrix[3][3];

	RootBone.Serialize(Out["BoneTree"]);

	return true;
}

void FSkeleton::Register()
{
	FResourceManager& ResourceManager = FResourceManager::Get();
	if (ResourceManager.FindObject<FSkeleton>(SkeletonName))
	{
		ResourceManager.RemoveObject<FSkeleton>(SkeletonName);
	}
	ResourceManager.Register<FSkeleton>(shared_from_this());
}

void FSkeleton::Rename(const string& name)
{
	SkeletonName = name;
	CachePath = "/Cache/Skeleton/" + SkeletonName + ".fskeleton";
}

void FSkeleton::SetData(const aiScene* scene)
{
	if (!scene)return;
	auto assimpToGlmMatrix = [](aiMatrix4x4 mat)->glm::mat4
	{
		glm::mat4 m;
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				m[x][y] = mat[y][x];
			}
		}
		return m;
	};

	unordered_map<string, pair<int, mat4>> BoneInfoTable;

	for (uint32_t Idx = 0; Idx < scene->mNumMeshes; Idx++)
	{
		aiMesh* mesh = scene->mMeshes[Idx];
		for (uint32_t BoneIdx = 0; BoneIdx < mesh->mNumBones; BoneIdx++)
		{
			aiBone* bone = mesh->mBones[BoneIdx];
			glm::mat4 m = assimpToGlmMatrix(bone->mOffsetMatrix);
			BoneInfoTable[bone->mName.C_Str()] = { BoneIdx, m };
		}
	}

	BoneNum = BoneInfoTable.size();

	std::function<void(FBone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable, glm::mat4)>
		readSkeleton = [&readSkeleton, &assimpToGlmMatrix,this](FBone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable, mat4 transform)
	{
		if (boneInfoTable.find(node->mName.C_Str()) != boneInfoTable.end())
		{
			boneOutput.Name = node->mName.C_Str();
			boneOutput.Id = boneInfoTable[boneOutput.Name].first;
			boneOutput.OffsetMatrix = boneInfoTable[boneOutput.Name].second;
			boneOutput.TransformMatrix = transform;

			for (int i = 0; i < node->mNumChildren; i++)
			{
				FBone child;
				readSkeleton(child, node->mChildren[i], boneInfoTable, assimpToGlmMatrix(node->mTransformation));
				boneOutput.Children.push_back(child);
			}
			return;
		}
		else
		{
			for (int i = 0; i < node->mNumChildren; i++)
			{
				readSkeleton(boneOutput, node->mChildren[i], boneInfoTable, transform);
			}
		}
		return;
	};

	readSkeleton(RootBone, scene->mRootNode, BoneInfoTable, mat4(1.0f));
}

bool FBone::Parse(IN FJson& In)
{
	Name = In["BoneName"].asString();
	Id = In["Id"].asInt();
	FJson& offset_matrix = In["OffsetMatrix"];
	OffsetMatrix[0][0] = offset_matrix["00"].asFloat();
	OffsetMatrix[0][1] = offset_matrix["01"].asFloat();
	OffsetMatrix[0][2] = offset_matrix["02"].asFloat();
	OffsetMatrix[0][3] = offset_matrix["03"].asFloat();

	OffsetMatrix[1][0] = offset_matrix["10"].asFloat();
	OffsetMatrix[1][1] = offset_matrix["11"].asFloat();
	OffsetMatrix[1][2] = offset_matrix["12"].asFloat();
	OffsetMatrix[1][3] = offset_matrix["13"].asFloat();

	OffsetMatrix[2][0] = offset_matrix["20"].asFloat();
	OffsetMatrix[2][1] = offset_matrix["21"].asFloat();
	OffsetMatrix[2][2] = offset_matrix["22"].asFloat();
	OffsetMatrix[2][3] = offset_matrix["23"].asFloat();

	OffsetMatrix[3][0] = offset_matrix["30"].asFloat();
	OffsetMatrix[3][1] = offset_matrix["31"].asFloat();
	OffsetMatrix[3][2] = offset_matrix["32"].asFloat();
	OffsetMatrix[3][3] = offset_matrix["33"].asFloat();

	FJson& transform_matrix = In["TransformMatrix"];
	TransformMatrix[0][0] = transform_matrix["00"].asFloat();
	TransformMatrix[0][1] = transform_matrix["01"].asFloat();
	TransformMatrix[0][2] = transform_matrix["02"].asFloat();
	TransformMatrix[0][3] = transform_matrix["03"].asFloat();

	TransformMatrix[1][0] = transform_matrix["10"].asFloat();
	TransformMatrix[1][1] = transform_matrix["11"].asFloat();
	TransformMatrix[1][2] = transform_matrix["12"].asFloat();
	TransformMatrix[1][3] = transform_matrix["13"].asFloat();

	TransformMatrix[2][0] = transform_matrix["20"].asFloat();
	TransformMatrix[2][1] = transform_matrix["21"].asFloat();
	TransformMatrix[2][2] = transform_matrix["22"].asFloat();
	TransformMatrix[2][3] = transform_matrix["23"].asFloat();

	TransformMatrix[3][0] = transform_matrix["30"].asFloat();
	TransformMatrix[3][1] = transform_matrix["31"].asFloat();
	TransformMatrix[3][2] = transform_matrix["32"].asFloat();
	TransformMatrix[3][3] = transform_matrix["33"].asFloat();

	for (auto& child : In["Children"])
	{
		FBone Bone;
		Bone.Parse(child);
		Children.push_back(Bone);
	}
	return true;
}

bool FBone::Serialize(OUT FJson& Out)
{
	Out["BoneName"] = Name;
	Out["Id"] = Id;
	FJson& offset_matrix = Out["OffsetMatrix"];
	offset_matrix["00"] = OffsetMatrix[0][0];
	offset_matrix["01"] = OffsetMatrix[0][1];
	offset_matrix["02"] = OffsetMatrix[0][2];
	offset_matrix["03"] = OffsetMatrix[0][3];

	offset_matrix["10"] = OffsetMatrix[1][0];
	offset_matrix["11"] = OffsetMatrix[1][1];
	offset_matrix["12"] = OffsetMatrix[1][2];
	offset_matrix["13"] = OffsetMatrix[1][3];

	offset_matrix["20"] = OffsetMatrix[2][0];
	offset_matrix["21"] = OffsetMatrix[2][1];
	offset_matrix["22"] = OffsetMatrix[2][2];
	offset_matrix["23"] = OffsetMatrix[2][3];

	offset_matrix["30"] = OffsetMatrix[3][0];
	offset_matrix["31"] = OffsetMatrix[3][1];
	offset_matrix["32"] = OffsetMatrix[3][2];
	offset_matrix["33"] = OffsetMatrix[3][3];

	FJson& transform_matrix = Out["TransformMatrix"];
	transform_matrix["00"] = TransformMatrix[0][0];
	transform_matrix["01"] = TransformMatrix[0][1];
	transform_matrix["02"] = TransformMatrix[0][2];
	transform_matrix["03"] = TransformMatrix[0][3];

	transform_matrix["10"] = TransformMatrix[1][0];
	transform_matrix["11"] = TransformMatrix[1][1];
	transform_matrix["12"] = TransformMatrix[1][2];
	transform_matrix["13"] = TransformMatrix[1][3];

	transform_matrix["20"] = TransformMatrix[2][0];
	transform_matrix["21"] = TransformMatrix[2][1];
	transform_matrix["22"] = TransformMatrix[2][2];
	transform_matrix["23"] = TransformMatrix[2][3];

	transform_matrix["30"] = TransformMatrix[3][0];
	transform_matrix["31"] = TransformMatrix[3][1];
	transform_matrix["32"] = TransformMatrix[3][2];
	transform_matrix["33"] = TransformMatrix[3][3];

	for (auto& Child :Children)
	{
		FJson child_bone;
		Child.Serialize(child_bone);
		Out["Children"].append(child_bone);
	}
	return true;
}
