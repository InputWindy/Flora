#include <Render/RHI/OpenGL/OpenGLRHI.h>
#include <Render/RHI/OpenGL/OpenGLBuffer.h>
#include <Render/RHI/OpenGL/OpenGLTexture.h>
#include <Render/RHI/OpenGL/OpenGLFrameBuffer.h>
#include <Render/RHI/OpenGL/OpenGLRenderBuffer.h>
#include <Render/RHI/OpenGL/OpenGLSampler.h>
#include <Render/RHI/OpenGL/OpenGLShader.h>
#include <Render/RHI/OpenGL/OpenGLShaderProgram.h>
#include <Render/RHI/OpenGL/OpenGLVertexArray.h>
#include <Render/RHI/OpenGL/OpenGLQuery.h>
#include <cassert>
std::shared_ptr<XVerse::XRHIShader> XVerse::XOpenGLRHI::CreateShader(const XRHIShaderCreateInfo& Info)
{
    std::shared_ptr<XVerse::XOpenGLShader> Shader = std::make_shared<XOpenGLShader>(Info.ShaderType);
    if (Shader->InitRHI())
    {
        Shader->SetSourceCode(Info.ShaderCode);
        Shader->UpdateRHI();
    }
    else Shader = nullptr;
    
    return Shader;
}

std::shared_ptr<XVerse::XRHIShaderProgram> XVerse::XOpenGLRHI::CreateShaderProgram(const XRHIShaderProgramCreateInfo& Info)
{
    std::shared_ptr<XVerse::XOpenGLShaderProgram> ShaderProgram = std::make_shared<XOpenGLShaderProgram>();
    if (ShaderProgram->InitRHI())
    {
        ShaderProgram->AttachShader(Info.VertexShader);
        ShaderProgram->AttachShader(Info.TessControlShader);
        ShaderProgram->AttachShader(Info.TessEvaluationShader);
        ShaderProgram->AttachShader(Info.GeometryShader);
        ShaderProgram->AttachShader(Info.FragmentShader);
        ShaderProgram->AttachShader(Info.ComputeShader);
        ShaderProgram->UpdateRHI();
    }
    else ShaderProgram = nullptr;
    return ShaderProgram;
}

std::shared_ptr<XVerse::XRHISampler> XVerse::XOpenGLRHI::CreateSampler(const XRHISamplerCreateInfo& Info)
{
    std::shared_ptr<XVerse::XOpenGLSampler> Sampler = std::make_shared<XOpenGLSampler>();
    assert(glGetError() == GL_NO_ERROR);
    if (Sampler->InitRHI())
    {
        assert(glGetError() == GL_NO_ERROR);
        Sampler->SetSamplerState(Info);
        assert(glGetError() == GL_NO_ERROR);
        /*Sampler->SetWrapR(Info.WrapR);
        Sampler->SetWrapS(Info.WrapS);
        Sampler->SetWrapT(Info.WrapT);
        Sampler->SetMinFilter(Info.MinFilterMode);
        Sampler->SetMagFilter(Info.MagFilterMode);
        Sampler->SetMinLod(Info.MinMipLevel);
        Sampler->SetMaxLod(Info.MaxMipLevel);
        Sampler->SetLodBias(Info.LodBias);
        Sampler->SetBorderColor(Info.BorderColor[0], Info.BorderColor[1], Info.BorderColor[2], Info.BorderColor[3]);
        Sampler->SetCompareMode(Info.CompareMode);
        Sampler->SetCompareFunc(Info.CompareFunc);*/

        Sampler->UpdateRHI();
    }
    else Sampler = nullptr;
    return Sampler;
}

std::shared_ptr<XVerse::XRHITexture> XVerse::XOpenGLRHI::CreateTexture(const XRHITextureCreateInfo& Info)
{
    std::shared_ptr<XRHITexture> Texture = nullptr;
    switch (Info.TextureType)
    {
    case XVerse::ETextureTarget::TT_TEXTURE_1D:break;
    case XVerse::ETextureTarget::TT_PROXY_TEXTURE_1D:break;
    case XVerse::ETextureTarget::TT_TEXTURE_2D:Texture = std::make_shared<XOpenGLTexture2D>(Info.MipLevels,Info.InternalFormat,Info.SizeX,Info.SizeY); break;
    case XVerse::ETextureTarget::TT_PROXY_TEXTURE_2D:break;
    case XVerse::ETextureTarget::TT_TEXTURE_1D_ARRAY:break;
    case XVerse::ETextureTarget::TT_PROXY_TEXTURE_1D_ARRAY:break;
    case XVerse::ETextureTarget::TT_TEXTURE_RECTANGLE:break;
    case XVerse::ETextureTarget::TT_PROXY_TEXTURE_RECTANGLE:break;
    case XVerse::ETextureTarget::TT_TEXTURE_CUBE_MAP_POSITIVE_X:break;
    case XVerse::ETextureTarget::TT_TEXTURE_CUBE_MAP_NEGATIVE_X:break;
    case XVerse::ETextureTarget::TT_TEXTURE_CUBE_MAP_POSITIVE_Y:break;
    case XVerse::ETextureTarget::TT_TEXTURE_CUBE_MAP_NEGATIVE_Y:break;
    case XVerse::ETextureTarget::TT_TEXTURE_CUBE_MAP_POSITIVE_Z:break;
    case XVerse::ETextureTarget::TT_TEXTURE_CUBE_MAP_NEGATIVE_Z:break;
    case XVerse::ETextureTarget::TT_PROXY_TEXTURE_CUBE_MAP:     break;
    case XVerse::ETextureTarget::TT_TEXTURE_2D_MULTISAMPLE:     break;
    case XVerse::ETextureTarget::TT_PROXY_TEXTURE_2D_MULTISAMPLE:break;
    case XVerse::ETextureTarget::TT_TEXTURE_3D:Texture = std::make_shared<XOpenGLVolumeTexture>(Info.MipLevels, Info.InternalFormat, Info.SizeX, Info.SizeY,Info.SizeZ); break;
    case XVerse::ETextureTarget::TT_PROXY_TEXTURE_3D:break;
    case XVerse::ETextureTarget::TT_TEXTURE_2D_ARRAY:break;
    case XVerse::ETextureTarget::TT_PROXY_TEXTURE_2D_ARRAY:break;
    case XVerse::ETextureTarget::TT_TEXTURE_2D_MULTISAMPLE_ARRAY:break;
    case XVerse::ETextureTarget::TT_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY:break;
    case XVerse::ETextureTarget::TT_TEXTURE_CUBE_MAP:Texture = std::make_shared<XOpenGLTextureCube>(Info.MipLevels, Info.InternalFormat, Info.SizeX); break;
    case XVerse::ETextureTarget::TT_TEXTURE_CUBE_MAP_ARRAY:break;
    case XVerse::ETextureTarget::TT_TEXTURE_BUFFER:break;
    case XVerse::ETextureTarget::TT_MAX_COUNT:break;
    default:
        break;
    }

    if (Texture)
    {
        assert(glGetError() == GL_NO_ERROR);
        Texture->InitRHI();
        assert(glGetError() == GL_NO_ERROR);
        Texture->SetSamplerState(Info.DefaultSamplerInfo);
        //Texture->SetWrapR(Info.DefaultSamplerInfo.WrapR);
        //Texture->SetWrapS(Info.DefaultSamplerInfo.WrapS);
        //Texture->SetWrapT(Info.DefaultSamplerInfo.WrapT);
        //Texture->SetMinFilter(Info.DefaultSamplerInfo.MinFilterMode);
        //Texture->SetMagFilter(Info.DefaultSamplerInfo.MagFilterMode);
        //Texture->SetMinLod(Info.DefaultSamplerInfo.MinMipLevel);
        //Texture->SetMaxLod(Info.DefaultSamplerInfo.MaxMipLevel);
        ////Texture->SetLodBias(Info.DefaultSamplerInfo.LodBias);
        //Texture->SetBorderColor(Info.DefaultSamplerInfo.BorderColor[0], Info.DefaultSamplerInfo.BorderColor[1], Info.DefaultSamplerInfo.BorderColor[2], Info.DefaultSamplerInfo.BorderColor[3]);
        //Texture->SetCompareMode(Info.DefaultSamplerInfo.CompareMode);
        //Texture->SetCompareFunc(Info.DefaultSamplerInfo.CompareFunc);
        Texture->UpdateRHI();
        assert(glGetError() == GL_NO_ERROR);
    }

    return Texture;
}

std::shared_ptr<XVerse::XRHIBuffer> XVerse::XOpenGLRHI::CreateBuffer(const XRHIBufferCreateInfo& Info)
{
    std::shared_ptr<XVerse::XRHIBuffer> Buffer = nullptr;

    switch (Info.BufferTarget)
    {
    case XVerse::EBufferTarget::BT_ARRAY_BUFFER:Buffer = std::make_shared<XOpenGLVertexBuffer>(Info.BindingSlot); break;
    case XVerse::EBufferTarget::BT_ATOMIC_COUNTER_BUFFER:Buffer = std::make_shared<XOpenGLAtomicCounterBuffer>(Info.BindingSlot); break;
    case XVerse::EBufferTarget::BT_COPY_READ_BUFFER:break;
    case XVerse::EBufferTarget::BT_COPY_WRITE_BUFFER:break;
    case XVerse::EBufferTarget::BT_DISPATCH_INDIRECT_BUFFER:Buffer = std::make_shared<XOpenGLDispatchIndirectBuffer>(Info.BindingSlot); break;
    case XVerse::EBufferTarget::BT_DRAW_INDIRECT_BUFFER:Buffer = std::make_shared<XOpenGLDrawIndirectBuffer>(Info.BindingSlot); break;
    case XVerse::EBufferTarget::BT_ELEMENT_ARRAY_BUFFER:Buffer = std::make_shared<XOpenGLIndexBuffer>(Info.BindingSlot); break;
    case XVerse::EBufferTarget::BT_PIXEL_PACK_BUFFER:break;
    case XVerse::EBufferTarget::BT_PIXEL_UNPACK_BUFFER:break;
    case XVerse::EBufferTarget::BT_QUERY_BUFFER:break;
    case XVerse::EBufferTarget::BT_SHADER_STORAGE_BUFFER:Buffer = std::make_shared<XOpenGLShaderStorageBuffer>(Info.BindingSlot); break;
    case XVerse::EBufferTarget::BT_TEXTURE_BUFFER:break;
    case XVerse::EBufferTarget::BT_TRANSFORM_FEEDBACK_BUFFER:break;
    case XVerse::EBufferTarget::BT_UNIFORM_BUFFER:Buffer = std::make_shared<XOpenGLUniformBuffer>(Info.BindingSlot); break;
    case XVerse::EBufferTarget::BT_MAX_COUNT:
    default:
        break;
    }

    if (Buffer)
    {
        Buffer->InitRHI();
        Buffer->UpdateRHI();
    }

    return Buffer;
}

std::shared_ptr<XVerse::XRHIVertexArray> XVerse::XOpenGLRHI::CreateVertexArray(const XRHIVertexArrayCreateInfo& Info)
{
    std::shared_ptr<XVerse::XOpenGLVertexArray> VertexArray = std::make_shared<XOpenGLVertexArray>();
    if (VertexArray->InitRHI())
    {
        VertexArray->Setup(Info.IndexBuffer, Info.VertexBuffer, Info.VertexBufferLayout);
        VertexArray->UpdateRHI();
    }
    else VertexArray = nullptr;
    return VertexArray;
}

std::shared_ptr<XVerse::XRHIRenderBuffer> XVerse::XOpenGLRHI::CreateRenderBuffer(const XRHIRenderBufferCreateInfo& Info)
{
    std::shared_ptr<XVerse::XOpenGLRenderBuffer> RenderBuffer = std::make_shared<XOpenGLRenderBuffer>(Info.Width, Info.Height, Info.InternalFormat);
    if (RenderBuffer->InitRHI())
    {
        RenderBuffer->UpdateRHI();
    }
    else RenderBuffer = nullptr;
    return RenderBuffer;
}

std::shared_ptr<XVerse::XRHIFrameBuffer> XVerse::XOpenGLRHI::CreateFrameBuffer(const XRHIFrameBufferCreateInfo& Info)
{
    std::shared_ptr<XVerse::XOpenGLFrameBuffer> FrameBuffer = std::make_shared<XOpenGLFrameBuffer>(Info.DepthStencilAttachment);
    if (FrameBuffer->InitRHI())
    {
        FrameBuffer->SetColorAttachment(Info.ColorAttachments);
        FrameBuffer->EnableOutputAttachments(Info.DrawBuffers);
        FrameBuffer->UpdateRHI();
    }
    else FrameBuffer = nullptr;
    return FrameBuffer;
}

std::shared_ptr<XVerse::XRHIQuery> XVerse::XOpenGLRHI::CreateQuery(const XRHIQueryCreateInfo& Info)
{
    std::shared_ptr<XVerse::XRHIQuery> Query = nullptr;

    switch (Info.QueryTarget)
    {
    case XVerse::EQueryTarget::EQT_SAMPLES_PASSED:break;
    case XVerse::EQueryTarget::EQT_ANY_SAMPLES_PASSED:break;
    case XVerse::EQueryTarget::EQT_ANY_SAMPLES_PASSED_CONSERVATIVE:break;
    case XVerse::EQueryTarget::EQT_PRIMITIVES_GENERATED:Query = std::make_shared<XOpenGLPrimitiveQuery>(); break;
    case XVerse::EQueryTarget::EQT_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN:break;
    case XVerse::EQueryTarget::EQT_TIME_ELAPSED:Query = std::make_shared<XOpenGLTimeQuery>(); break;
    case XVerse::EQueryTarget::EQT_MAX_COUNT:break;
    default:
        break;
    }
    
	if (Query && Query->InitRHI())
	{
        Query->UpdateRHI();
	}

	return Query;
}

void XVerse::XOpenGLRHI::SetupMaterialState(XMaterialState& MaterialState)
{
    if (MaterialState.bEnableDepth)
    {
        glEnable(GL_DEPTH_TEST);
        if (MaterialState.bIsDepthReadOnly) {
            glDepthMask(GL_FALSE);
        }
        else
        {
            glDepthMask(GL_TRUE);
        }
        glDepthFunc(ToGLCompareFunc(MaterialState.DepthCompareFunc));

        if (MaterialState.bEnableDepthBias)
        {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(MaterialState.BiasOffsetFactor, MaterialState.BiasOffsetScale);
        }
        else
        {
            glDisable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(0,0);
        }
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }

    if (MaterialState.bEnableBlend)
    {
        glEnable(GL_BLEND);
        glBlendEquation(ToGLBlendEquation(MaterialState.BlendEquation));
        glBlendFunc(ToGLBlendFactor(MaterialState.BlendSrc), ToGLBlendFactor(MaterialState.BlendDest));
    }
    else
    {
        glDisable(GL_BLEND);
    }

    if (MaterialState.bEnableStencil)
    {
        glEnable(GL_STENCIL_TEST);
        if (MaterialState.bIsStencilReadOnly)
        {
            glStencilFunc(ToGLCompareFunc(MaterialState.StencilCompareFunc), MaterialState.StencilRef, 0x00);
        }
        else
        {
            glStencilFunc(ToGLCompareFunc(MaterialState.StencilCompareFunc), MaterialState.StencilRef, 0xFF);
        }
        
        glStencilOp(ToGLStencilOp(MaterialState.SFailOp), ToGLStencilOp(MaterialState.SPassZFailOp), ToGLStencilOp(MaterialState.PassOp));
    }
    else
    {
        glDisable(GL_STENCIL_TEST);
    }

    if (MaterialState.bEnableCull)
    {
        glEnable(GL_CULL_FACE);
        glFrontFace(ToGLFrontFace(MaterialState.FrontFace));
        glCullFace(ToGLCullFace(MaterialState.CullFace));        
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}

void XVerse::XOpenGLRHI::DrawElement(EDrawMode InDrawMode, std::shared_ptr<XRHIVertexArray> InVertexArray, size_t InIndexNum)
{
    std::shared_ptr<XOpenGLVertexArray> OpenglVertexArray = std::dynamic_pointer_cast<XOpenGLVertexArray>(InVertexArray);
    if (OpenglVertexArray && OpenglVertexArray->IsValid())
    {
        OpenglVertexArray->Bind();
        glDrawElements(ToGLDrawMode(InDrawMode), InIndexNum, GL_UNSIGNED_INT, 0);
    }
}

void XVerse::XOpenGLRHI::DrawElementInstanced(EDrawMode InDrawMode, std::shared_ptr<XRHIVertexArray> InVertexArray, size_t InIndexNum, uint32_t InInstanceNum)
{
    std::shared_ptr<XOpenGLVertexArray> OpenglVertexArray = std::dynamic_pointer_cast<XOpenGLVertexArray>(InVertexArray);
    if (OpenglVertexArray && OpenglVertexArray->IsValid())
    {
        OpenglVertexArray->Bind();
        glDrawElementsInstanced(ToGLDrawMode(InDrawMode), InIndexNum, GL_UNSIGNED_INT, 0, InInstanceNum);
    }
}

void XVerse::XOpenGLRHI::ClearFrameBuffer(EClearBufferBit InBufferBit)
{
    glClear(ToGLClearBufferBit(InBufferBit));
    //glClear(GL_COLOR_BUFFER_BIT);
}

void XVerse::XOpenGLRHI::ClearColor(float r, float g, float b, float a)
{
    glClearColor(r,g,b,a);
}

void XVerse::XOpenGLRHI::ResizeViewport(uint32_t xoffset, uint32_t yoffset, uint32_t x, uint32_t y)
{
    glViewport(xoffset, yoffset, x, y);
}

void XVerse::XOpenGLRHI::DispatchCompute(uint32_t x, uint32_t y, uint32_t z)
{
    glDispatchCompute(x, y, z);
}

void XVerse::XOpenGLRHI::DispatchComputeIndirect(uint32_t offset)
{
    glDispatchComputeIndirect(offset);
}

void XVerse::XOpenGLRHI::DrawArrayIndirect(EDrawMode DrawMode, std::shared_ptr<XRHIDrawIndirectBuffer> IndirectBuffer)
{
    if (IndirectBuffer)
    {
        IndirectBuffer->Bind();
        glMultiDrawArraysIndirect(ToGLDrawMode(DrawMode), nullptr, IndirectBuffer->GetCmdNum(), 0);
    }
}

void XVerse::XOpenGLRHI::DrawElementIndirect(EDrawMode DrawMode, std::shared_ptr<XRHIDrawIndirectBuffer> IndirectBuffer, std::shared_ptr<XRHIBuffer> Ibo)
{
	if (IndirectBuffer)
	{
		static GLuint VAO;
		auto ScopeInit = [&]()->bool
			{
				glGenVertexArrays(1, &VAO);
				return true;
			};

		static bool bScopeInit = ScopeInit();
		assert(glGetError() == GL_NO_ERROR);
		glBindVertexArray(VAO);
		assert(glGetError() == GL_NO_ERROR);
		//vbo->Bind();
		Transition(Ibo, EBufferTarget::BT_ELEMENT_ARRAY_BUFFER);
		assert(glGetError() == GL_NO_ERROR);
		IndirectBuffer->Bind();
		assert(glGetError() == GL_NO_ERROR);
		glMultiDrawElementsIndirect(ToGLDrawMode(DrawMode), GL_UNSIGNED_INT, nullptr, IndirectBuffer->GetCmdNum(), 0);
		assert(glGetError() == GL_NO_ERROR);
	}
}

void XVerse::XOpenGLRHI::Transition(std::shared_ptr<XRHIBuffer> Buffer, EBufferTarget Dst)
{
    uint32_t Handle = Buffer->GetHandle();
    switch (Dst)
    {
    case XVerse::EBufferTarget::BT_ARRAY_BUFFER:glBindBuffer(GL_ARRAY_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_ATOMIC_COUNTER_BUFFER:glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_COPY_READ_BUFFER:glBindBuffer(GL_COPY_READ_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_COPY_WRITE_BUFFER:glBindBuffer(GL_COPY_WRITE_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_DISPATCH_INDIRECT_BUFFER:glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_DRAW_INDIRECT_BUFFER:glBindBuffer(GL_DRAW_INDIRECT_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_ELEMENT_ARRAY_BUFFER:glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_PIXEL_PACK_BUFFER:glBindBuffer(GL_PIXEL_PACK_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_PIXEL_UNPACK_BUFFER:glBindBuffer(GL_PIXEL_UNPACK_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_QUERY_BUFFER:glBindBuffer(GL_QUERY_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_SHADER_STORAGE_BUFFER:glBindBuffer(GL_SHADER_STORAGE_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_TEXTURE_BUFFER:glBindBuffer(GL_TEXTURE_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_TRANSFORM_FEEDBACK_BUFFER:glBindBuffer(GL_TRANSFORM_FEEDBACK_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_UNIFORM_BUFFER:glBindBuffer(GL_UNIFORM_BUFFER, Handle); break;
    case XVerse::EBufferTarget::BT_MAX_COUNT:
    default:
        break;
    }
}


void XVerse::XOpenGLRHI::MemoryBarrier(EMemoryBarrierBit InBufferBit)
{
    glMemoryBarrier(ToGLMemoryBufferBit(InBufferBit));
}

void XVerse::XOpenGLRHI::MemoryBarrier(EBufferTarget InTarget)
{
    switch (InTarget)
    {
    case XVerse::EBufferTarget::BT_ARRAY_BUFFER:assert(0);break;
    case XVerse::EBufferTarget::BT_ATOMIC_COUNTER_BUFFER:MemoryBarrier(MBB_ATOMIC_COUNTER_BARRIER_BIT); break;
    case XVerse::EBufferTarget::BT_COPY_READ_BUFFER:assert(0); break;
    case XVerse::EBufferTarget::BT_COPY_WRITE_BUFFER:assert(0); break;
    case XVerse::EBufferTarget::BT_DISPATCH_INDIRECT_BUFFER:MemoryBarrier(MBB_COMMAND_BARRIER_BIT); break;
    case XVerse::EBufferTarget::BT_DRAW_INDIRECT_BUFFER:MemoryBarrier(MBB_COMMAND_BARRIER_BIT); break;
    case XVerse::EBufferTarget::BT_ELEMENT_ARRAY_BUFFER:assert(0); break;
    case XVerse::EBufferTarget::BT_PIXEL_PACK_BUFFER:assert(0); break;
    case XVerse::EBufferTarget::BT_PIXEL_UNPACK_BUFFER:assert(0); break;
    case XVerse::EBufferTarget::BT_QUERY_BUFFER:assert(0); break;
    case XVerse::EBufferTarget::BT_SHADER_STORAGE_BUFFER: MemoryBarrier(MBB_SHADER_STORAGE_BARRIER_BIT); break;
    case XVerse::EBufferTarget::BT_TEXTURE_BUFFER:assert(0); break;
    case XVerse::EBufferTarget::BT_TRANSFORM_FEEDBACK_BUFFER:assert(0); break;
    case XVerse::EBufferTarget::BT_UNIFORM_BUFFER: MemoryBarrier(MBB_UNIFORM_BARRIER_BIT); break;
    case XVerse::EBufferTarget::BT_MAX_COUNT:
    default:
        break;
    }
}

void XVerse::XOpenGLRHI::FlushRenderCommand()
{
    glFlush();
}

void XVerse::XOpenGLRHI::FinishRenderCommand()
{
    glFinish();
}

void XVerse::XOpenGLRHI::AssertError(const std::string& info)
{/*
    switch (GLenum ErrorType = glGetError())
    {
    case GL_INVALID_ENUM:
    {
        XVERSE_CORE_ERROR("OpenGL Error: GL_INVALID_ENUM ")assert(0, info);
        break;
    }
    case GL_INVALID_VALUE:
    {
        XVERSE_CORE_ERROR("OpenGL Error: GL_INVALID_VALUE ")assert(0, info);
            break;
    }
    case GL_INVALID_OPERATION:
    {
        XVERSE_CORE_ERROR("OpenGL Error: GL_INVALID_OPERATION ")assert(0, info);
            break;
    }
    case GL_INVALID_FRAMEBUFFER_OPERATION:
    {
        XVERSE_CORE_ERROR("OpenGL Error: GL_INVALID_FRAMEBUFFER_OPERATION ")assert(0, info);
            break;
    }
    case GL_STACK_OVERFLOW:
    {
        XVERSE_CORE_ERROR("OpenGL Error: GL_STACK_OVERFLOW ")assert(0, info);
            break;
    }
    case GL_STACK_UNDERFLOW:
    {
        XVERSE_CORE_ERROR("OpenGL Error: GL_STACK_UNDERFLOW ")assert(0, info);
            break;
    }
    case GL_OUT_OF_MEMORY:
    {
        XVERSE_CORE_ERROR("OpenGL Error: GL_OUT_OF_MEMORY ")assert(0, info);
            break;
    }
    default:
        break;
    }*/
}

#include <exception>

bool XVerse::XOpenGLRHI::InitOpenGL(void* Callback)
{
    bool ret = gladLoadGLLoader((GLADloadproc)Callback);

    if (!ret)
	{
		XVERSE_CORE_ERROR("failed to load opengl,no gpu");
        throw std::runtime_error("failed to load opengl,no gpu");
    }


	XVERSE_CORE_INFO("Opengl version : {0}", glGetString(GL_VERSION));
	XVERSE_CORE_INFO("Opengl vendor : {0}", glGetString(GL_VENDOR));
	XVERSE_CORE_INFO("Opengl renderer : {0}", glGetString(GL_RENDERER));

    int x, y, z;

    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &x);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &y);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &z);

    XVERSE_CORE_INFO("MAX_COMPUTE_WORK_GROUP_SIZE : (x:{0},y:{1},z:{2})",x,y,z);

    return ret;
}



GLenum XVerse::ToGLDrawMode(XVerse::EDrawMode InDrawMode)
{
    GLenum DrawMode = GL_TRIANGLES;
    switch (InDrawMode)
    {
    case XVerse::EDrawMode::DM_POINTS:DrawMode = GL_POINTS; break;
    case XVerse::EDrawMode::DM_LINE_STRIP:DrawMode = GL_LINE_STRIP; break;
    case XVerse::EDrawMode::DM_LINE_LOOP:DrawMode = GL_LINE_LOOP; break;
    case XVerse::EDrawMode::DM_LINES:DrawMode = GL_LINES; break;
    case XVerse::EDrawMode::DM_LINE_STRIP_ADJACENCY:DrawMode = GL_LINE_STRIP_ADJACENCY; break;
    case XVerse::EDrawMode::DM_LINES_ADJACENCY:DrawMode = GL_LINES_ADJACENCY; break;
    case XVerse::EDrawMode::DM_TRIANGLE_STRIP:DrawMode = GL_TRIANGLE_STRIP; break;
    case XVerse::EDrawMode::DM_TRIANGLE_FAN:DrawMode = GL_TRIANGLE_FAN; break;
    case XVerse::EDrawMode::DM_TRIANGLES:DrawMode = GL_TRIANGLES; break;
    case XVerse::EDrawMode::DM_TRIANGLE_STRIP_ADJACENCY:DrawMode = GL_TRIANGLE_STRIP_ADJACENCY; break;
    case XVerse::EDrawMode::DM_TRIANGLES_ADJACENCY:DrawMode = GL_TRIANGLES_ADJACENCY; break;
    case XVerse::EDrawMode::DM_PATCHES:DrawMode = GL_PATCHES; break;
    default:break;
    }
    return DrawMode;
}

GLenum XVerse::ToGLFilterMode(XVerse::EFilterMode InMode)
{
    GLenum Mode = GL_LINEAR;
    switch (InMode)
    {
    case XVerse::EFilterMode::FM_NEAREST:Mode = GL_NEAREST; break;
    case XVerse::EFilterMode::FM_LINEAR:Mode = GL_LINEAR; break;
    case XVerse::EFilterMode::FM_NEAREST_MIPMAP_NEAREST:Mode = GL_NEAREST_MIPMAP_NEAREST; break;
    case XVerse::EFilterMode::FM_LINEAR_MIPMAP_NEAREST:Mode = GL_LINEAR_MIPMAP_NEAREST; break;
    case XVerse::EFilterMode::FM_NEAREST_MIPMAP_LINEAR:Mode = GL_NEAREST_MIPMAP_LINEAR; break;
    case XVerse::EFilterMode::FM_LINEAR_MIPMAP_LINEAR:Mode = GL_LINEAR_MIPMAP_LINEAR; break;
    case XVerse::EFilterMode::FM_MAX_COUNT:
    default:
        break;
    }
    return Mode;
}
GLenum XVerse::ToGLWrapMode(XVerse::EWrapMode InMode)
{
    GLenum Mode = GL_CLAMP_TO_EDGE;
    switch (InMode)
    {
    case XVerse::EWrapMode::WM_CLAMP_TO_BORDER:Mode = GL_CLAMP_TO_BORDER; break;
    case XVerse::EWrapMode::WM_CLAMP_TO_EDGE:Mode = GL_CLAMP_TO_EDGE; break;
    case XVerse::EWrapMode::WM_MIRRORED_REPEAT:Mode = GL_MIRRORED_REPEAT; break;
    case XVerse::EWrapMode::WM_REPEAT:Mode = GL_REPEAT; break;
    case XVerse::EWrapMode::WM_MIRROR_CLAMP_TO_EDGE:Mode = GL_MIRROR_CLAMP_TO_EDGE; break;
    case XVerse::EWrapMode::WM_MAX_COUNT:
    default:
        break;
    }
    return Mode;
}

GLenum XVerse::ToGLTextureCompareMode(XVerse::ETextureCompareMode InMode)
{
    GLenum Mode = GL_COMPARE_REF_TO_TEXTURE;
    switch (InMode)
    {
    case XVerse::ETextureCompareMode::TCM_COMPARE_REF_TO_TEXTURE:Mode = GL_COMPARE_REF_TO_TEXTURE; break;
    case XVerse::ETextureCompareMode::TCM_NONE:Mode = GL_NONE; break;
    case XVerse::ETextureCompareMode::TCM_MAX_COUNT:
    default:
        break;
    }
    return Mode;
}
GLenum XVerse::ToGLCompareFunc(XVerse::ECompareFunc InFunc)
{
    GLenum Func = GL_LESS;
    switch (InFunc)
    {
    case XVerse::ECompareFunc::CF_LESS:Func = GL_LESS; break;
    case XVerse::ECompareFunc::CF_NEVER:Func = GL_NEVER; break;
    case XVerse::ECompareFunc::CF_EQUAL:Func = GL_EQUAL; break;
    case XVerse::ECompareFunc::CF_LEQUAL:Func = GL_LEQUAL; break;
    case XVerse::ECompareFunc::CF_GREATER:Func = GL_GREATER; break;
    case XVerse::ECompareFunc::CF_NOTEQUAL:Func = GL_NOTEQUAL; break;
    case XVerse::ECompareFunc::CF_GEQUAL:Func = GL_GEQUAL; break;
    case XVerse::ECompareFunc::CF_ALWAYS:Func = GL_ALWAYS; break;
    case XVerse::ECompareFunc::CF_MAX_COUNT:
    default:
        break;
    }
    return Func;
}

GLenum XVerse::ToGLInternalFormat(XVerse::EInternalFormat InFormat)
{
    GLenum Format = GL_RGBA32F;
    switch (InFormat)
    {
    case XVerse::EInternalFormat::IF_R8:                 Format = GL_R8                ;break;
    case XVerse::EInternalFormat::IF_R8I:                Format = GL_R8I               ;break;
    case XVerse::EInternalFormat::IF_R8UI:               Format = GL_R8UI              ;break;
    case XVerse::EInternalFormat::IF_R16:                Format = GL_R16               ;break;
    case XVerse::EInternalFormat::IF_R16I:               Format = GL_R16I              ;break;
    case XVerse::EInternalFormat::IF_R16UI:              Format = GL_R16UI             ;break;
    case XVerse::EInternalFormat::IF_R32I:               Format = GL_R32I              ;break;
    case XVerse::EInternalFormat::IF_R32UI:              Format = GL_R32UI             ;break;
    case XVerse::EInternalFormat::IF_RG8:                Format = GL_RG8               ;break;
    case XVerse::EInternalFormat::IF_RG8I:               Format = GL_RG8I              ;break;
    case XVerse::EInternalFormat::IF_RG8UI:              Format = GL_RG8UI            ;break;
    case XVerse::EInternalFormat::IF_RG16:               Format = GL_RG16              ;break;
    case XVerse::EInternalFormat::IF_RG16I:              Format = GL_RG16I             ;break;
    case XVerse::EInternalFormat::IF_RG16UI:             Format = GL_RG16UI            ;break;
    case XVerse::EInternalFormat::IF_RG32I:              Format = GL_RG32I             ;break;
    case XVerse::EInternalFormat::IF_RG32UI:             Format = GL_RG32UI            ;break;
    case XVerse::EInternalFormat::IF_RGB8:               Format = GL_RGB8              ;break;
    case XVerse::EInternalFormat::IF_RGB8I:              Format = GL_RGB8I             ;break;
    case XVerse::EInternalFormat::IF_RGB8UI:             Format = GL_RGB8UI            ;break;
    case XVerse::EInternalFormat::IF_RGB16:              Format = GL_RGB16             ;break;
    case XVerse::EInternalFormat::IF_RGB16I:             Format = GL_RGB16I           ;break;
    case XVerse::EInternalFormat::IF_RGB16UI:            Format = GL_RGB16UI           ;break;
    case XVerse::EInternalFormat::IF_RGB32I:             Format = GL_RGB32I            ;break;
    case XVerse::EInternalFormat::IF_RGB32UI:            Format = GL_RGB32UI           ;break;
    case XVerse::EInternalFormat::IF_RGBA8:              Format = GL_RGBA8             ;break;
    case XVerse::EInternalFormat::IF_RGBA8I:             Format = GL_RGBA8I            ;break;
    case XVerse::EInternalFormat::IF_RGBA8UI:            Format = GL_RGBA8UI           ;break;
    case XVerse::EInternalFormat::IF_RGBA16:             Format = GL_RGBA16            ;break;
    case XVerse::EInternalFormat::IF_RGBA16I:            Format = GL_RGBA16I           ;break;
    case XVerse::EInternalFormat::IF_RGBA16UI:           Format = GL_RGBA16UI          ;break;
    case XVerse::EInternalFormat::IF_RGBA32I:            Format = GL_RGBA32I           ;break;
    case XVerse::EInternalFormat::IF_RGBA32UI:           Format = GL_RGBA32UI          ;break;
    case XVerse::EInternalFormat::IF_R16F:               Format = GL_R16F             ;break;
    case XVerse::EInternalFormat::IF_R32F:               Format = GL_R32F              ;break;
    case XVerse::EInternalFormat::IF_RG16F:              Format = GL_RG16F            ;break;
    case XVerse::EInternalFormat::IF_RG32F:              Format = GL_RG32F             ;break;
    case XVerse::EInternalFormat::IF_RGB16F:             Format = GL_RGB16F            ;break;
    case XVerse::EInternalFormat::IF_RGB32F:             Format = GL_RGB32F            ;break;
    case XVerse::EInternalFormat::IF_RGBA16F:            Format = GL_RGBA16F           ;break;
    case XVerse::EInternalFormat::IF_RGBA32F:            Format = GL_RGBA32F           ;break;
    case XVerse::EInternalFormat::IF_DEPTH_COMPONENT16:  Format = GL_DEPTH_COMPONENT16 ;break;
    case XVerse::EInternalFormat::IF_DEPTH_COMPONENT24:  Format = GL_DEPTH_COMPONENT24 ;break;
    case XVerse::EInternalFormat::IF_DEPTH_COMPONENT32F: Format = GL_DEPTH_COMPONENT32F;break;
    case XVerse::EInternalFormat::IF_DEPTH24_STENCIL8:   Format = GL_DEPTH24_STENCIL8  ;break;
    case XVerse::EInternalFormat::IF_DEPTH32F_STENCIL8:  Format = GL_DEPTH32F_STENCIL8 ;break;
    case XVerse::EInternalFormat::IF_STENCIL_INDEX:      Format = GL_STENCIL_INDEX     ;break;
    case XVerse::EInternalFormat::IF_MAX_COUNT:          
    default:
        break;
    }
    return Format;
}

GLenum XVerse::ToGLBufferTarget(XVerse::EBufferTarget InUsage)
{
    GLenum Usage = GL_ARRAY_BUFFER;

    switch (InUsage)
    {
    case XVerse::EBufferTarget::BT_ARRAY_BUFFER:Usage  = GL_ARRAY_BUFFER; break;
    case XVerse::EBufferTarget::BT_ATOMIC_COUNTER_BUFFER:Usage  = GL_ATOMIC_COUNTER_BUFFER; break;
    case XVerse::EBufferTarget::BT_COPY_READ_BUFFER:Usage  = GL_COPY_READ_BUFFER; break;
    case XVerse::EBufferTarget::BT_COPY_WRITE_BUFFER:Usage  = GL_COPY_WRITE_BUFFER; break;
    case XVerse::EBufferTarget::BT_DISPATCH_INDIRECT_BUFFER:Usage  = GL_DISPATCH_INDIRECT_BUFFER; break;
    case XVerse::EBufferTarget::BT_DRAW_INDIRECT_BUFFER:Usage  = GL_DRAW_INDIRECT_BUFFER; break;
    case XVerse::EBufferTarget::BT_ELEMENT_ARRAY_BUFFER:Usage  = GL_ELEMENT_ARRAY_BUFFER; break;
    case XVerse::EBufferTarget::BT_PIXEL_PACK_BUFFER:Usage  = GL_PIXEL_PACK_BUFFER; break;
    case XVerse::EBufferTarget::BT_PIXEL_UNPACK_BUFFER:Usage  = GL_PIXEL_UNPACK_BUFFER; break;
    case XVerse::EBufferTarget::BT_QUERY_BUFFER:Usage  = GL_QUERY_BUFFER; break;
    case XVerse::EBufferTarget::BT_SHADER_STORAGE_BUFFER:Usage  = GL_SHADER_STORAGE_BUFFER; break;
    case XVerse::EBufferTarget::BT_TEXTURE_BUFFER:Usage  = GL_TEXTURE_BUFFER; break;
    case XVerse::EBufferTarget::BT_TRANSFORM_FEEDBACK_BUFFER:Usage  = GL_TRANSFORM_FEEDBACK_BUFFER; break;
    case XVerse::EBufferTarget::BT_UNIFORM_BUFFER:Usage  = GL_UNIFORM_BUFFER; break;
    case XVerse::EBufferTarget::BT_MAX_COUNT:
    default:
        break;
    }
    return Usage;
}

GLenum XVerse::ToGLBufferUsage(XVerse::EBufferUsage InUsage)
{
    GLenum Usage = GL_DYNAMIC_DRAW;
    switch (InUsage)
    {
    case XVerse::EBufferUsage::BU_STREAM_DRAW:Usage  = GL_STREAM_DRAW; break;
        case XVerse::EBufferUsage::BU_STREAM_READ:Usage  = GL_STREAM_READ; break;
        case XVerse::EBufferUsage::BU_STREAM_COPY:Usage  = GL_STREAM_COPY; break;
        case XVerse::EBufferUsage::BU_STATIC_DRAW:Usage  = GL_STATIC_DRAW; break;
        case XVerse::EBufferUsage::BU_STATIC_READ:Usage  = GL_STATIC_READ; break;
        case XVerse::EBufferUsage::BU_STATIC_COPY:Usage  = GL_STATIC_COPY; break;
        case XVerse::EBufferUsage::BU_DYNAMIC_DRAW:Usage  = GL_DYNAMIC_DRAW; break;
        case XVerse::EBufferUsage::BU_DYNAMIC_READ:Usage  = GL_DYNAMIC_READ; break;
        case XVerse::EBufferUsage::BU_DYNAMIC_COPY:Usage  = GL_DYNAMIC_COPY; break;
        case XVerse::EBufferUsage::BU_MAX_COUNT:
    default:
        break;
    }

    return Usage;
}


GLenum XVerse::ToGLAccessPolicy(XVerse::EAccessPolicy InPolicy)
{
    GLenum Policy;
    switch (InPolicy)
    {
    case XVerse::EAccessPolicy::AP_READ_ONLY:Policy = GL_READ_ONLY; break;
    case XVerse::EAccessPolicy::AP_WRITE_ONLY:Policy = GL_WRITE_ONLY; break;
    case XVerse::EAccessPolicy::AP_READ_WRITE:Policy = GL_READ_WRITE; break;
    case XVerse::EAccessPolicy::AP_MAX_COUNT:
    default:
        break;
    }
    return Policy;
}

GLenum XVerse::ToGLFormat(XVerse::EFormat InFormat)
{
    GLenum Format = GL_RGB;
    switch (InFormat)
    {
        case XVerse::EFormat::F_RED:Format = GL_RED; break;
        case XVerse::EFormat::F_RG:Format = GL_RG; break;
        case XVerse::EFormat::F_RGB:Format = GL_RGB; break;
        case XVerse::EFormat::F_BGR:Format = GL_BGR; break;
        case XVerse::EFormat::F_RGBA:Format = GL_RGBA; break;
        case XVerse::EFormat::F_BGRA:Format = GL_BGRA; break;
        case XVerse::EFormat::F_RED_INTEGER:Format = GL_RED_INTEGER; break;
        case XVerse::EFormat::F_RG_INTEGER:Format = GL_RG_INTEGER; break;
        case XVerse::EFormat::F_RGB_INTEGER:Format = GL_RGB_INTEGER; break;
        case XVerse::EFormat::F_BGR_INTEGER:Format = GL_BGR_INTEGER; break;
        case XVerse::EFormat::F_RGBA_INTEGER:Format = GL_RGBA_INTEGER; break;
        case XVerse::EFormat::F_BGRA_INTEGER:Format = GL_BGRA_INTEGER; break;
        case XVerse::EFormat::F_STENCIL_INDEX:Format = GL_STENCIL_INDEX; break;
        case XVerse::EFormat::F_DEPTH_COMPONENT:Format = GL_DEPTH_COMPONENT; break;
        case XVerse::EFormat::F_DEPTH_STENCIL:Format = GL_DEPTH_STENCIL; break;
    default:
        break;
    }
    return Format;
}

GLenum XVerse::ToGLDataType(XVerse::EDataType InDataType)
{
    GLenum DataType = GL_UNSIGNED_BYTE;
    switch (InDataType)
    {
    case XVerse::EDataType::DT_UNSIGNED_BYTE:DataType = GL_UNSIGNED_BYTE; break;
        case XVerse::EDataType::DT_BYTE:DataType = GL_BYTE; break;
        case XVerse::EDataType::DT_UNSIGNED_SHORT:DataType = GL_UNSIGNED_SHORT; break;
        case XVerse::EDataType::DT_SHORT:DataType = GL_SHORT; break;
        case XVerse::EDataType::DT_UNSIGNED_INT:DataType = GL_UNSIGNED_INT; break;
        case XVerse::EDataType::DT_INT:DataType = GL_INT; break;
        case XVerse::EDataType::DT_HALF_FLOAT:DataType = GL_HALF_FLOAT; break;
        case XVerse::EDataType::DT_FLOAT:DataType = GL_FLOAT; break;
    default:
        break;
    }
    return DataType;
}

GLenum XVerse::ToGLBlendEquation(XVerse::EBlendEquation InBlendEquation)
{
    GLenum BlendEquation;
    switch (InBlendEquation)
    {
    case XVerse::EBlendEquation::BE_FUNC_ADD:BlendEquation = GL_FUNC_ADD; break;
    case XVerse::EBlendEquation::BE_FUNC_SUBTRACT:BlendEquation = GL_FUNC_SUBTRACT; break;
    case XVerse::EBlendEquation::BE_FUNC_REVERSE_SUBTRACT:BlendEquation = GL_FUNC_REVERSE_SUBTRACT; break;
    case XVerse::EBlendEquation::BE_MIN:BlendEquation = GL_MIN; break;
    case XVerse::EBlendEquation::BE_MAX:BlendEquation = GL_MAX; break;
    case XVerse::EBlendEquation::BE_MAX_COUNT:
    default:
        break;
    }
    return BlendEquation;
}

GLenum XVerse::ToGLBlendFactor(XVerse::EBlendFactor InBlendFactor)
{
    GLenum BlendFactor = GL_ZERO;
    switch (InBlendFactor)
    {
    case XVerse::EBlendFactor::BF_ZERO:BlendFactor = GL_ZERO; break;
    case XVerse::EBlendFactor::BF_ONE:BlendFactor = GL_ONE; break;
    case XVerse::EBlendFactor::BF_SRC_COLOR:BlendFactor = GL_SRC_COLOR; break;
    case XVerse::EBlendFactor::BF_DST_COLOR:BlendFactor = GL_DST_COLOR; break;
    case XVerse::EBlendFactor::BF_ONE_MINUS_SRC_COLOR:BlendFactor = GL_ONE_MINUS_SRC_COLOR; break;
    case XVerse::EBlendFactor::BF_ONE_MINUS_DST_COLOR:BlendFactor = GL_ONE_MINUS_DST_COLOR; break;
    case XVerse::EBlendFactor::BF_SRC_ALPHA:BlendFactor = GL_SRC_ALPHA; break;
    case XVerse::EBlendFactor::BF_ONE_MINUS_SRC_ALPHA:BlendFactor = GL_ONE_MINUS_SRC_ALPHA; break;
    case XVerse::EBlendFactor::BF_DST_ALPHA:BlendFactor = GL_DST_ALPHA; break;
    case XVerse::EBlendFactor::BF_ONE_MINUS_DST_ALPHA:BlendFactor = GL_ONE_MINUS_DST_ALPHA; break;
    case XVerse::EBlendFactor::BF_CONSTANT_COLOR:BlendFactor = GL_CONSTANT_COLOR; break;
    case XVerse::EBlendFactor::BF_ONE_MINUS_CONSTANT_COLOR:BlendFactor = GL_ONE_MINUS_CONSTANT_COLOR; break;
    case XVerse::EBlendFactor::BF_CONSTANT_ALPHA:BlendFactor = GL_CONSTANT_ALPHA; break;
    case XVerse::EBlendFactor::BF_ONE_MINUS_CONSTANT_ALPHA:BlendFactor = GL_ONE_MINUS_CONSTANT_ALPHA; break;
    case XVerse::EBlendFactor::BF_SRC_ALPHA_SATURATE:BlendFactor = GL_SRC_ALPHA_SATURATE; break;
    case XVerse::EBlendFactor::BF_SRC1_COLOR:BlendFactor = GL_SRC1_COLOR; break;
    case XVerse::EBlendFactor::BF_ONE_MINUS_SRC1_COLOR:BlendFactor = GL_ONE_MINUS_SRC1_COLOR; break;
    case XVerse::EBlendFactor::BF_SRC1_ALPHA:BlendFactor = GL_SRC1_ALPHA; break;
    case XVerse::EBlendFactor::BF_ONE_MINUS_SRC1_ALPHA:BlendFactor = GL_ONE_MINUS_SRC1_ALPHA; break;
    case XVerse::EBlendFactor::BF_MAX_COUNT:
    default:
        break;
    }
    return BlendFactor;
}

GLenum XVerse::ToGLStencilOp(XVerse::EStencilOp InStencilOp)
{
    GLenum StencilOp = GL_KEEP;
    switch (InStencilOp)
    {
    case XVerse::EStencilOp::SP_KEEP:StencilOp = GL_KEEP; break;
    case XVerse::EStencilOp::SP_ZERO:StencilOp = GL_ZERO; break;
    case XVerse::EStencilOp::SP_REPLACE:StencilOp = GL_REPLACE; break;
    case XVerse::EStencilOp::SP_INCR:StencilOp = GL_INCR; break;
    case XVerse::EStencilOp::SP_INCR_WRAP:StencilOp = GL_INCR_WRAP; break;
    case XVerse::EStencilOp::SP_DECR:StencilOp = GL_DECR; break;
    case XVerse::EStencilOp::SP_DECR_WRAP:StencilOp = GL_DECR_WRAP; break;
    case XVerse::EStencilOp::SP_INVERT:StencilOp = GL_INVERT; break;
    case XVerse::EStencilOp::SP_MAX_COUNT:
    default:
        break;
    }
    return StencilOp;
}

GLenum XVerse::ToGLCullFace(XVerse::ECullFace InCullFace)
{
    GLenum CullFace = GL_BACK;
    switch (InCullFace)
    {
    case XVerse::ECullFace::CF_BACK:CullFace = GL_BACK; break;
    case XVerse::ECullFace::CF_FRONT:CullFace = GL_FRONT; break;
    case XVerse::ECullFace::CF_FRONT_AND_BACK:CullFace = GL_FRONT_AND_BACK; break;
    case XVerse::ECullFace::CF_MAX_COUNT:
    default:
        break;
    }
    return CullFace;
}

GLenum XVerse::ToGLFrontFace(XVerse::EFrontFace InFrontFace)
{
    GLenum FrontFace = GL_CW;
    switch (InFrontFace)
    {
    case XVerse::EFrontFace::FF_CCW:FrontFace = GL_CCW; break;
    case XVerse::EFrontFace::FF_CW:FrontFace = GL_CW; break;
    case XVerse::EFrontFace::FF_MAX_COUNT:
    default:
        break;
    }
    return FrontFace;
}


GLbitfield XVerse::ToGLMemoryBufferBit(EMemoryBarrierBit InBufferBit)
{
    GLbitfield BufferBit = 0;
    BufferBit |= InBufferBit & MBB_VERTEX_ATTRIB_ARRAY_BARRIER_BIT? GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT :0;
    BufferBit |= InBufferBit & MBB_ELEMENT_ARRAY_BARRIER_BIT ? GL_ELEMENT_ARRAY_BARRIER_BIT :0;
    BufferBit |= InBufferBit & MBB_UNIFORM_BARRIER_BIT ? GL_UNIFORM_BARRIER_BIT :0;
    BufferBit |= InBufferBit & MBB_TEXTURE_FETCH_BARRIER_BIT ? GL_TEXTURE_FETCH_BARRIER_BIT :0;
    BufferBit |= InBufferBit & MBB_SHADER_IMAGE_ACCESS_BARRIER_BIT ? GL_SHADER_IMAGE_ACCESS_BARRIER_BIT :0;
    BufferBit |= InBufferBit & MBB_COMMAND_BARRIER_BIT ? GL_COMMAND_BARRIER_BIT :0;
    BufferBit |= InBufferBit & MBB_PIXEL_BUFFER_BARRIER_BIT ? GL_PIXEL_BUFFER_BARRIER_BIT :0;
    BufferBit |= InBufferBit & MBB_TEXTURE_UPDATE_BARRIER_BIT ? GL_TEXTURE_UPDATE_BARRIER_BIT :0;
    BufferBit |= InBufferBit & MBB_BUFFER_UPDATE_BARRIER_BIT ? GL_BUFFER_UPDATE_BARRIER_BIT :0;
    BufferBit |= InBufferBit & MBB_FRAMEBUFFER_BARRIER_BIT ? GL_FRAMEBUFFER_BARRIER_BIT :0;
    BufferBit |= InBufferBit & MBB_TRANSFORM_FEEDBACK_BARRIER_BIT ? GL_TRANSFORM_FEEDBACK_BARRIER_BIT :0;
    BufferBit |= InBufferBit & MBB_ATOMIC_COUNTER_BARRIER_BIT ? GL_ATOMIC_COUNTER_BARRIER_BIT :0;
    BufferBit |= InBufferBit & MBB_SHADER_STORAGE_BARRIER_BIT ? GL_SHADER_STORAGE_BARRIER_BIT : 0;
    return BufferBit;
}

GLbitfield XVerse::ToGLClearBufferBit(EClearBufferBit InBufferBit)
{
    GLbitfield BufferBit = 0;
    BufferBit |= InBufferBit & CBB_COLOR_BUFFER_BIT ? GL_COLOR_BUFFER_BIT : 0;
    BufferBit |= InBufferBit & CBB_DEPTH_BUFFER_BIT ? GL_DEPTH_BUFFER_BIT : 0;
    BufferBit |= InBufferBit & CBB_STENCIL_BUFFER_BIT ? GL_STENCIL_BUFFER_BIT : 0;
    return BufferBit;
}

int XVerse::XOpenGLRHI::GetFormatCompNum(XVerse::EFormat InFormat)
{
    switch (InFormat)
    {
    case XVerse::EFormat::F_RED:return 1;
    case XVerse::EFormat::F_RG:return 2;
    case XVerse::EFormat::F_RGB:return 3;
    case XVerse::EFormat::F_BGR:return 3;
    case XVerse::EFormat::F_RGBA:return 4;
    case XVerse::EFormat::F_BGRA:return 5;
    case XVerse::EFormat::F_RED_INTEGER:return 1;
    case XVerse::EFormat::F_RG_INTEGER:return 2;
    case XVerse::EFormat::F_RGB_INTEGER:return 3;
    case XVerse::EFormat::F_BGR_INTEGER:return 3;
    case XVerse::EFormat::F_RGBA_INTEGER:return 4;
    case XVerse::EFormat::F_BGRA_INTEGER:return 4;
    case XVerse::EFormat::F_STENCIL_INDEX:return 1;
    case XVerse::EFormat::F_DEPTH_COMPONENT:return 1;
    case XVerse::EFormat::F_DEPTH_STENCIL:return 2;
    default:
        break;
    }
    return 0;
}

int XVerse::XOpenGLRHI::GetDataTypeSize(XVerse::EDataType InDataType)
{
    switch (InDataType)
    {
    case XVerse::EDataType::DT_UNSIGNED_BYTE:return sizeof(GLubyte);
    case XVerse::EDataType::DT_BYTE:return sizeof(GLbyte);
    case XVerse::EDataType::DT_UNSIGNED_SHORT:return sizeof(GLushort);
    case XVerse::EDataType::DT_SHORT:return sizeof(GLshort);
    case XVerse::EDataType::DT_UNSIGNED_INT:return sizeof(GLuint);
    case XVerse::EDataType::DT_INT:return sizeof(GLint);
    case XVerse::EDataType::DT_HALF_FLOAT:return sizeof(GLhalf);
    case XVerse::EDataType::DT_FLOAT:return sizeof(GLfloat);
    default:
        break;
    }
    return 0;
}
