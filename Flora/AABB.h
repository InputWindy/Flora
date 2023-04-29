#pragma once
#include "Core.h"
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
using glm::vec3;
struct FLORA_API FAABB
{
	vec3 Max = vec3(-FLT_MAX);
	vec3 Min = vec3(FLT_MAX);

	vec3 Center = vec3(0.0f);
	vec3 Extend = vec3(0.0f);

	void AddPoint(const vec3&);
	void Merge(const FAABB&);
	glm::mat4 GetTransform()const;
};

