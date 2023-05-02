#include "SceneManager.h"
#include "InputSystem.h"
#include "Viewport.h"
#include <memory>
void FSceneManager::BeginUpdate()
{
	FInputSystem::Get().GatherInputEvent();
	if (CurrentScene)
	{
		if (FGameScene::GetMainScene().IsFocused())
		{
			CurrentScene->ProcessInputEvent();
		}
		CurrentScene->BeginUpdate();
	}
}
void FSceneManager::Update()
{
	if (CurrentScene)
	{
		CurrentScene->Update();
	}
}

void FSceneManager::EndUpdate()
{
	if (CurrentScene)
	{
		CurrentScene->EndUpdate();
	}
}

Ref<FScene> FSceneManager::AddScene(const string& name) 
{
	Ref<FScene> Res = FindScene(name);
	if (!Res)
	{
		Scenes.insert({name,std::make_shared<FScene>(name)});
	}
	return FindScene(name);
}

Ref<FScene> FSceneManager::FindScene(const string& name)
{
	Ref<FScene> Res = nullptr;
	auto find = Scenes.find(name);
	if (find != Scenes.end())
	{
		Res = find->second;
	};
	return Res;
}

Ref<FScene> FSceneManager::GetCurrentScene()
{
	return CurrentScene;
}

void FSceneManager::RemoveScene(const string& name)
{
	if (CurrentScene->GetName() == name)CurrentScene = nullptr;
	auto find = Scenes.find(name);
	if (find != Scenes.end())
	{
		Scenes.erase(find);
	};
}

void FSceneManager::SetCurrentScene(Ref<FScene> s)
{
	CurrentScene = s;
}
