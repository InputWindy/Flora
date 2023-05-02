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
FTransform::FTransform()
{
	UpdateCoord();
}

glm::mat4 FTransform::GetModelMatrix()
{
	return glm::inverse(GetViewMatrix());
}

glm::mat4 FTransform::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Front, Up);
}

void FTransform::MoveForward(float ds)
{
	Position += Front * ds;
}

void FTransform::MoveBack(float ds)
{
	Position -= Front * ds;
}

void FTransform::MoveLeft(float ds)
{
	Position -= Right * ds;
}

void FTransform::MoveRight(float ds)
{
	Position += Right * ds;
}

void FTransform::MoveUp(float dS)
{
	Position += Up * dS;
}

void FTransform::MoveDown(float dS)
{
	Position -= Up * dS;
}

void FTransform::RotateX(float dAngle, bool constrain)
{
	Rotation.x += dAngle;
	if (constrain)
	{
		if (Rotation.x > 89.0f)
			Rotation.x = 89.0f;
		if (Rotation.x < -89.0f)
			Rotation.x = -89.0f;
	}
	UpdateCoord();
}

void FTransform::RotateY(float dAngle, bool constrain)
{
	Rotation.y += dAngle;
	if (constrain)
	{
		if (Rotation.y > 89.0f)
			Rotation.y = 89.0f;
		if (Rotation.y < -89.0f)
			Rotation.y = -89.0f;
	}
	UpdateCoord();
}

void FTransform::RotateZ(float dAngle, bool constrain)
{
	Rotation.z += dAngle;
	if (constrain)
	{
		if (Rotation.z > 89.0f)
			Rotation.z = 89.0f;
		if (Rotation.z < -89.0f)
			Rotation.z = -89.0f;
	}
	UpdateCoord();
}

void FTransform::UpdateCoord()
{ 
	// calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
	front.y = sin(glm::radians(Rotation.x));
	front.z = sin(glm::radians(Rotation.y)) * cos(glm::radians(Rotation.x));
	Front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}
