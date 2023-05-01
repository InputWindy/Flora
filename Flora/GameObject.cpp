#include "GameObject.h"
#include <functional>
#define BIND_EVENT_FUNCTION(func) std::bind(&func,this,std::placeholders::_1)
FGameObject::FGameObject(const char* name, EGameObjectType t, FScene*s)
	:Name(name),Type(t),Owner(s)
{
	assert(Owner);
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

template<>
FTransformComponent* FGameObject::AddComponent<FTransformComponent>()
{
	FTransformComponent* Res = new FTransformComponent(this);
	Components[EComponentType::Transform].reset(Res);
	return Res;
}

template<>
FTransformComponent* FGameObject::GetComponent<FTransformComponent>()
{
	auto find = Components.find(EComponentType::Transform);
	if (find != Components.end())
	{
		return (*find).second->DynamicCast<FTransformComponent>();
	}
	return nullptr;
}

template<>
bool FGameObject::RemoveComponent<FTransformComponent>()
{
	auto find = Components.find(EComponentType::Transform);
	if (find != Components.end())
	{
		Components.erase(find);
		return true;
	}
	return false;
}

template<>
FCameraComponent* FGameObject::AddComponent<FCameraComponent>()
{
	FCameraComponent* Res = new FCameraComponent(this);
	Components[EComponentType::Camera].reset(Res);
	return Res;
}

template<>
FCameraComponent* FGameObject::GetComponent<FCameraComponent>()
{
	auto find = Components.find(EComponentType::Camera);
	if (find != Components.end())
	{
		return (*find).second->DynamicCast<FCameraComponent>();
	}
	return nullptr;
}

template<>
bool FGameObject::RemoveComponent<FCameraComponent>()
{
	auto find = Components.find(EComponentType::Camera);
	if (find != Components.end())
	{
		Components.erase(find);
		return true;
	}
	return false;
}

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
