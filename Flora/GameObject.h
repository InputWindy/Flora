#pragma once
#include "Component.h"
#include "Event.h"
#include <unordered_map>
using std::string;
using std::vector;
using std::unordered_map;
enum class EGameObjectType
{
	None,
	CameraObject
};

class FScene;
class FLORA_API FGameObject:public ISerialization
{
	friend class FScene;
protected:
	FGameObject(const char*,EGameObjectType,FScene*);
public:
	virtual ~FGameObject() = default;

	template<typename T>
	T* AddComponent();

	template<typename T>
	T* GetComponent();

	template<typename T>
	bool RemoveComponent();
public:
	inline const int&  GetParent() { return ParentId; };
	inline const auto* GetOwner()  { return Owner; };
	inline const auto& GetName()   { return Name; };
	inline const auto& GetType()   { return Type; };
protected:
	/// <summary>
	/// Input Event
	/// </summary>
	virtual bool OnKeyPressedEvent(FKeyPressedEvent& e) = 0;
	virtual bool OnKeyReleasedEvent(FKeyReleasedEvent& e) = 0;
	virtual bool OnKeyTypedEvent(FKeyTypedEvent& e) = 0;

	virtual bool OnMouseMoveEvent(FMouseMoveEvent& e) = 0;
	virtual bool OnMouseScrolledEvent(FMouseScrolledEvent& e) = 0;
	virtual bool OnMouseButtonPressedEvent(FMouseButtonPressedEvent& e) = 0;
	virtual bool OnMouseButtonReleasedEvent(FMouseButtonReleasedEvent& e) = 0;

	virtual bool BeginUpdate() = 0;
	virtual bool Update() = 0;
	virtual bool EndUpdate() = 0;
public:
	virtual bool Parse(IN FJson&) = 0;
	virtual bool Serialize(OUT FJson&) = 0;
protected:
	string Name;
	EGameObjectType Type = EGameObjectType::None;
	unordered_map < EComponentType, Ref<FComponent>> Components;
private:
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


