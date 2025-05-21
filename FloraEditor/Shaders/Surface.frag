#version 460 core
layout(location = 0)out ivec2 RenderTarget;

uniform int u_MeshID;

void main()
{
	RenderTarget = ivec2(u_MeshID + 1,gl_PrimitiveID + 1);
}
