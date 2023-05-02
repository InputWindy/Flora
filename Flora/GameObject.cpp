#include "GameObject.h"
#include <imgui.h>
#include <functional>
#define BIND_EVENT_FUNCTION(func) std::bind(&func,this,std::placeholders::_1)
FGameObject::FGameObject(const char* name, EGameObjectType t, FScene*s)
	:Name(name),Type(t),Owner(s)
{
	assert(Owner);
	AddComponent<FTransformComponent>();
}

void FGameObject::Rename(const string& name)
{
	Name = name;
}

void FGameObject::SetParent(int Parent)
{
	ParentId = Parent;
}

void FGameObject::AddChild(uint32_t Child)
{
	Children.push_back(Child);
}

void FGameObject::RemoveChild(uint32_t Child)
{
	for (auto Iter = Children.begin();Iter != Children.end();++Iter)
	{
		if (*Iter == Child)
		{
			Children.erase(Iter);
			return;
		}
	}
}

//template<>
//bool FGameObject::RemoveComponent<FTransformComponent>()
//{
//	auto find = Components.find(EComponentType::Transform);
//	if (find != Components.end())
//	{
//		Components.erase(find);
//		return true;
//	}
//	return false;
//}
//
//template<>
//bool FGameObject::RemoveComponent<FScriptComponent>()
//{
//	auto find = Components.find(EComponentType::Script);
//	if (find != Components.end())
//	{
//		Components.erase(find);
//		return true;
//	}
//	return false;
//}

//template<>
//FCameraComponent* FGameObject::AddComponent<FCameraComponent>()
//{
//	FCameraComponent* Res = new FCameraComponent(this);
//	Components[EComponentType::Camera].reset(Res);
//	return Res;
//}

//template<>
//FCameraComponent*  FGameObject::GetComponent<FCameraComponent>()
//{
//	auto find = Components.find(EComponentType::Camera);
//	if (find != Components.end())
//	{
//		return (*find).second->DynamicCast<FCameraComponent>();
//	}
//	return nullptr;
//}

//template<>
//bool FGameObject::RemoveComponent<FCameraComponent>()
//{
//	auto find = Components.find(EComponentType::Camera);
//	if (find != Components.end())
//	{
//		Components.erase(find);
//		return true;
//	}
//	return false;
//}

bool FGameObject::HandleInputEvent(FEvent& e)
{
	FEventDispatcher Dispatcher(e);
	Dispatcher.Dispatch<FKeyPressedEvent>(BIND_EVENT_FUNCTION(FGameObject::OnKeyPressedEvent));
	Dispatcher.Dispatch<FKeyReleasedEvent>(BIND_EVENT_FUNCTION(FGameObject::OnKeyReleasedEvent));
	Dispatcher.Dispatch<FKeyTypedEvent>(BIND_EVENT_FUNCTION(FGameObject::OnKeyTypedEvent));

	Dispatcher.Dispatch<FMouseMoveEvent>(BIND_EVENT_FUNCTION(FGameObject::OnMouseMoveEvent));
	Dispatcher.Dispatch<FMouseScrolledEvent>(BIND_EVENT_FUNCTION(FGameObject::OnMouseScrolledEvent));
	Dispatcher.Dispatch<FMouseButtonPressedEvent>(BIND_EVENT_FUNCTION(FGameObject::OnMouseButtonPressedEvent));
	Dispatcher.Dispatch<FMouseButtonReleasedEvent>(BIND_EVENT_FUNCTION(FGameObject::OnMouseButtonReleasedEvent));
	return true;
}

bool FGameObject::BeginUpdate()
{
	return false;
}

bool FGameObject::Update()
{
	FScriptComponent* Script = GetComponent<FScriptComponent>();
	if (Script and Script->OnUpdate)
	{
		Script->OnUpdate(ImGui::GetIO().DeltaTime);
	}
	if (ParentId == -1)
	{
		FTransform Parent;
		FTransformComponent* TransformComponent = GetComponent<FTransformComponent>();
		if (TransformComponent)
		{
			TransformComponent->Update(Parent);
		}
	}
	return false;
}

bool FGameObject::EndUpdate()
{
	return false;
}

void FGameObject::Awake()
{
	FScriptComponent* Script = GetComponent<FScriptComponent>();
	if (Script and Script->OnAwake)
	{
		Script->OnAwake();
	}
}

void FGameObject::Release()
{
	FScriptComponent* Script = GetComponent<FScriptComponent>();
	if (Script and Script->OnRelease)
	{
		Script->OnRelease();
	}
}

bool FGameObject::OnKeyPressedEvent(FKeyPressedEvent& e)
{
	return false;
}

bool FGameObject::OnKeyReleasedEvent(FKeyReleasedEvent& e)
{
	return false;
}

bool FGameObject::OnKeyTypedEvent(FKeyTypedEvent& e)
{
	return false;
}

bool FGameObject::OnMouseMoveEvent(FMouseMoveEvent& e)
{
	return false;
}

bool FGameObject::OnMouseScrolledEvent(FMouseScrolledEvent& e)
{
	return false;
}

bool FGameObject::OnMouseButtonPressedEvent(FMouseButtonPressedEvent& e)
{
	return false;
}

bool FGameObject::OnMouseButtonReleasedEvent(FMouseButtonReleasedEvent& e)
{
	return false;
}
