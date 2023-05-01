#include "CameraObject.h"
#include "InputSystem.h"
#include <imgui.h>
FCameraObject::FCameraObject(const char* name, FScene* owner)
	:FGameObject(name,EGameObjectType::CameraObject,owner)
{
	AddComponent<FTransformComponent>();
	AddComponent<FCameraComponent>();
}

bool FCameraObject::OnKeyPressedEvent(FKeyPressedEvent& e)
{
	FInputSystem& InputSystem = FInputSystem::Get();
	if(InputSystem.IsKeyPressed(ImGuiKey_A))
		printf("A");
	return true;
}

bool FCameraObject::OnKeyReleasedEvent(FKeyReleasedEvent& e)
{
	return false;
}

bool FCameraObject::OnKeyTypedEvent(FKeyTypedEvent& e)
{
	return false;
}

bool FCameraObject::OnMouseMoveEvent(FMouseMoveEvent& e)
{
	return false;
}

bool FCameraObject::OnMouseScrolledEvent(FMouseScrolledEvent& e)
{
	return false;
}

bool FCameraObject::OnMouseButtonPressedEvent(FMouseButtonPressedEvent& e)
{
	return false;
}

bool FCameraObject::OnMouseButtonReleasedEvent(FMouseButtonReleasedEvent& e)
{
	return false;
}

bool FCameraObject::BeginUpdate()
{
	return false;
}

bool FCameraObject::Update()
{
	FTransform Parent;
	//GetComponent<FTransform>()->Update(Parent);
	return true;
}

bool FCameraObject::EndUpdate()
{
	return false;
}

bool FCameraObject::Parse(IN FJson&)
{
	return false;
}

bool FCameraObject::Serialize(OUT FJson&)
{
	return false;
}
