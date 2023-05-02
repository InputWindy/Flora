#pragma once
#include "Flora.h"
struct DebugScene_0:public FLayer
{
	virtual void OnAwake() final;
	virtual void OnRelease() final;
	virtual void OnUpdate() final;
	virtual void OnGUI() final;
};

