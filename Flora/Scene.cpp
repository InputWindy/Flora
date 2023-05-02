#include "Scene.h"
#include "CameraObject.h"
#include "InputSystem.h"
#include <memory>
using namespace std;
FScene::FScene(const string& name)
	:SceneName(name)
{
	//DebugCamera = make_shared<FCameraObject>("DebugCamera",this);
}

void FScene::Rename(const string& name)
{
	SceneName = name;
}

bool FScene::Parse(IN FJson&)
{
	return false;
}

bool FScene::Serialize(OUT FJson&)
{
	return false;
}

void FScene::BeginUpdate()
{
	DebugCamera->BeginUpdate();
}

void FScene::Update()
{
	DebugCamera->Update();
}

void FScene::EndUpdate()
{
	DebugCamera->EndUpdate();
	
}

void FScene::Awake()
{
	DebugCamera->Awake();
}

void FScene::Release()
{
	DebugCamera->Release();
}

void FScene::ProcessInputEvent()
{
	FInputSystem& Input = FInputSystem::Get();
	
	//send event to
	for (auto& e : Input.KeyPressedEvents)
	{
		DebugCamera->HandleInputEvent(e.second);
	}

	for (auto& e : Input.KeyReleasedEvents)
	{
		DebugCamera->HandleInputEvent(e.second);
	}

	for (auto& e : Input.KeyTypedEvents)
	{
		DebugCamera->HandleInputEvent(e.second);
	}

	for (auto& e : Input.MouseButtonPressedEvents)
	{
		DebugCamera->HandleInputEvent(e.second);
	}

	for (auto& e : Input.MouseButtonReleasedEvents)
	{
		DebugCamera->HandleInputEvent(e.second);
	}

	DebugCamera->HandleInputEvent(Input.MouseMovedEvent);
	DebugCamera->HandleInputEvent(Input.MouseScrolledEvent);
}
