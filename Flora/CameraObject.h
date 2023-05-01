#pragma once
#include "GameObject.h"
class FLORA_API FCameraObject:public FGameObject
{
	friend class FScene;
public:
	FCameraObject(const char*, FScene*);
	virtual ~FCameraObject() = default;
protected:
	/// <summary>
	/// Input Event
	/// </summary>
	virtual bool OnKeyPressedEvent(FKeyPressedEvent& e) final;
	virtual bool OnKeyReleasedEvent(FKeyReleasedEvent& e) final;
	virtual bool OnKeyTypedEvent(FKeyTypedEvent& e) final;

	virtual bool OnMouseMoveEvent(FMouseMoveEvent& e) final;
	virtual bool OnMouseScrolledEvent(FMouseScrolledEvent& e) final;
	virtual bool OnMouseButtonPressedEvent(FMouseButtonPressedEvent& e) final;
	virtual bool OnMouseButtonReleasedEvent(FMouseButtonReleasedEvent& e) final;
protected:
	virtual bool BeginUpdate()final;
	virtual bool Update()final;
	virtual bool EndUpdate()final;
public:
	virtual bool Parse(IN FJson&) final;
	virtual bool Serialize(OUT FJson&) final;
};

