#pragma once
#include <Render/RHI/OpenGL/OpenGLRHI.h>
namespace flora
{
	class XOpenGLVertexArray :public XRHIVertexArray
	{
	public:
		XOpenGLVertexArray() = default;
		virtual ~XOpenGLVertexArray();
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
		virtual void Bind()   const override;

		virtual void UnBind() const override;

		virtual void Setup(std::shared_ptr<XRHIBuffer>, std::shared_ptr<XRHIBuffer>, const BufferLayout&) override;

	public:
		inline uint32_t GetHandle()const { return Handle; }
	private:
		uint32_t Handle;
		
	};
}