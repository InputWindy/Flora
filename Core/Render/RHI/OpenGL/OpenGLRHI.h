#pragma once
#include <Render/RHI/RHI.h>
#include <glad/glad.h>

namespace XVerse
{
	class XOpenGLRHI:public XRHI
	{
	public:
		XOpenGLRHI() = default;
		virtual ~XOpenGLRHI() = default;
	public:
		virtual std::shared_ptr<XRHIShader>				CreateShader(const XRHIShaderCreateInfo&);
		virtual std::shared_ptr<XRHIShaderProgram>		CreateShaderProgram(const XRHIShaderProgramCreateInfo&);
		virtual std::shared_ptr<XRHISampler>			CreateSampler(const XRHISamplerCreateInfo&);
		virtual std::shared_ptr<XRHITexture>			CreateTexture(const XRHITextureCreateInfo&);
		virtual std::shared_ptr<XRHIBuffer>				CreateBuffer(const XRHIBufferCreateInfo&);
		virtual std::shared_ptr<XRHIVertexArray>		CreateVertexArray(const XRHIVertexArrayCreateInfo&);
		virtual std::shared_ptr<XRHIRenderBuffer>		CreateRenderBuffer(const XRHIRenderBufferCreateInfo&);
		virtual std::shared_ptr<XRHIFrameBuffer>		CreateFrameBuffer(const XRHIFrameBufferCreateInfo&);
		virtual std::shared_ptr<XRHIQuery>				CreateQuery(const XRHIQueryCreateInfo&);

	public:
		virtual int GetFormatCompNum(XVerse::EFormat) override;
		virtual int GetDataTypeSize(XVerse::EDataType) override;
	public:
		virtual void SetupMaterialState(XMaterialState&) override;

		/*
			Operations
		*/
		virtual void DrawElement(EDrawMode, std::shared_ptr<XRHIVertexArray>,size_t) override;

		virtual void DrawElementInstanced(EDrawMode, std::shared_ptr<XRHIVertexArray>, size_t, uint32_t = 1) override;

		virtual void ClearFrameBuffer(EClearBufferBit) override;

		virtual void ClearColor(float r, float g, float b, float a)override;

		virtual void ResizeViewport(uint32_t, uint32_t, uint32_t, uint32_t) override;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="num_groups_x"></param>
		/// <param name="num_groups_y"></param>
		/// <param name="num_groups_z"></param>
		virtual void DispatchCompute(uint32_t, uint32_t, uint32_t) override;

		/// <summary>
		/// The offset into the buffer object currently bound to the GL_DISPATCH_INDIRECT_BUFFER buffer target at which the dispatch parameters are stored.
		/// </summary>
		/// <param name="offset"></param>
		virtual void DispatchComputeIndirect(uint32_t) override;

		/// <summary>
		///  indirect draw array
		/// </summary>
		/// <param name=""></param>
		virtual void DrawArrayIndirect(EDrawMode,std::shared_ptr<XRHIDrawIndirectBuffer>) override;

		/// <summary>
		/// indirect draw element
		/// </summary>
		/// <param name=" draw mode"></param>
		/// <param name=" cmd buffer"></param>
		/// <param name=" ibo "></param>
		virtual void DrawElementIndirect(EDrawMode, std::shared_ptr<class XRHIDrawIndirectBuffer>,std::shared_ptr<XRHIBuffer>) override;

		/// <summary>
		/// bind buffer to specific target ( -f )
		/// </summary>
		/// <param name=""></param>
		/// <param name=""></param>
		virtual void Transition(std::shared_ptr<XRHIBuffer>, EBufferTarget)override;

		/// <summary>
		/// Memory barriers ensure that the visibility of all write operations prior to a synchronization point inserted in the rendering pipeline for subsequent read operations.
		/// In brief,flush memory write command.
		/// </summary>
		/// <param name="memory type"></param>
		virtual void MemoryBarrier(EMemoryBarrierBit) override;

		virtual void MemoryBarrier(EBufferTarget) override;

		virtual void FlushRenderCommand() override;

		virtual void FinishRenderCommand() override;

		virtual void AssertError(const std::string&) override;
	public:
		bool InitOpenGL(void*);
	};
	
	GLenum ToGLDrawMode(XVerse::EDrawMode);
	GLenum ToGLFilterMode(XVerse::EFilterMode);
	GLenum ToGLWrapMode(XVerse::EWrapMode);
	GLenum ToGLTextureCompareMode(XVerse::ETextureCompareMode);
	GLenum ToGLCompareFunc(XVerse::ECompareFunc);
	GLenum ToGLInternalFormat(XVerse::EInternalFormat);
	GLenum ToGLBufferTarget(XVerse::EBufferTarget);
	GLenum ToGLBufferUsage(XVerse::EBufferUsage);
	GLenum ToGLAccessPolicy(XVerse::EAccessPolicy);
	GLenum ToGLFormat(XVerse::EFormat);
	GLenum ToGLDataType(XVerse::EDataType);
	GLenum ToGLBlendEquation(XVerse::EBlendEquation);
	GLenum ToGLBlendFactor(XVerse::EBlendFactor);
	GLenum ToGLStencilOp(XVerse::EStencilOp);
	GLenum ToGLCullFace(XVerse::ECullFace);
	GLenum ToGLFrontFace(XVerse::EFrontFace);

	
	GLbitfield ToGLMemoryBufferBit(EMemoryBarrierBit);
	GLbitfield ToGLClearBufferBit(EClearBufferBit InBufferBit);
	

}