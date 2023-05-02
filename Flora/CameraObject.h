#pragma once
#include "GameObject.h"
class FLORA_API FCameraObject:public FGameObject
{
	friend class FScene;
public:
	FCameraObject(const char*, FScene*);
	virtual ~FCameraObject() = default;
public:
	virtual bool Parse(IN FJson&) ;
	virtual bool Serialize(OUT FJson&) ;
protected:
	float MovementSpeed = 2.5f;
	float MouseSensitivity = 0.1f;
	float MouseWheelSensitivity = 0.1f;
	//float Zoom = 45.0f;
public:
	inline float GetMovementSpeed()const { return MovementSpeed; };
	inline float GetMouseSensitivity()const { return MouseSensitivity; };
	inline float GetMouseWheelSensitivity()const { return MouseWheelSensitivity; };
};

