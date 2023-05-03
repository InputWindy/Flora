#pragma once
#include "Flora.h"
#include <iostream>
class DebugCamera:public FCameraObject
{
public:
	DebugCamera(const char*, FScene*,uint32_t);
	virtual ~DebugCamera() = default;
public:
	void CameraAwake()
	{
		printf("OnAwake");
	}

	void CameraUpdate(float)
	{
		FTransformComponent* RootComponent = GetComponent<FTransformComponent>();
		FCameraComponent* CameraComponent = GetComponent<FCameraComponent>();
		if (RootComponent and CameraComponent)
		{
			float MovementSpeed = CameraComponent->GetMovementSpeed();
			float MouseSensitivity = CameraComponent->GetMouseSensitivity();
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
					float dAngleY = Input.MouseDeltaX() * MouseSensitivity;
					Transform.RotateX(dAngleX);
					Transform.RotateY(dAngleY);
				}
			}
		}
		else assert(0, "Null Transform");
	}

	void CameraRelease()
	{

	}
};
