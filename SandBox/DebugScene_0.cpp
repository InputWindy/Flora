#include "DebugScene_0.h"
#include "DebugCamera.h"


void DebugScene_0::OnAwake()
{
	FSceneManager& SceneManager = FSceneManager::Get();
	Ref<FScene> CurrentScene = SceneManager.AddScene("DebugScene_0");
	SceneManager.SetCurrentScene(CurrentScene);
	CurrentScene->SpawnDebugCamera<DebugCamera>("DebugCamera");
}

void DebugScene_0::OnRelease()
{
}

void DebugScene_0::OnUpdate()
{
}

void DebugScene_0::OnGUI()
{
}
