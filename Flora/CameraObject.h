#pragma once
#include "GameObject.h"
class FLORA_API FCameraObject:public FGameObject
{
	friend class FScene;
public:
	FCameraObject(const char*, FScene*,uint32_t);
	virtual ~FCameraObject() = default;
public:
	virtual bool Parse(IN FJson&) ;
	virtual bool Serialize(OUT FJson&) ;
};

