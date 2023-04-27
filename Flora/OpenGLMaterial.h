#pragma once
#include "Material.h"
class FLORA_API FOpenGLMaterial:public FMaterial
{
public:
	FOpenGLMaterial()= default;
public:
	virtual ~FOpenGLMaterial();
	static Ref<FOpenGLMaterial> Generate();
	static Ref<FOpenGLMaterial> Generate(const char*, const char* = "", const char* = "");
public:
	virtual uint32_t GetHandle()const final;

	virtual void Reload() final;
protected:
	uint32_t Handle = 0;
};

