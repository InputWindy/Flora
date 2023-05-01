#pragma once
#include "Core.h"
#include "Scene.h"
using std::unordered_map;
class FLORA_API FSceneManager
{
protected:
	FSceneManager() = default;
public:
	virtual ~FSceneManager() = default;

	static inline FSceneManager& Get() { static FSceneManager msSceneManager; return msSceneManager; };
public:
	void BeginUpdate();
	void Update();
	void EndUpdate();

	Ref<FScene> AddScene(const string&);
	Ref<FScene> FindScene(const string&);
	Ref<FScene> GetCurrentScene();
	void RemoveScene(const string&);
	void SetCurrentScene(Ref<FScene>);

private:
	unordered_map<string, Ref<FScene>> Scenes;
	Ref<FScene> CurrentScene = nullptr;
};

