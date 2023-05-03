#include "Scene.h"
#include "CameraObject.h"
#include "InputSystem.h"
#include <memory>
using namespace std;
FScene::FScene(const string& name)
	:SceneName(name)
{
	
}


void FScene::Rename(const string& name)
{
	SceneName = name;
}

void FScene::SetMainCamera(Ref<FCameraObject> Camera)
{
	MainCamera = Camera;
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
	if(MainCamera) MainCamera->BeginUpdate();
}

void FScene::Update()
{
	if (MainCamera) MainCamera->Update();
}

void FScene::EndUpdate()
{
	if (MainCamera) MainCamera->EndUpdate();
	
}

void FScene::Awake()
{
	if (MainCamera) MainCamera->Awake();
}

void FScene::Release()
{
	if (MainCamera) MainCamera->Release();
}

void FScene::ProcessInputEvent()
{
	FInputSystem& Input = FInputSystem::Get();
	if (MainCamera)
	{
		for (auto& e : Input.KeyPressedEvents)
		{
			MainCamera->HandleInputEvent(e.second);
		}

		for (auto& e : Input.KeyReleasedEvents)
		{
			MainCamera->HandleInputEvent(e.second);
		}

		for (auto& e : Input.KeyTypedEvents)
		{
			MainCamera->HandleInputEvent(e.second);
		}

		for (auto& e : Input.MouseButtonPressedEvents)
		{
			MainCamera->HandleInputEvent(e.second);
		}

		for (auto& e : Input.MouseButtonReleasedEvents)
		{
			MainCamera->HandleInputEvent(e.second);
		}

		MainCamera->HandleInputEvent(Input.MouseMovedEvent);
		MainCamera->HandleInputEvent(Input.MouseScrolledEvent);
	}
	
}


uint32_t FScene::FObjectManager::NextId()
{
	uint32_t Res = GameObjects.size();
	for (uint32_t Idx = 0; Idx < GameObjects.size(); ++Idx)
	{
		if (GameObjects[Idx] == nullptr)
		{
			Res = Idx;
			break;
		}
	}
	if (Res == GameObjects.size())
	{
		GameObjects.push_back(nullptr);
	}
	return Res;
};

bool FScene::FObjectManager::IsValidId(uint32_t Id)
{
	return Id < GameObjects.size();
};

bool FScene::FObjectManager::RegisterObj(EGameObjectType T, uint32_t Id, Ref<FGameObject> Obj)
{
	if (IsValidId(Id))
	{
		GameObjectsMap[T].push_back(Id);
		GameObjects[Id] = Obj;
		return true;
	}
	return false;
};

Ref<FGameObject> FScene::FObjectManager::GetGameObjectByID(uint32_t Id)
{
	Ref<FGameObject> Res = nullptr;
	if (Id < GameObjects.size())
	{
		Res = GameObjects[Id];
	}
	return Res;
};

Ref<FGameObject> FScene::FObjectManager::GetGameObjectByID(EGameObjectType T,uint32_t Id)
{
	Ref<FGameObject> Res = nullptr;
	if (Id < GameObjectsMap[T].size())
	{
		Res = GetGameObjectByID(GameObjectsMap[T][Id]);
	}
	return Res;
};

bool FScene::FObjectManager::RemoveObj(uint32_t Id)
{
	if (IsValidId(Id))
	{
		for (auto& Category : GameObjectsMap)
		{
			for (auto Iter = Category.second.begin(); Iter != Category.second.end(); ++Iter)
			{
				if (*Iter == Id)
				{
					Category.second.erase(Iter);
				}
			};
		}
		GameObjects[Id] = nullptr;
		return true;
	}
	return false;
};

bool FScene::FObjectManager::RemoveObj(Ref<FGameObject> Obj)
{
	if (Obj == nullptr)return false;
	return RemoveObj(Obj->GetId());
};