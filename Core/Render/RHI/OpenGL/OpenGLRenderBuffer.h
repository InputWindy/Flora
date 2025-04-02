#pragma once
#include <Render/RHI/OpenGL/OpenGLRHI.h>
namespace XVerse
{
	class XOpenGLRenderBuffer :public XRHIRenderBuffer
	{
	public:
		XOpenGLRenderBuffer() = default;
		XOpenGLRenderBuffer(uint32_t InX, uint32_t InY, EInternalFormat InInternalFormat) :XRHIRenderBuffer(InX,InY, InInternalFormat){}
		virtual ~XOpenGLRenderBuffer();
	public:
		/// <summary>
		/// whether GPU resources have been allocated successfully.
		/// </summary>
		virtual bool IsValid()override;

		/// <summary>
		/// try allocate GPU resource
		/// </summary>
		virtual bool InitRHI()override;

		/// <summary>
		/// try deallocate GPU resource
		/// </summary>
		virtual bool ReleaseRHI()override;

		/// <summary>
		/// deep copy
		/// <param name="rhs"></param>
		/// </summary>
		virtual bool CopyFrom(std::shared_ptr<XRHIResource>)override;

		/// <summary>
		/// synchronize CPU resources to the GPU without checking the correctness of the resources.
		/// </summary>
		virtual bool UpdateRHI()override;
	public:
		virtual void Bind() override;
		virtual void UnBind() override;
	public:
		inline uint32_t GetHandle()const { return Handle; }
	private:
		uint32_t Handle;

	};
}