#pragma once
#include "Core.h"
class FLORA_API FSceneManager
{
protected:
	FSceneManager() = default;
public:
	virtual ~FSceneManager() = default;

	static inline FSceneManager& Get() { static FSceneManager msSceneManager; return msSceneManager; };
public:
	void Update();
};

