#include "CameraObject.h"
#include "Transform.h"
#include "InputSystem.h"
#include "Application.h"
#include "Viewport.h"
#include <imgui.h>

FCameraObject::FCameraObject(const char* name, FScene* owner)
	:FGameObject(name,EGameObjectType::CameraObject,owner)
{
	AddComponent<FTransformComponent>();
	AddComponent<FCameraComponent>();
	AddComponent<FScriptComponent>();
}

bool FCameraObject::OnKeyPressedEvent(FKeyPressedEvent& e)
{
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
	

	FInputSystem& InputSystem = FInputSystem::Get();
	FTransformComponent* RootComponent = GetComponent<FTransformComponent>();
	if (RootComponent)
	{
		if (FGameScene::GetMainScene().IsFocused())
		{
			float DeltaTime = FApplication::GetApp()->GetAppDeltaTime();
			FTransform& Transform = RootComponent->GetTransform();
			FInputSystem& Input = FInputSystem::Get();

			if (Input.IsKeyPressed(F_KEY_W))
				Transform.MoveForward(DeltaTime * MovementSpeed);
			if (Input.IsKeyPressed(F_KEY_S))
				Transform.MoveBack(DeltaTime * MovementSpeed);
			if (Input.IsKeyPressed(F_KEY_A))
				Transform.MoveLeft(DeltaTime * MovementSpeed);
			if (Input.IsKeyPressed(F_KEY_D))
				Transform.MoveRight(DeltaTime * MovementSpeed);
			if (Input.IsKeyPressed(F_KEY_SPACE))
			{
				float dAngleX = -Input.MouseDeltaY() * MouseSensitivity;
				float dAngleY =  Input.MouseDeltaX() * MouseSensitivity;
				Transform.RotateX(dAngleX);
				Transform.RotateY(dAngleY);
			}
		}
	}
	else assert(0, "Null Transform");

	if (ParentId == -1)
	{
		FTransform Parent;
		GetComponent<FTransformComponent>()->Update(Parent);
	}
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
