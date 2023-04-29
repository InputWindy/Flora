#pragma once
#include "Mesh.h"
class FLORA_API FOpenGLMesh:public FMesh
{
public:
	FOpenGLMesh(const char*);
	~FOpenGLMesh();
public:
	virtual void SetData(const std::vector<FVertex>&, const std::vector<uint32_t>&, const FAABB&) final;
private:
	uint32_t Vao = 0;
	uint32_t Vbo = 0;
	uint32_t Ibo = 0;
};

