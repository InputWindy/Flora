#include "Component.h"

FComponent::FComponent(FGameObject* o, EComponentType t)
	:Owner(o),Type(t)
{
}

FCameraComponent::FCameraComponent(FGameObject* o)
	:FComponent(o,EComponentType::Camera)
{
	SetCameraType(ECameraType::Perspective);
}

void FCameraComponent::SetCameraType(ECameraType t)
{
	switch (t)
	{
	case ECameraType::Perspective:Camera.reset(new FPerspectiveCamera); break;
	case ECameraType::Orthographic:Camera.reset(new FOrthographicCamera); break;
	default:assert(0, "Undefined Camera Type");break;
	}
	Camera->Update();
}

bool FCameraComponent::Parse(IN FJson&)
{
	return false;
}

bool FCameraComponent::Serialize(OUT FJson&)
{
	return false;
}

FTransformComponent::FTransformComponent(FGameObject*o)
	:FComponent(o, EComponentType::Transform)
{
}

void FTransformComponent::Update(const FTransform& Parent)
{
	//Transform.Update(Parent);
}

bool FTransformComponent::Parse(IN FJson&)
{
	return false;
}

bool FTransformComponent::Serialize(OUT FJson&)
{
	return false;
}




//template<>
//FTransformComponent* FComponent::DynamicCast<FTransformComponent>()
//{
//	return dynamic_cast<FTransformComponent*>(this);
//}
//
//template<>
//FCameraComponent* FComponent::DynamicCast<FCameraComponent>()
//{
//	return dynamic_cast<FCameraComponent*>(this);
//}
//
//template<>
//FScriptComponent* FComponent::DynamicCast<FScriptComponent>()
//{
//	return dynamic_cast<FScriptComponent*>(this);
//}

FScriptComponent::FScriptComponent(FGameObject*o)
	:FComponent(o, EComponentType::Script)
{
}

void FScriptComponent::BindAwakeCallback(OnAwakeCallback callback)
{
	OnAwake = callback;
}

void FScriptComponent::BindReleaseCallback(OnReleaseCallback callback)
{
	OnRelease = callback;
}

void FScriptComponent::BindUpdateCallback(OnUpdateCallback callback)
{
	OnUpdate = callback;
}

bool FScriptComponent::Parse(IN FJson&)
{
	return false;
}

bool FScriptComponent::Serialize(OUT FJson&)
{
	return false;
}

FAnimationComponent::FAnimationComponent(FGameObject*o)
	:FComponent(o, EComponentType::Animation)
{
}

void FAnimationComponent::AddAnimation()
{
	AnimationMap.insert({"Empty",nullptr});
}

void FAnimationComponent::ResetAnimation(const string& Name,Ref<FAnimation> Anim)
{
	if (AnimationMap.find(Name) != AnimationMap.end())
	{
		AnimationMap[Name] = Anim;
	}
}

Ref<FAnimation>& FAnimationComponent::operator[](const string& Name)
{
	return AnimationMap[Name];
}

bool FAnimationComponent::Parse(IN FJson&)
{
	return false;
}

bool FAnimationComponent::Serialize(OUT FJson&)
{
	return false;
}

FMeshComponent::FMeshComponent(FGameObject*o)
	:FComponent(o,EComponentType::Mesh)
{
}

Ref<FMesh>& FMeshComponent::operator[](uint32_t Idx)
{
	return Meshes[Idx];
}

bool FMeshComponent::Parse(IN FJson&)
{
	return false;
}

bool FMeshComponent::Serialize(OUT FJson&)
{
	return false;
}
