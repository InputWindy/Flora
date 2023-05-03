#pragma once
#include "Component.h"
#include "Event.h"
#include <unordered_map>
using std::string;
using std::vector;
using std::unordered_map;
typedef uint32_t EGameObjectType;
enum 
{
	EGameObjectType_None,
	EGameObjectType_CameraObject,
	EGameObjectType_Actor,
	EGameObjectType_Pawn,
	EGameObjectType_Character
};

class FScene;
class FLORA_API FGameObject:public ISerialization
{
	friend class FScene;
protected:
	FGameObject(const char*,EGameObjectType,FScene*, uint32_t);
public:
	virtual ~FGameObject() = default;

	template<typename T>
	inline T* AddComponent()
	{
		if (std::is_same_v<T, FCameraComponent>)
		{
			T* Res = new T(this);
			Components[EComponentType::Camera].reset(Res);
			return Res;
		}
		else if (std::is_same_v<T, FTransformComponent>)
		{
			T* Res = new T(this);
			Components[EComponentType::Transform].reset(Res);
			return Res;
		}
		else if (std::is_same_v<T, FScriptComponent>)
		{
			T* Res = new T(this);
			Components[EComponentType::Script].reset(Res);
			return Res;
		}
		else if (std::is_same_v<T, FAnimationComponent>)
		{
			T* Res = new T(this);
			Components[EComponentType::Animation].reset(Res);
			return Res;
		}
		else if (std::is_same_v<T, FMeshComponent>)
		{
			T* Res = new T(this);
			Components[EComponentType::Mesh].reset(Res);
			return Res;
		}
		else assert(0);
		return nullptr;
	};

	template<typename T>
	inline T* GetComponent()
	{
		if (std::is_same_v<T, FCameraComponent>)
		{
			auto find = Components.find(EComponentType::Camera);
			if (find != Components.end())
			{
				return (*find).second->DynamicCast<T>();
			}
		}
		else if (std::is_same_v<T, FTransformComponent>)
		{
			auto find = Components.find(EComponentType::Transform);
			if (find != Components.end())
			{
				return (*find).second->DynamicCast<T>();
			}
		}
		else if (std::is_same_v<T, FScriptComponent>)
		{
			auto find = Components.find(EComponentType::Script);
			if (find != Components.end())
			{
				return (*find).second->DynamicCast<T>();
			}
		}
		else if (std::is_same_v<T, FAnimationComponent>)
		{
			auto find = Components.find(EComponentType::Animation);
			if (find != Components.end())
			{
				return (*find).second->DynamicCast<T>();
			}
		}
		else if (std::is_same_v<T, FMeshComponent>)
		{
			auto find = Components.find(EComponentType::Mesh);
			if (find != Components.end())
			{
				return (*find).second->DynamicCast<T>();
			}
		}
		else assert(0);
		
		return nullptr;
	};

	template<typename T>
	inline bool RemoveComponent()
	{
		if (std::is_same_v<T, FCameraComponent>)
		{
			auto find = Components.find(EComponentType::Camera);
			if (find != Components.end())
			{
				Components.erase(find);
				return true;
			}
		}
		else if (std::is_same_v<T, FTransformComponent>)
		{
			auto find = Components.find(EComponentType::Transform);
			if (find != Components.end())
			{
				Components.erase(find);
				return true;
			}
		}
		else if (std::is_same_v<T, FScriptComponent>)
		{
			auto find = Components.find(EComponentType::Script);
			if (find != Components.end())
			{
				Components.erase(find);
				return true;
			}
		}
		else if (std::is_same_v<T, FAnimationComponent>)
		{
			auto find = Components.find(EComponentType::Animation);
			if (find != Components.end())
			{
				Components.erase(find);
				return true;
			}
		}
		else if (std::is_same_v<T, FMeshComponent>)
		{
			auto find = Components.find(EComponentType::Mesh);
			if (find != Components.end())
			{
				Components.erase(find);
				return true;
			}
		}
		else assert(0);
		
		return false;
	};
public:
	inline const int&  GetParent() { return ParentId; };
	inline const auto* GetOwner()  { return Owner; };
	inline const auto& GetName()   { return Name; };
	inline const auto& GetType()   { return Type; };
	inline const auto& GetId()	   { return Id; };
protected:
	/// <summary>
	/// Input Event
	/// </summary>
	virtual bool OnKeyPressedEvent(FKeyPressedEvent& e) ;
	virtual bool OnKeyReleasedEvent(FKeyReleasedEvent& e) ;
	virtual bool OnKeyTypedEvent(FKeyTypedEvent& e) ;

	virtual bool OnMouseMoveEvent(FMouseMoveEvent& e) ;
	virtual bool OnMouseScrolledEvent(FMouseScrolledEvent& e) ;
	virtual bool OnMouseButtonPressedEvent(FMouseButtonPressedEvent& e) ;
	virtual bool OnMouseButtonReleasedEvent(FMouseButtonReleasedEvent& e) ;
protected:
	virtual bool BeginUpdate();
	virtual bool Update();
	virtual bool EndUpdate();

	virtual void Awake();
	virtual void Release();
public:
	virtual bool Parse(IN FJson&) = 0;
	virtual bool Serialize(OUT FJson&) = 0;
protected:
	string Name;
	EGameObjectType Type = EGameObjectType_None;
	unordered_map < EComponentType, Ref<FComponent>> Components;

	uint32_t Id = 0;
	int ParentId = -1;
	vector<uint32_t> Children;

	FScene* Owner = nullptr;
private:
	bool HandleInputEvent(FEvent&);
	void AddChild(uint32_t);
	void RemoveChild(uint32_t);
	void Rename(const string&);
	void SetParent(int);
};


