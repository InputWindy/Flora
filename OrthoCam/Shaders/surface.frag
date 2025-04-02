#version 460 core

layout(location = 0)out uvec4 Color0;

uniform uint u_Mask;

void main()
{
	Color0 = uvec4(255u,0u,0u,255u);
}
