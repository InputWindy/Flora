#version 460 core
#include "GpuScene.glsl"

layout(location = 0) out vec4 FragColor;
layout(location = 1) out ivec2 VisibilityBuffer;

in flat int InstanceID;
in vec3 Color;

void main()
{
	FragColor = vec4(Color,1);
//	VisibilityBuffer = ivec2(InstanceID,gl_PrimitiveID);
}