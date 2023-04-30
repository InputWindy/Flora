#include "Transform.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
#include <ImGuizmo.h>
using namespace glm;
void FTransform::Update(mat4 InParentTransformToRoot)
{
	glm::mat4 InvParentTransformToRoot = glm::inverse(InParentTransformToRoot);

	vec3 parent_pos, parent_rotate, parent_scale;
	ImGuizmo::DecomposeMatrixToComponents(
		value_ptr(InParentTransformToRoot), 
		value_ptr(parent_pos), 
		value_ptr(parent_rotate), 
		value_ptr(parent_scale));

	//scale->rotate->transform
	TransformToParent =
	{
		LocalScale.x / parent_scale.x,0,0,0,
		0,LocalScale.y / parent_scale.y,0,0,
		0,0,LocalScale.z / parent_scale.z,0,
		0,0,0,1
	};

	TransformToParent = mat4_cast(glm::qua<float>(glm::radians(LocalRotation))) * TransformToParent;
	TransformToParent[3][0] = LocalPosition.x / parent_scale.x;
	TransformToParent[3][1] = LocalPosition.y / parent_scale.y;
	TransformToParent[3][2] = LocalPosition.z / parent_scale.z;
	TransformToParent[3][3] = 1;

	TransformToRoot = InParentTransformToRoot * TransformToParent;
}

void FTransform::ToLocalSpace(vec3& v)
{
	v = InvTransformToRoot * vec4(v, 1.0f);
}

void FTransform::MoveWorldForward(float ds)
{
	vec3 Dir = glm::normalize(WorldCoordFront) * ds;
	ToLocalSpace(Dir);
	LocalPosition += Dir;
}
void FTransform::MoveWorldBack(float ds)
{
	vec3 Dir = glm::normalize(WorldCoordFront) * ds;
	ToLocalSpace(Dir);
	LocalPosition -= Dir;
}
void FTransform::MoveWorldLeft(float ds)
{
	vec3 Dir = glm::normalize(WorldCoordRight) * ds;
	ToLocalSpace(Dir);
	LocalPosition -= Dir;
}
void FTransform::MoveWorldRight(float ds)
{
	vec3 Dir = glm::normalize(WorldCoordRight) * ds;
	ToLocalSpace(Dir);
	LocalPosition += Dir;
}
void FTransform::MoveWorldUp(float ds)
{
	vec3 Dir = glm::normalize(WorldCoordUp) * ds;
	ToLocalSpace(Dir);
	LocalPosition += Dir;
}
void FTransform::MoveWorldDown(float ds)
{
	vec3 Dir = glm::normalize(WorldCoordUp) * ds;
	ToLocalSpace(Dir);
	LocalPosition -= Dir;
}

void FTransform::MoveWorldX(float dx)
{
	vec3 Dir = { dx,0,0 };
	ToLocalSpace(Dir);
	LocalPosition += Dir;
}

void FTransform::MoveWorldY(float dy)
{
	vec3 Dir = { 0,dy,0 };
	ToLocalSpace(Dir);
	LocalPosition += Dir;
}

void FTransform::MoveWorldZ(float dz)
{
	vec3 Dir = { 0,0,dz };
	ToLocalSpace(Dir);
	LocalPosition += Dir;
}

void FTransform::MoveWorld(vec3 ds)
{
	ToLocalSpace(ds);
	LocalPosition += ds;
}

void FTransform::MoveToWorldPos(vec3 WorldPos)
{
	ToLocalSpace(WorldPos);
	LocalPosition = WorldPos;
}

void FTransform::MoveLocalForward(float ds)
{
	vec3 Dir = glm::normalize(WorldCoordFront) * ds;
	LocalPosition += Dir;
}

void FTransform::MoveLocalBack(float ds)
{
	vec3 Dir = glm::normalize(WorldCoordFront) * ds;
	LocalPosition -= Dir;
}

void FTransform::MoveLocalLeft(float ds)
{
	vec3 Dir = glm::normalize(WorldCoordRight) * ds;
	LocalPosition -= Dir;
}

void FTransform::MoveLocalRight(float ds)
{
	vec3 Dir = glm::normalize(WorldCoordRight) * ds;
	LocalPosition += Dir;
}

void FTransform::MoveLocalUp(float ds)
{
	vec3 Dir = glm::normalize(WorldCoordUp) * ds;
	LocalPosition += Dir;
}

void FTransform::MoveLocalDown(float ds)
{
	vec3 Dir = glm::normalize(WorldCoordUp) * ds;
	LocalPosition -= Dir;
}

void FTransform::MoveLocalX(float dx)
{
	LocalPosition.x += dx;
}

void FTransform::MoveLocalY(float dy)
{
	LocalPosition.y += dy;
}

void FTransform::MoveLocalZ(float dz)
{
	LocalPosition.z += dz;
}

void FTransform::MoveLocal(vec3 ds)
{
	LocalPosition += ds;
}

void FTransform::MoveToLocalPos(vec3 LocalPos)
{
	LocalPosition = LocalPos;
}

void FTransform::RotateToLocal(vec3 Angle)
{
	LocalRotation = Angle;
}

void FTransform::RotateLocal(vec3 dAngle)
{
	LocalRotation += dAngle;
}
