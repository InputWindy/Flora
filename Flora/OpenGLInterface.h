#pragma once
#include "Core.h"
#include "Type.h"
#include "RenderCommand.h"
class FLORA_API FOpenGLInterface:public FRenderCommand
{
	friend class FOpenGLTexture;
public:
	FOpenGLInterface();
	virtual ~FOpenGLInterface() = default;
public:
	virtual bool InitResources() final;
public:
	/// <summary>
	/// apply for an empty texture
	/// </summary>
	virtual Ref<FTexture> GenerateTexture(
		IN const char*,		/*texture name*/								
		IN uint16_t,		/*texture width*/								
		IN uint16_t,		/*texture height*/							
		IN uint16_t,		/*texture depth(for 3D texture)*/				
		IN ETextureTarget,	/*texture type*/								
		IN EInternalFormat,	/*texture internal format(default rgba32f)*/	 
		IN FTextureInfo		/*texture infos*/								
	)final;
	/// <summary>
	/// apply for a texture with raw data from disk
	/// </summary>
	virtual Ref<FTexture> GenerateTexture(
		IN FImage,							 /*img data loaded from disk*/
		IN bool mutisample = false,			 /*enable msaa*/
		IN FTextureInfo info = FTextureInfo()/*texture infos*/
	)final;
	/// <summary>
	/// apply for a texture with img path
	/// </summary>
	virtual Ref<FTexture> GenerateTexture(
		IN const std::string& root,			 /*asset root path*/
		IN const std::string& relative,		 /*path*/
		IN bool b_hdr = false,				 /*loaded as float raw data*/
		IN bool b_flip = false,				 /*flip up and down*/
		IN bool mutisample = false,			 /*enable msaa*/
		IN FTextureInfo info = FTextureInfo()/*texture infos*/
	)final;
	/// <summary>
	/// apply for a new material
	/// </summary>
	virtual Ref<FMaterial> GenerateMaterial(
		IN const char* Name,/*material name*/
		IN const char* VertexShader = "",/*vshader(default empty)*/
		IN const char* FragmentShader = ""/*fshader(default empty)*/
	)final;

	/// <summary>
	/// apply for a new material(empty)
	/// </summary>
	virtual Ref<FMaterial> GenerateMaterial()final;
public:
	virtual void SetBool(const char*, uint32_t, const bool&)override;
	virtual void SetInt(const char*, uint32_t, const int&)override;
	virtual void SetInt2(const char*, uint32_t, const glm::ivec2&)override;
	virtual void SetInt3(const char*, uint32_t, const glm::ivec3&)override;
	virtual void SetInt4(const char*, uint32_t, const glm::ivec4&)override;
	virtual void SetFloat(const char*, uint32_t, const float&)override;
	virtual void SetFloat2(const char*, uint32_t, const glm::vec2&)override;
	virtual void SetFloat3(const char*, uint32_t, const glm::vec3&)override;
	virtual void SetFloat4(const char*, uint32_t, const glm::vec4&)override;
	virtual void SetDouble(const char*, uint32_t, const double&)override;
	virtual void SetDouble2(const char*, uint32_t, const  glm::dvec2&)override;
	virtual void SetDouble3(const char*, uint32_t, const  glm::dvec3&)override;
	virtual void SetDouble4(const char*, uint32_t, const  glm::dvec4&)override;
	virtual void SetMat2f(const char*, uint32_t, uint32_t, const vector<glm::mat2>&)override;
	virtual void SetMat3f(const char*, uint32_t, uint32_t, const vector<glm::mat3>&)override;
	virtual void SetMat4f(const char*, uint32_t, uint32_t, const vector<glm::mat4>&)override;

	virtual void SetRenderState(const FRenderState&)override;
public:
	//======================//
	//     OpenGL Command   //
	//======================//
	virtual void Flush()  override; //flush opengl command queue
	virtual void Finish() override;//flush opengl command queue and wait for render finish

	virtual void DrawElements(size_t, EDrawMode = EDrawMode_TRIANGLES)override;
	virtual void DrawElementsInstanced(size_t, int, EDrawMode = EDrawMode_TRIANGLES)override;

	virtual void BindDrawData(uint32_t, uint32_t)override;//vao + ibo

	virtual bool CompileShader(uint32_t)override;
	virtual bool LinkShader(uint32_t)override;
private:
	static uint32_t CompareMethod[];
	static uint32_t BlendFunc[];
	static uint32_t BlendFactor[];
	static uint32_t Operation[];
	static uint32_t FrontFace[];
	static uint32_t CullMode[];

	static uint32_t TextureTarget[];
	static uint32_t WrapMode[];
	static uint32_t FilterMode[];
	static uint32_t InternalFormat[];
	static uint32_t ColorAttachment[];
public:
	virtual const char* CompareMethodToString(uint32_t)override;
	virtual const char* BlendFuncToString(uint32_t)override;
	virtual const char* BlendFactorToString(uint32_t)override;
	virtual const char* OperationToString(uint32_t)override;
	virtual const char* FrontFaceToString(uint32_t)override;
	virtual const char* CullModeToString(uint32_t)override;

	virtual const char* TextureTargetToString(uint32_t)override;
	virtual const char* WrapModeToString(uint32_t)override;
	virtual const char* FilterModeToString(uint32_t)override;
	virtual const char* InternalFormatToString(uint32_t)override;
public:
	virtual uint32_t StringToCompareMethod(const char*)override;
	virtual uint32_t StringToBlendFunc(const char*)override;
	virtual uint32_t StringToBlendFactor(const char*)override;
	virtual uint32_t StringToOperation(const char*)override;
	virtual uint32_t StringToFrontFace(const char*)override;
	virtual uint32_t StringToCullMode(const char*)override;

	virtual uint32_t StringToTextureTarget(const char*)override;
	virtual uint32_t StringToWrapMode(const char*)override;
	virtual uint32_t StringToFilterMode(const char*)override;
	virtual uint32_t StringToInternalFormat(const char*)override;
};

