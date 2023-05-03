#include "CameraObject.h"
#include "Transform.h"
#include "InputSystem.h"
#include "Application.h"
#include "Viewport.h"
#include <imgui.h>

FCameraObject::FCameraObject(const char* name, FScene* owner,uint32_t id)
	:FGameObject(name,EGameObjectType_CameraObject,owner,id)
{
	AddComponent<FCameraComponent>();
}

bool FCameraObject::Parse(IN FJson&)
{
	return false;
}

bool FCameraObject::Serialize(OUT FJson&)
{
	return false;
}
