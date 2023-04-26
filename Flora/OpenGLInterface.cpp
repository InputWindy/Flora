#include "OpenGLInterface.h"
#include "OpenGLTexture.h"
#include "OpenGLMaterial.h"
#include "Console.h"
#include <glad.h>

using namespace glm;
uint32_t FOpenGLInterface::CompareMethod[] =
{
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS
};

uint32_t FOpenGLInterface::BlendFunc[] =
{
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT
};

uint32_t FOpenGLInterface::BlendFactor[] =
{
	GL_ZERO,GL_ONE,
	GL_SRC_COLOR,GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,GL_ONE_MINUS_DST_ALPHA,
	GL_CONSTANT_COLOR,GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,GL_ONE_MINUS_CONSTANT_ALPHA
};

uint32_t FOpenGLInterface::Operation[] =
{
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR,
	GL_INCR_WRAP,
	GL_DECR,
	GL_DECR_WRAP,
	GL_INVERT
};

uint32_t FOpenGLInterface::FrontFace[] =
{
	GL_CCW,GL_CW
};

uint32_t FOpenGLInterface::CullMode[] =
{
	GL_FRONT,GL_BACK,GL_FRONT_AND_BACK
};

uint32_t FOpenGLInterface::TextureTarget[] =
{
	GL_TEXTURE_1D,
	GL_TEXTURE_2D,
	GL_TEXTURE_2D_MULTISAMPLE,
	GL_TEXTURE_3D,
	GL_TEXTURE_2D_MULTISAMPLE_ARRAY ,
	GL_TEXTURE_CUBE_MAP,
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
};

uint32_t FOpenGLInterface::WrapMode[] =
{
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER
};

uint32_t FOpenGLInterface::FilterMode[] =
{
	GL_NEAREST,
	GL_LINEAR,
	GL_NEAREST_MIPMAP_NEAREST,
	GL_LINEAR_MIPMAP_NEAREST,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR
};

uint32_t FOpenGLInterface::InternalFormat[] =
{
	GL_DEPTH_COMPONENT,
	GL_RED,
	GL_DEPTH_STENCIL,
	GL_RG ,
	GL_RGB ,
	GL_RGBA ,
	GL_R8 ,
	GL_R16 ,
	GL_R16F ,
	GL_R32F ,
	GL_R8I ,
	GL_R16I ,
	GL_R32I,
	GL_R8UI,
	GL_R16UI,
	GL_R32UI,
	GL_RG8 ,
	GL_RG16 ,
	GL_RG16F  ,
	GL_RG32F  ,
	GL_RG8I   ,
	GL_RG16I  ,
	GL_RG32I  ,
	GL_RG8UI  ,
	GL_RG16UI ,
	GL_RG32UI ,
	GL_RGB32F ,
	GL_RGB32I ,
	GL_RGB32UI ,
	GL_RGBA8 ,
	GL_RGBA16 ,
	GL_RGBA16F,
	GL_RGBA32F,
	GL_RGB8I,
	GL_RGBA16I,
	GL_RGBA32I,
	GL_RGBA8UI,
	GL_RGBA16UI ,
	GL_RGBA32UI ,
};

uint32_t FOpenGLInterface::ColorAttachment[] =
{
		GL_COLOR_ATTACHMENT0 ,
		GL_COLOR_ATTACHMENT1 ,
		GL_COLOR_ATTACHMENT2 ,
		GL_COLOR_ATTACHMENT3 ,
		GL_COLOR_ATTACHMENT4 ,
		GL_COLOR_ATTACHMENT5 ,
		GL_COLOR_ATTACHMENT6 ,
		GL_COLOR_ATTACHMENT7 ,
		GL_COLOR_ATTACHMENT8 ,
		GL_COLOR_ATTACHMENT9 ,
		GL_COLOR_ATTACHMENT10,
		GL_COLOR_ATTACHMENT11,
		GL_COLOR_ATTACHMENT12,
		GL_COLOR_ATTACHMENT13,
		GL_COLOR_ATTACHMENT14,
		GL_COLOR_ATTACHMENT15,
		GL_COLOR_ATTACHMENT16,
		GL_COLOR_ATTACHMENT17,
		GL_COLOR_ATTACHMENT18,
		GL_COLOR_ATTACHMENT19,
		GL_COLOR_ATTACHMENT20,
		GL_COLOR_ATTACHMENT21,
		GL_COLOR_ATTACHMENT22,
		GL_COLOR_ATTACHMENT23,
		GL_COLOR_ATTACHMENT24,
		GL_COLOR_ATTACHMENT25,
		GL_COLOR_ATTACHMENT26,
		GL_COLOR_ATTACHMENT27,
		GL_COLOR_ATTACHMENT28,
		GL_COLOR_ATTACHMENT29,
		GL_COLOR_ATTACHMENT30,
		GL_COLOR_ATTACHMENT31
};


FOpenGLInterface::FOpenGLInterface()
{
	Type = E_RHI_OpenGL;
}

Ref<FTexture> FOpenGLInterface::GenerateTexture(IN const char* name, IN uint16_t w, IN uint16_t h, IN uint16_t z, IN ETextureTarget type, IN EInternalFormat inform, IN FTextureInfo info)
{
	return FOpenGLTexture::Generate(name,w,h,z,type,inform,info);
}

Ref<FTexture> FOpenGLInterface::GenerateTexture(IN FImage img, IN bool mutisample, IN FTextureInfo info)
{
	return FOpenGLTexture::Generate(img, mutisample, info);
}

Ref<FTexture> FOpenGLInterface::GenerateTexture(IN const std::string& root, IN const std::string& relative, IN bool b_hdr, IN bool b_flip, IN bool mutisample, IN FTextureInfo info)
{
	FImage img(root, relative);
	img.Load(b_hdr, b_flip);

	Ref<FTexture> Tex = FTexture::Generate(img, mutisample,info);

	img.Free();
	return Tex;
}


Ref<FMaterial> FOpenGLInterface::GenerateMaterial(IN const char* Name, IN const char* vshader, IN const char* fshader)
{
	return FOpenGLMaterial::Generate(Name, vshader, fshader);
}

void FOpenGLInterface::SetBool(const char* Name, uint32_t material, const bool& Data)
{
	glUniform1i(glGetUniformLocation(material, Name), (int)Data);
}

void FOpenGLInterface::SetInt(const char* Name, uint32_t material, const  int& Data)
{
	glUniform1i(glGetUniformLocation(material, Name), Data);
}

void FOpenGLInterface::SetInt2(const char* Name, uint32_t material, const ivec2& Data)
{
	glUniform2i(glGetUniformLocation(material, Name), Data.x, Data.y);
}

void FOpenGLInterface::SetInt3(const char* Name, uint32_t material, const  ivec3& Data)
{
	glUniform3i(glGetUniformLocation(material, Name), Data.x, Data.y, Data.z);
}

void FOpenGLInterface::SetInt4(const char* Name, uint32_t material, const  ivec4& Data)
{
	glUniform4i(glGetUniformLocation(material, Name), Data.x, Data.y, Data.z, Data.w);
}

void FOpenGLInterface::SetFloat(const char* Name, uint32_t material, const  float& Data)
{
	glUniform1f(glGetUniformLocation(material, Name), Data);
}

void FOpenGLInterface::SetFloat2(const char* Name, uint32_t material, const  vec2& Data)
{
	glUniform2f(glGetUniformLocation(material, Name), Data.x, Data.y);
}

void FOpenGLInterface::SetFloat3(const char* Name, uint32_t material, const  vec3& Data)
{
	glUniform3f(glGetUniformLocation(material, Name), Data.x, Data.y, Data.z);
}

void FOpenGLInterface::SetFloat4(const char* Name, uint32_t material, const  vec4& Data)
{
	glUniform4f(glGetUniformLocation(material, Name), Data.x, Data.y, Data.z, Data.w);
}

void FOpenGLInterface::SetDouble(const char* Name, uint32_t material, const  double& Data)
{
	glUniform1d(glGetUniformLocation(material, Name), Data);
}

void FOpenGLInterface::SetDouble2(const char* Name, uint32_t material, const  dvec2& Data)
{
	glUniform2d(glGetUniformLocation(material, Name), Data.x, Data.y);
}

void FOpenGLInterface::SetDouble3(const char* Name, uint32_t material, const  dvec3& Data)
{
	glUniform3d(glGetUniformLocation(material, Name), Data.x, Data.y, Data.z);
}

void FOpenGLInterface::SetDouble4(const char* Name, uint32_t material, const dvec4& Data)
{
	glUniform4d(glGetUniformLocation(material, Name), Data.x, Data.y, Data.z, Data.w);
}


void FOpenGLInterface::SetMat2f(const char* Name, uint32_t material, uint32_t cnt, const vector<mat2>& Data)
{
	glUniformMatrix2fv(glGetUniformLocation(material, Name), cnt, GL_FALSE, value_ptr(Data[0]));
}

void FOpenGLInterface::SetMat3f(const char* Name, uint32_t material, uint32_t cnt, const vector<mat3>& Data)
{
	glUniformMatrix3fv(glGetUniformLocation(material, Name), cnt, GL_FALSE, value_ptr(Data[0]));
}

void FOpenGLInterface::SetMat4f(const char* Name, uint32_t material, uint32_t cnt, const vector<mat4>& Data)
{
	glUniformMatrix4fv(glGetUniformLocation(material, Name), cnt, GL_FALSE, value_ptr(Data[0]));
}

void FOpenGLInterface::SetRenderState(const FRenderState& RenderState)
{/*
		set render state!
	*/
	if (RenderState.bEnableDepth) {
		glEnable(GL_DEPTH_TEST);
		if (RenderState.bDepthReadOnly) {
			glDepthMask(GL_FALSE);
		}
		else
		{
			glDepthMask(GL_TRUE);
		}
		glDepthFunc(CompareMethod[RenderState.mCompareMethod]);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	if (RenderState.bEnableBlend) {
		glEnable(GL_BLEND);
		glBlendEquation(BlendFunc[RenderState.mBlendFuncMode]);
		glBlendFunc(BlendFactor[RenderState.mBlendSrc], BlendFactor[RenderState.mBlendDest]);
	}
	else
	{
		glDisable(GL_BLEND);
	}
	if (RenderState.bEnableStencil) {
		glEnable(GL_STENCIL);
		if (RenderState.bStencilReadOnly) {
			glStencilFunc(CompareMethod[RenderState.mCompareMethod], RenderState.mStencilRef, 0x00);
		}
		else {
			glStencilFunc(CompareMethod[RenderState.mCompareMethod], RenderState.mStencilRef, 0xFF);
		}

		glStencilOp(Operation[RenderState.mSFailOp], Operation[RenderState.mSPassZFailOp], Operation[RenderState.mPassOp]);
	}
	else
	{
		glDisable(GL_STENCIL);
	}

	if (RenderState.bEnableCull)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(CullMode[RenderState.mCullMode]);
		glFrontFace(FrontFace[RenderState.mFrontDefinition]);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void FOpenGLInterface::Flush()
{
	glFlush();
}

void FOpenGLInterface::Finish()
{
	glFinish();
}

void FOpenGLInterface::DrawElements(size_t Num, EDrawMode DrawMode)
{
	glDrawElements((GLenum)DrawMode, Num, GL_UNSIGNED_INT, 0);
}

void FOpenGLInterface::DrawElementsInstanced(size_t Num, int Cnt, EDrawMode DrawMode)
{
	glDrawElementsInstanced((GLenum)DrawMode, Num, GL_UNSIGNED_INT, 0, Cnt);
}

void FOpenGLInterface::BindDrawData(uint32_t vao, uint32_t ibo)
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

bool FOpenGLInterface::CompileShader(uint32_t Handle)
{
	auto CheckShader = [](uint32_t Handle)->bool
	{
		GLint success;
		GLchar infoLog[1024];
		glGetShaderiv(Handle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(Handle, 1024, NULL, infoLog);

			std::string Comment = "ERROR::SHADER_COMPILATION_ERROR:\t\t";
			Comment += infoLog;
			PUSH_ERROR_MSG("Material", Comment.c_str());

			return false;
		}
		return true;
	};
	return false;
}

bool FOpenGLInterface::LinkShader(uint32_t Handle)
{
	return false;
}

const char* FOpenGLInterface::CompareMethodToString(uint32_t t)
{
	switch (t)
	{
	case 0:return "NEVER";
	case 1:return "LESS";
	case 2:return "EQUAL";
	case 3:return "LEQUAL";
	case 4:return "GREATER";
	case 5:return "NOTEQUAL";
	case 6:return "GEQUAL";
	case 7:return "ALWAYS";
	default:
		break;
	}
	return nullptr;
}

const char* FOpenGLInterface::BlendFuncToString(uint32_t t)
{
	switch (t)
	{
	case 0:return "ADD";
	case 1:return "SUBTRACT";
	case 2:return "REVERSE_SUBTRACT";
	default:
		break;
	}
	return nullptr;
}

const char* FOpenGLInterface::BlendFactorToString(uint32_t t)
{
	switch (t)
	{
	case 0: return "ZERO";
	case 1: return "ONE";
	case 2: return "SRC_COLOR ";
	case 3: return "ONE_MINUS_SRC_COLOR";
	case 4: return "DST_COLOR";
	case 5: return "ONE_MINUS_DST_COLOR";
	case 6: return "SRC_ALPHA";
	case 7: return "ONE_MINUS_SRC_ALPHA";
	case 8: return "DST_ALPHA";
	case 9: return "ONE_MINUS_DST_ALPHA";
	case 10:return "CONSTANT_COLOR";
	case 11:return "ONE_MINUS_CONSTANT_COLOR";
	case 12:return "CONSTANT_ALPHA";
	case 13:return "ONE_MINUS_CONSTANT_ALPHA";
	default:
		break;
	}
	return nullptr;
}

const char* FOpenGLInterface::OperationToString(uint32_t t)
{
	switch (t)
	{
	case 0: return "KEEP";
	case 1: return "ZERO";
	case 2: return "REPLACE";
	case 3: return "INCR";
	case 4: return "INCR_WRAP";
	case 5: return "DECR";
	case 6: return "DECR_WRAP";
	case 7: return "INVER";
	default:
		break;
	}
	return nullptr;
}

const char* FOpenGLInterface::FrontFaceToString(uint32_t t)
{
	switch (t)
	{
	case 0: return "CCW";
	case 1: return "CW";
	default:
		break;
	}
	return nullptr;
}

const char* FOpenGLInterface::CullModeToString(uint32_t t)
{
	switch (t)
	{
	case 0:return "FRONT";
	case 1:return "BACK";
	case 2:return "FRONT_AND_BAC";
	default:
		break;
	}
	return nullptr;
}

const char* FOpenGLInterface::TextureTargetToString(uint32_t t)
{
	switch (t)
	{
	case 0:  return "1D";
	case 1:  return "2D";
	case 2:  return "2D_MULTISAMPLE";
	case 3:  return "3D";
	case 4:  return "2D_MULTISAMPLE_ARRAY";
	case 5:  return "CUBE_MAP";
	case 6:  return "CUBE_MAP_POSITIVE_X";
	case 7:  return "CUBE_MAP_NEGATIVE_X";
	case 8:  return "CUBE_MAP_POSITIVE_Y";
	case 9:  return "CUBE_MAP_NEGATIVE_Y";
	case 10: return "CUBE_MAP_POSITIVE_Z";
	case 11: return "CUBE_MAP_NEGATIVE_Z";
	default:
		break;
	}
	return nullptr;
}

const char* FOpenGLInterface::WrapModeToString(uint32_t t)
{
	switch (t)
	{
	case 0:  return "REPEAT";
	case 1:  return "MIRRORED_REPEAT";
	case 2:  return "CLAMP_TO_EDGE";
	case 3:  return "CLAMP_TO_BORDE";
	default:
		break;
	}
	return nullptr;
}

const char* FOpenGLInterface::FilterModeToString(uint32_t t)
{
	switch (t)
	{
	case 0:  return "NEAREST";
	case 1:  return "LINEAR";
	case 2:  return "NEAREST_MIPMAP_NEAREST";
	case 3:  return "LINEAR_MIPMAP_NEAREST";
	case 4:  return "NEAREST_MIPMAP_LINEAR";
	case 5:  return "LINEAR_MIPMAP_LINEA";
	default:
		break;
	}
	return nullptr;
}

const char* FOpenGLInterface::InternalFormatToString(uint32_t t)
{
	switch (t)
	{
	case 0:   return "DEPTH_COMPONENT";
	case 1:   return "RED";
	case 2:   return "DEPTH_STENCIL";
	case 3:   return "RG";
	case 4:   return "RGB";
	case 5:   return "RGBA";
	case 6:   return "R8";
	case 7:   return "R16";
	case 8:   return "R16F";
	case 9:   return "R32F";
	case 10:  return "R8I";
	case 11:  return "R16I";
	case 12:  return "R32I";
	case 13:  return "R8UI";
	case 14:  return "R16UI";
	case 15:  return "R32UI";
	case 16:  return "RG8";
	case 17:  return "RG16";
	case 18:  return "RG16F";
	case 19:  return "RG32F";
	case 20:  return "RG8I";
	case 21:  return "RG16I";
	case 22:  return "RG32I";
	case 23:  return "RG8UI";
	case 24:  return "RG16UI";
	case 25:  return "RG32UI";
	case 26:  return "RGB32F";
	case 27:  return "RGB32I";
	case 28:  return "RGB32UI";
	case 29:  return "RGBA8";
	case 30:  return "RGBA16";
	case 31:  return "RGBA16F";
	case 32:  return "RGBA32F";
	case 33:  return "RGB8I";
	case 34:  return "RGBA16I";
	case 35:  return "RGBA32I";
	case 36:  return "RGBA8UI";
	case 37:  return "RGBA16UI";
	case 38:  return "RGBA32UI";
	default:
		break;
	}
	return nullptr;
}

uint32_t FOpenGLInterface::StringToCompareMethod(const char*s)
{
	string ss = s;
	if (ss == "NEVER")return 0;
	if (ss == "LESS")return 1;
	if (ss == "EQUAL")return 2;
	if (ss == "LEQUAL")return 3;
	if (ss == "GREATER")return 4;
	if (ss == "NOTEQUAL")return 5;
	if (ss == "GEQUAL")return 6;
	if (ss == "ALWAYS")return 7;
	else return 3;
}

uint32_t FOpenGLInterface::StringToBlendFunc(const char*s)
{
	string ss = s;
	if (ss == "ADD")return 0;
	if (ss == "SUBTRACT")return 1;
	if (ss == "REVERSE_SUBTRACT")return 2;
	else return 0;
}

uint32_t FOpenGLInterface::StringToBlendFactor(const char*s)
{
	string ss = s;
	if (ss == "ZERO")return 0;
	if (ss == "ONE")return 1;
	if (ss == "SRC_COLOR ")return 2;
	if (ss == "ONE_MINUS_SRC_COLOR")return 3;
	if (ss == "DST_COLOR")return 4;
	if (ss == "ONE_MINUS_DST_COLOR")return 5;
	if (ss == "SRC_ALPHA")return 6;
	if (ss == "ONE_MINUS_SRC_ALPHA")return 7;
	if (ss == "DST_ALPHA")return 8;
	if (ss == "ONE_MINUS_DST_ALPHA")return 9;
	if (ss == "CONSTANT_COLOR")return 10;
	if (ss == "ONE_MINUS_CONSTANT_COLOR")return 11;
	if (ss == "CONSTANT_ALPHA")return 12;
	if (ss == "ONE_MINUS_CONSTANT_ALPHA")return 13;
	else return 2;
}

uint32_t FOpenGLInterface::StringToOperation(const char*s)
{
	string ss = s;
	if (ss == "KEEP")return 0;
	if (ss == "ZERO")return 1;
	if (ss == "REPLACE")return 2;
	if (ss == "INCR")return 3;
	if (ss == "INCR_WRAP")return 4;
	if (ss == "DECR")return 5;
	if (ss == "DECR_WRAP")return 6;
	if (ss == "INVER")return 7;
	else return 0;
}

uint32_t FOpenGLInterface::StringToFrontFace(const char*s)
{
	string ss = s;
	if (ss == "CCW")return 0;
	if (ss == "CW")return 1;
	else return 0;
}

uint32_t FOpenGLInterface::StringToCullMode(const char*s)
{
	string ss = s;
	if (ss == "FRONT")return 0;
	if (ss == "BACK")return 1;
	if (ss == "FRONT_AND_BAC")return 2;
	else return 1;
}

uint32_t FOpenGLInterface::StringToTextureTarget(const char*s)
{
	string ss = s;
	if (ss == "1D")return 0;
	if (ss == "2D")return 1;
	if (ss == "2D_MULTISAMPLE")return 2;
	if (ss == "3D")return 3;
	if (ss == "2D_MULTISAMPLE_ARRAY")return 4;
	if (ss == "CUBE_MAP")return 5;
	if (ss == "CUBE_MAP_POSITIVE_X")return 6;
	if (ss == "CUBE_MAP_NEGATIVE_X")return 7;
	if (ss == "CUBE_MAP_POSITIVE_Y")return 8;
	if (ss == "CUBE_MAP_NEGATIVE_Y")return 9;
	if (ss == "CUBE_MAP_POSITIVE_Z")return 10;
	if (ss == "CUBE_MAP_NEGATIVE_Z")return 11;
	else return 1;
}

uint32_t FOpenGLInterface::StringToWrapMode(const char*s)
{
	string ss = s;
	if (ss == "REPEAT")return 0;
	if (ss == "MIRRORED_REPEAT")return 1;
	if (ss == "CLAMP_TO_EDGE")return 2;
	if (ss == "CLAMP_TO_BORDE")return 3;
	else return 0;
}

uint32_t FOpenGLInterface::StringToFilterMode(const char*s)
{
	string ss = s;
	if (ss == "NEAREST")return 0;
	if (ss == "LINEAR")return 1;
	if (ss == "NEAREST_MIPMAP_NEAREST")return 2;
	if (ss == "LINEAR_MIPMAP_NEAREST")return 3;
	if (ss == "NEAREST_MIPMAP_LINEAR")return 4;
	if (ss == "LINEAR_MIPMAP_LINEA")return 5;
	else return 1;
}

uint32_t FOpenGLInterface::StringToInternalFormat(const char*s)
{
	string ss = s;
	if (ss == "DEPTH_COMPONENT")return 0;
	if (ss == "RED")return 1;
	if (ss == "DEPTH_STENCIL")return 2;
	if (ss == "RG")return 3;
	if (ss == "RGB")return 4;
	if (ss == "RGBA")return 5;
	if (ss == "R8")return 6;
	if (ss == "R16")return 7;
	if (ss == "R16F")return 8;
	if (ss == "R32F")return 9;
	if (ss == "R8I")return 10;
	if (ss == "R16I")return 11;
	if (ss == "R32I")return 12;
	if (ss == "R8UI")return 13;
	if (ss == "R16UI")return 14;
	if (ss == "R32UI")return 15;
	if (ss == "RG8")return 16;
	if (ss == "RG16")return 17;
	if (ss == "RG16F")return 18;
	if (ss == "RG32F")return 19;
	if (ss == "RG8I")return 20;
	if (ss == "RG16I")return 21;
	if (ss == "RG32I")return 22;
	if (ss == "RG8UI")return 23;
	if (ss == "RG16UI")return 24;
	if (ss == "RG32UI")return 25;
	if (ss == "RGB32F")return 26;
	if (ss == "RGB32I")return 27;
	if (ss == "RGB32UI")return 28;
	if (ss == "RGBA8")return 29;
	if (ss == "RGBA16")return 30;
	if (ss == "RGBA16F")return 31;
	if (ss == "RGBA32F")return 32;
	if (ss == "RGB8I")return 33;
	if (ss == "RGBA16I")return 34;
	if (ss == "RGBA32I")return 35;
	if (ss == "RGBA8UI")return 36;
	if (ss == "RGBA16UI")return 37;
	if (ss == "RGBA32UI")return 38;
	else return 0;
}
