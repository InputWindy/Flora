#pragma once
#include <Render/RHI/OpenGL/OpenGLRHI.h>
namespace XVerse
{
	class XOpenGLTimeQuery : public XRHITimeQuery
	{
	public:
		XOpenGLTimeQuery() = default;
		virtual ~XOpenGLTimeQuery();
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
		virtual void BeginQuery() override;
		virtual void EndQuery() override;

		virtual double GetElapsedTime()const override { return EndTime - StartTime; };
	private:
		uint32_t BeginHandle = 0;
		uint32_t EndHandle = 0;

		uint64_t StartTime = 0;
		uint64_t EndTime = 0;

	};

	class XOpenGLPrimitiveQuery : public XRHIPrimitiveQuery
	{
	public:
		XOpenGLPrimitiveQuery() = default;
		virtual ~XOpenGLPrimitiveQuery();
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
		virtual void BeginQuery() override;
		virtual void EndQuery() override;

		virtual unsigned int GetPrimitivesGenerated()const override { return PrimitivesGenerated; };

	private:
		uint32_t Handle = 0;
		unsigned int PrimitivesGenerated = 0;
	};
}