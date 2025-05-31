#version 460 core
#include "GpuScene.glsl"

out flat int InstanceID;
out vec3 Color;

void main()
{
	InstanceID = int(gl_InstanceID);

	vec3 Pos = LoadPosVec3(gl_VertexID * 3);
	mat4 Model = GetModelMatrix(gl_InstanceID);
	mat4 View = GetViewMatrix();
	mat4 Proj = GetProjMatrix();

	AtomicAddVertexCntStat();

	Color = normalize(Pos) * 0.5 + 0.5;

	gl_Position = Proj * View * Model * vec4(Pos,1);
}