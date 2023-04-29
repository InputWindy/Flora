#include "Mesh.h"
#include "ResourceManager.h"
#include "Console.h"
using namespace glm;
using namespace std;
FMesh::FMesh()
{
	Type = EResourceType::Mesh;
}

void FMesh::GetAABB(const mat4& ModelMatrix, FAABB& InOut)
{
	vec4 b_0 = ModelMatrix * vec4(OBB.Min.x, OBB.Min.y, OBB.Min.z, 1);
	vec4 b_1 = ModelMatrix * vec4(OBB.Min.x, OBB.Min.y, OBB.Max.z, 1);
	vec4 b_2 = ModelMatrix * vec4(OBB.Max.x, OBB.Min.y, OBB.Min.z, 1);
	vec4 b_3 = ModelMatrix * vec4(OBB.Max.x, OBB.Min.y, OBB.Max.z, 1);

	vec4 t_0 = ModelMatrix * vec4(OBB.Min.x, OBB.Max.y, OBB.Min.z, 1);
	vec4 t_1 = ModelMatrix * vec4(OBB.Min.x, OBB.Max.y, OBB.Max.z, 1);
	vec4 t_2 = ModelMatrix * vec4(OBB.Max.x, OBB.Max.y, OBB.Min.z, 1);
	vec4 t_3 = ModelMatrix * vec4(OBB.Max.x, OBB.Max.y, OBB.Max.z, 1);

	FAABB Tmp;
	Tmp.AddPoint(b_0);
	Tmp.AddPoint(b_1);
	Tmp.AddPoint(b_2);
	Tmp.AddPoint(b_3);
	Tmp.AddPoint(t_0);
	Tmp.AddPoint(t_1);
	Tmp.AddPoint(t_2);
	Tmp.AddPoint(t_3);

	InOut = Tmp;

	InOut.Center = InOut.Min + InOut.Max;
	InOut.Center.x /= 2;
	InOut.Center.y /= 2;
	InOut.Center.z /= 2;

	InOut.Extend = glm::abs(InOut.Max - InOut.Min);
	InOut.Extend.x /= 2;
	InOut.Extend.y /= 2;
	InOut.Extend.z /= 2;
}

void FMesh::Register()
{
	FResourceManager& ResourceManager = FResourceManager::Get();
	if (ResourceManager.FindObject<FMesh>(LastName))
	{
		ResourceManager.RemoveObject<FMesh>(LastName);
	}
	ResourceManager.Register<FMesh>(shared_from_this());
}

void FMesh::Rename(const string& name)
{
	string comment = "Rename From ";
	comment += MeshName;
	comment += " To ";
	comment += name;
	PUSH_TRACE_MSG("Mesh", comment.c_str());
	LastName = MeshName;
	MeshName = name;
	CachePath = "/Cache/Mesh/" + MeshName + ".fmesh";
}

bool FMesh::Parse(IN FJson& In)
{
	assert(0, "FMesh::Parse Unused!");
	return true;
}

bool FMesh::Serialize(OUT FJson& Out)
{
	Out["MeshName"] = MeshName;
	Out["ModelResourcePath"] = ModelResourcePath;
	Out["ModelMeshName"] = ModelMeshName;
	return true;
}
