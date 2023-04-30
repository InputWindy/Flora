#pragma once
#include "Core.h"
#include "Type.h"
#include "Texture.h"
#include "RenderState.h"
#include "Material.h"
#include "Mesh.h"
#include "Canvas.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
class FLORA_API FRenderCommand
{
	friend class FTexture;
	friend class FMaterial;
protected:
	FRenderCommand() = default;
public:
	virtual ~FRenderCommand() = default;

	static FRenderCommand* Create(ERenderCommandType Type = E_RHI_OpenGL);
public:
	virtual bool InitResources() = 0;
public:
	/// <summary>
	/// generate an empty texture
	/// </summary>
	virtual Ref<FTexture> GenerateTexture(
		IN const char*,		/*Name*/
		IN uint16_t,		/*Width*/
		IN uint16_t,		/*Height*/
		IN uint16_t,		/*Depth*/
		IN ETextureTarget,	/*Target*/
		IN EInternalFormat, /*Internal*/
		IN FTextureInfo Info = FTextureInfo(),/*Texture Params*/
		IN uint32_t = 4 /*Samples*/
	) = 0;

	/// <summary>
	/// apply for a new mesh(empty)
	/// </summary>
	virtual Ref<FMesh> GenerateMesh(IN const char* Name) = 0;

	/// <summary>
	/// apply for a new material(empty)
	/// </summary>
	virtual Ref<FMaterial> GenerateMaterial(IN const char* Name) = 0;
	
	/// <summary>
	/// generate valid canvas with no output bindings & input bindings
	/// </summary>
	virtual Ref<FCanvas>   GenerateCanvas(IN uint32_t Wid,IN uint32_t Hei) = 0;
public:
	virtual void SetBool(const char*, uint32_t, const bool&) = 0;
	virtual void SetInt(const char*, uint32_t, const int&) = 0;
	virtual void SetInt2(const char*, uint32_t, const glm::ivec2&) = 0;
	virtual void SetInt3(const char*, uint32_t, const glm::ivec3&) = 0;
	virtual void SetInt4(const char*, uint32_t, const glm::ivec4&) = 0;
	virtual void SetFloat(const char*, uint32_t, const float&) = 0;
	virtual void SetFloat2(const char*, uint32_t, const glm::vec2&) = 0;
	virtual void SetFloat3(const char*, uint32_t, const glm::vec3&) = 0;
	virtual void SetFloat4(const char*, uint32_t, const glm::vec4&) = 0;
	virtual void SetDouble(const char*, uint32_t, const double&) = 0;
	virtual void SetDouble2(const char*, uint32_t, const  glm::dvec2&) = 0;
	virtual void SetDouble3(const char*, uint32_t, const  glm::dvec3&) = 0;
	virtual void SetDouble4(const char*, uint32_t, const  glm::dvec4&) = 0;
	virtual void SetMat2f(const char*, uint32_t, uint32_t, const vector<glm::mat2>&) = 0;
	virtual void SetMat3f(const char*, uint32_t, uint32_t, const vector<glm::mat3>&) = 0;
	virtual void SetMat4f(const char*, uint32_t, uint32_t, const vector<glm::mat4>&) = 0;

	virtual void SetRenderState(const FRenderState&) = 0;
public:
	//======================//
	//     OpenGL Command   //
	//======================//
	virtual void Flush()  = 0;//flush opengl command queue
	virtual void Finish() = 0;//flush opengl command queue and wait for render finish

	virtual void DrawElements(size_t,EDrawMode = EDrawMode_TRIANGLES) = 0;
	virtual void DrawElementsInstanced(size_t, int, EDrawMode = EDrawMode_TRIANGLES) = 0;

	virtual void BindDrawData(uint32_t, uint32_t) = 0;//vao + ibo

	virtual bool CompileShader(uint32_t) = 0;
	virtual bool LinkShader(uint32_t) = 0;
protected:
	ERenderCommandType Type = E_RHI_OpenGL;
public:
	virtual const char* CompareMethodToString(uint32_t) = 0;
	virtual const char* BlendFuncToString(uint32_t) = 0;
	virtual const char* BlendFactorToString(uint32_t) = 0;
	virtual const char* OperationToString(uint32_t) = 0;
	virtual const char* FrontFaceToString(uint32_t) = 0;
	virtual const char* CullModeToString(uint32_t) = 0;
	
	virtual const char* TextureTargetToString(uint32_t) = 0;
	virtual const char* WrapModeToString(uint32_t) = 0;
	virtual const char* FilterModeToString(uint32_t) = 0;
	virtual const char* FormatToString(uint32_t) =0;
	virtual const char* InternalFormatToString(uint32_t) = 0;
public:
	virtual uint32_t StringToCompareMethod(const char*) = 0;
	virtual uint32_t StringToBlendFunc(const char*) = 0;
	virtual uint32_t StringToBlendFactor(const char*) = 0;
	virtual uint32_t StringToOperation(const char*) = 0;
	virtual uint32_t StringToFrontFace(const char*) = 0;
	virtual uint32_t StringToCullMode(const char*) = 0;
	
	virtual uint32_t StringToTextureTarget(const char*) = 0;
	virtual uint32_t StringToWrapMode(const char*) = 0;
	virtual uint32_t StringToFilterMode(const char*) = 0;
	virtual uint32_t StringToFormat(const char*) = 0;
	virtual uint32_t StringToInternalFormat(const char*) = 0;
};

