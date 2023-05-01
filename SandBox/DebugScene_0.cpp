#include "DebugScene_0.h"

void DebugScene_0::OnAwake()
{
	FSceneManager& SceneManager = FSceneManager::Get();
	SceneManager.SetCurrentScene(SceneManager.AddScene("DebugScene_0"));
}

void DebugScene_0::OnUpdate()
{
}

void DebugScene_0::OnGUI()
{
}
