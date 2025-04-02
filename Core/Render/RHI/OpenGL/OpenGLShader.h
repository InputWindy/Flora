#pragma once
#include <Render/RHI/OpenGL/OpenGLRHI.h>
namespace XVerse
{
	class XOpenGLShader :public XRHIShader
	{
	public:
		XOpenGLShader() = default;
		XOpenGLShader(EShaderType InShaderType) :XRHIShader(InShaderType) {}
		virtual ~XOpenGLShader();
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
		/// load source code
		/// </summary>
		/// <param name="glsl shade code"></param>
		virtual void SetSourceCode(const std::string&)override;

		/// <summary>
		/// get compile log (if failed)
		/// </summary>
		/// <param name=""></param>
		virtual void GetShaderInfoLog(std::string&)override;

		/// <summary>
		/// compile shader source code
		/// </summary>
		/// <returns></returns>
		virtual bool Compile()override;

		/// <summary>
		/// check
		/// </summary>
		/// <returns></returns>
		virtual bool IsCompiled()override;
	public:
		inline uint32_t GetHandle()const { return Handle; }
	private:
		uint32_t Handle = 0;
	};
}