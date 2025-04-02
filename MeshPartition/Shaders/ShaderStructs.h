#ifndef SHADER_STRUCT_H
#define SHADER_STRUCT_H

struct FVertex
{
	vec4 Position;
	vec4 Normal;
	vec4 UV;
};

struct FMesh
{
	int GlobalTriOffset;
	int Dummy0;
	int Dummy1;
	int Dummy2;

	vec4 BoundingSphere;
};

struct FScene
{
	int NumMeshes;
	int NumTriangles;
	int Dummy0;
	int Dummy1;

	vec4 BoundingSphere;
};

struct FResults
{

};

#endif
