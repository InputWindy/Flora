#include "DebugCamera.h"

DebugCamera::DebugCamera(const char* name, FScene* scene, uint32_t id)
	:FCameraObject(name, scene, id)
{
	FScriptComponent* Script = AddComponent<FScriptComponent>();
	Script->BindAwakeCallback(std::bind(&DebugCamera::CameraAwake,this));
	Script->BindUpdateCallback(std::bind(&DebugCamera::CameraUpdate, this,std::placeholders::_1));
	Script->BindReleaseCallback(std::bind(&DebugCamera::CameraRelease, this));
}