#pragma once
#include <Render/RHI/OpenGL/OpenGLRHI.h>
namespace XVerse
{
	class XOpenGLSampler :public XRHISampler
	{
	public:
		XOpenGLSampler() = default;
		virtual ~XOpenGLSampler();
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
		/// <summary>
		/// bind sampler
		/// </summary>
		/// <param name="tex unit"></param>
		virtual void Bind(uint32_t) override;
		virtual void UnBind(uint32_t) override;

		virtual void SetSamplerState(const XRHISamplerCreateInfo&) ;
	public:
		inline uint32_t GetHandle()const { return Handle; }
	private:
		uint32_t Handle;

	};
}