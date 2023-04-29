#include "OpenGLMaterial.h"
#include "Application.h"
#include <glad.h>
FOpenGLMaterial::FOpenGLMaterial(const char* name)
{
	Rename(name);
	Handle = glCreateProgram();
}
FOpenGLMaterial::~FOpenGLMaterial()
{
	glDeleteProgram(Handle);
}

void FOpenGLMaterial::SetData(const string& vshader, const string& fshader)
{
	FRenderCommand* Rhi = FApplication::GetRHI();

	VertShader = vshader;
	FragShader = fshader;
	std::vector<uint32_t> ShaderHandles;
	{
		uint32_t ShaderHandle = glCreateShader(GL_VERTEX_SHADER);
		const char* code = VertShader.c_str();
		glShaderSource(ShaderHandle, 1, &code, NULL);

		if (!Rhi->CompileShader(ShaderHandle))
		{
			glDeleteShader(ShaderHandle);
		}
		else
		{
			ShaderHandles.push_back(ShaderHandle);
		}
	};
	{
		uint32_t ShaderHandle = glCreateShader(GL_FRAGMENT_SHADER);
		const char* code = FragShader.c_str();
		glShaderSource(ShaderHandle, 1, &code, NULL);

		if (!Rhi->CompileShader(ShaderHandle))
		{
			glDeleteShader(ShaderHandle);
		}
		else
		{
			ShaderHandles.push_back(ShaderHandle);
		}
	};

	for (auto id : ShaderHandles)glAttachShader(Handle, id);

	if (Rhi->LinkShader(Handle))GenerateUniforms();

	for (auto id : ShaderHandles)glDeleteShader(id);
}

uint32_t FOpenGLMaterial::GetHandle()const
{
	return Handle;
}
