#pragma once
#include <Render/RHI/OpenGL/OpenGLRHI.h>
namespace XVerse
{
	class XOpenGLShaderProgram :public XRHIShaderProgram
	{
	public:
		XOpenGLShaderProgram() = default;
		virtual ~XOpenGLShaderProgram();
	public:
		/// <summary>
		/// whether GPU resources have been allocated successfully.
		/// </summary>
		virtual bool IsValid() override;

		/// <summary>
		/// try allocate GPU resource
		/// </summary>
		virtual bool InitRHI() override;

		/// <summary>
		/// try deallocate GPU resource
		/// </summary>
		virtual bool ReleaseRHI() override;

		/// <summary>
		/// deep copy
		/// <param name="rhs"></param>
		/// </summary>
		virtual bool CopyFrom(std::shared_ptr<XRHIResource>) override;

		/// <summary>
		/// synchronize CPU resources to the GPU without checking the correctness of the resources.
		/// </summary>
		virtual bool UpdateRHI() override;
	public:
		/// <summary>
		/// Add shader
		/// </summary>
		/// <param name=""></param>
		virtual void AttachShader(std::shared_ptr<XRHIShader>)const override;

		/// <summary>
		/// link
		/// </summary>
		/// <returns></returns>
		virtual bool Link() const override;

		/// <summary>
		/// check link status
		/// </summary>
		/// <returns></returns>
		virtual bool IsLinked() const override;

		/// <summary>
		/// bind render context
		/// </summary>
		virtual void Use()  const override;

		/// <summary>
		/// get link error info log
		/// </summary>
		virtual void GetShaderProgramInfoLog(std::string&) const override;

		/// <summary>
		/// </summary>
		/// <param name="buffer"></param>
		virtual void SetBuffer(std::shared_ptr<XVerse::XRHIBuffer>) const override;

		/// <summary>
		/// glsl uniform samplerXX
		/// </summary>
		/// <param name="name"></param>
		/// <param name="texture unit"></param>
		/// <param name="texture object"></param>
		/// <param name="sampler"></param>
		virtual void SetTexture(const char*, uint32_t, std::shared_ptr<class XRHITexture>, std::shared_ptr<class XRHISampler>) const override;

		/// <summary>
		/// </summary>
		/// <param name="texture"></param>
		/// <param name="image_unit"></param>
		/// <param name="mip"></param>
		/// <param name="access_policy"></param>
		virtual void SetTextureImage(std::shared_ptr<class XRHITexture>, uint32_t, uint32_t , uint32_t /* = GL_READ_WRITE*/) const override;
		virtual void SetTextureImage(std::shared_ptr<class XRHITexture>, uint32_t, uint32_t, EAccessPolicy) const override;


		virtual void SetBool1(const char*, bool) const override;
		virtual void SetBool2(const char*, bool, bool) const override;
		virtual void SetBool3(const char*, bool, bool, bool) const override;
		virtual void SetBool4(const char*, bool, bool, bool, bool) const override;

		virtual void SetInt1(const char*, int) const override;
		virtual void SetInt2(const char*, int, int) const override;
		virtual void SetInt3(const char*, int, int, int) const override;
		virtual void SetInt4(const char*, int, int, int, int) const override;

		virtual void SetUInt1(const char*, uint32_t) const override;
		virtual void SetUInt2(const char*, uint32_t, uint32_t) const override;
		virtual void SetUInt3(const char*, uint32_t, uint32_t, uint32_t) const override;
		virtual void SetUInt4(const char*, uint32_t, uint32_t, uint32_t, uint32_t) const override;

		virtual void SetFloat1(const char*, float) const override;
		virtual void SetFloat2(const char*, float, float) const override;
		virtual void SetFloat3(const char*, float, float, float) const override;
		virtual void SetFloat4(const char*, float, float, float, float) const override;

		virtual void SetFloat1Array(const char*, size_t, float*) const override;
		virtual void SetFloat2Array(const char*, size_t, float*) const override;
		virtual void SetFloat3Array(const char*, size_t, float*) const override;
		virtual void SetFloat4Array(const char*, size_t, float*) const override;

		virtual void SetInt1Array(const char*, size_t, int*) const override;
		virtual void SetInt2Array(const char*, size_t, int*) const override;
		virtual void SetInt3Array(const char*, size_t, int*) const override;
		virtual void SetInt4Array(const char*, size_t, int*) const override;

		virtual void SetUInt1Array(const char*, size_t, uint32_t*) const override;
		virtual void SetUInt2Array(const char*, size_t, uint32_t*) const override;
		virtual void SetUInt3Array(const char*, size_t, uint32_t*) const override;
		virtual void SetUInt4Array(const char*, size_t, uint32_t*) const override;

		virtual void SetMatrixFloat2x2Array(const char*, size_t, bool, float*) const override;
		virtual void SetMatrixFloat3x3Array(const char*, size_t, bool, float*) const override;
		virtual void SetMatrixFloat4x4Array(const char*, size_t, bool, float*) const override;
		virtual void SetMatrixFloat2x3Array(const char*, size_t, bool, float*) const override;
		virtual void SetMatrixFloat3x2Array(const char*, size_t, bool, float*) const override;
		virtual void SetMatrixFloat2x4Array(const char*, size_t, bool, float*) const override;
		virtual void SetMatrixFloat4x2Array(const char*, size_t, bool, float*) const override;
		virtual void SetMatrixFloat3x4Array(const char*, size_t, bool, float*) const override;
		virtual void SetMatrixFloat4x3Array(const char*, size_t, bool, float*) const override;
	public:
		inline uint32_t GetHandle()const { return Handle; }
	private:
		uint32_t Handle;
	};
}