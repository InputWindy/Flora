#include "OpenGLMaterial.h"
#include "Application.h"
#include <glad.h>
FOpenGLMaterial::~FOpenGLMaterial()
{
	glDeleteProgram(Handle);
}
Ref<FOpenGLMaterial> FOpenGLMaterial::Generate()
{
	return make_shared<FOpenGLMaterial>();
}
Ref<FOpenGLMaterial> FOpenGLMaterial::Generate(const char* Name, const char* VShader, const char* FShader)
{
    if (!Name)return nullptr;

	FRenderCommand* Rhi = FApplication::GetRHI();

	Ref<FOpenGLMaterial> Res = make_shared<FOpenGLMaterial>();
	Res->MaterialName = Name;
	Res->VertShader = VShader;
	Res->FragShader = FShader;

	Res->CachePath = "/Cache/Material/" + Res->MaterialName + ".fmaterial";

	std::vector<uint32_t> ShaderHandles;
	{
		uint32_t ShaderHandle = glCreateShader(GL_VERTEX_SHADER);
		const char* code = Res->VertShader.c_str();
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
		const char* code = Res->FragShader.c_str();
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

	Res->Handle = glCreateProgram();
	
	for (auto id : ShaderHandles)glAttachShader(Res->Handle, id);
	
	if (Rhi->LinkShader(Res->Handle))Res->GenerateUniforms(); 
	else Res = nullptr;
	
	for (auto id : ShaderHandles)glDeleteShader(id);
	return Res;
}

uint32_t FOpenGLMaterial::GetHandle()const
{
	return Handle;
}

void FOpenGLMaterial::Reload()
{
	FRenderCommand* Rhi = FApplication::GetRHI();

	std::vector<uint32_t> ShaderHandles;
	{
		uint32_t ShaderHandle = glCreateShader(GL_VERTEX_SHADER);
		const char* code = VertShader.c_str();
		glShaderSource(ShaderHandle, 1, &code, NULL);
		glCompileShader(ShaderHandle);

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
		glCompileShader(ShaderHandle);

		if (!Rhi->CompileShader(ShaderHandle))
		{
			glDeleteShader(ShaderHandle);
		}
		else
		{
			ShaderHandles.push_back(ShaderHandle);
		}
	};

	glDeleteProgram(Handle);
	Handle = glCreateProgram();

	for (auto id : ShaderHandles)glAttachShader(Handle, id);

	bool bFlag = Rhi->LinkShader(Handle);

	for (auto id : ShaderHandles)glDeleteShader(id);

	GenerateUniforms();
}
