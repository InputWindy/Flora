#version 460 core

layout(location = 0) out vec4 DebugNormal;

in vec3 Normal;

void main()
{
	DebugNormal = vec4(Normal,1);
}