#pragma once
#include "Material.h"
class FLORA_API FOpenGLMaterial:public FMaterial
{
public:
	FOpenGLMaterial(const char*);
	virtual ~FOpenGLMaterial();
public:
	virtual void	 SetData(const string&, const string&) final;
	virtual uint32_t GetHandle()const final;
protected:
	uint32_t Handle = 0;
};

