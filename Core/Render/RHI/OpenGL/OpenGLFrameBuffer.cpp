#include "OpenGLFrameBuffer.h"
#include "OpenGLRenderBuffer.h"
#include "OpenGLTexture.h"

flora::XOpenGLFrameBuffer::~XOpenGLFrameBuffer()
{
    ReleaseRHI();
}

bool flora::XOpenGLFrameBuffer::IsValid()
{
    Bind();
    return glIsFramebuffer(Handle) == GL_TRUE;
}

bool flora::XOpenGLFrameBuffer::InitRHI()
{
    glGenFramebuffers(1, &Handle);
    Bind();
    if (DepthStencilAttachment && DepthStencilAttachment->IsValid())
    {
        DepthStencilAttachment->Bind();
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, std::dynamic_pointer_cast<XOpenGLRenderBuffer>(DepthStencilAttachment)->GetHandle());
    }
    return IsValid() && IsComplete();
}

bool flora::XOpenGLFrameBuffer::ReleaseRHI()
{
    glDeleteFramebuffers(1, &Handle);
    return true;
}

bool flora::XOpenGLFrameBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool flora::XOpenGLFrameBuffer::UpdateRHI()
{
    Bind();

    glViewport(0, 0, DepthStencilAttachment->GetSizeX(), DepthStencilAttachment->GetSizeY());

    int idx = 0;

    for (size_t idx = 0; idx < ColorAttachments.size(); ++idx)
    {
        auto& Attachment = ColorAttachments[idx];
        switch (Attachment.ColorBuffer->GetType())
        {
        //case GL_TEXTURE_1D:break;
        case ERHIResourceType::RT_Texture2D:
        {
            std::shared_ptr<XOpenGLTexture2D> opengTemTexture = std::dynamic_pointer_cast<XOpenGLTexture2D>(Attachment.ColorBuffer);
            if (opengTemTexture && opengTemTexture->IsValid())
            {
                opengTemTexture->Bind();
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_2D, opengTemTexture->GetHandle(), Attachment.MipLevel);
            }
            break;
        }
        case ERHIResourceType::RT_TextureCube:
        {
            std::shared_ptr<XOpenGLTextureCube> opengTemTexture = std::dynamic_pointer_cast<XOpenGLTextureCube>(Attachment.ColorBuffer);
            if (opengTemTexture && opengTemTexture->IsValid())
            {
                opengTemTexture->Bind();
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + idx, GL_TEXTURE_CUBE_MAP_POSITIVE_X + Attachment.CubeFace, opengTemTexture->GetHandle(), Attachment.MipLevel);
            }
            break;
        }
        /*case GL_TEXTURE_3D:break;
        case GL_TEXTURE_1D_ARRAY:break;
        case GL_TEXTURE_2D_ARRAY:break;
        case GL_TEXTURE_RECTANGLE:break;
        case GL_TEXTURE_CUBE_MAP:break;
        case GL_TEXTURE_CUBE_MAP_ARRAY:break;
        case GL_TEXTURE_BUFFER:break;
        case GL_TEXTURE_2D_MULTISAMPLE:break;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:break;*/
        default:
            break;
        }
    }

    std::vector<GLenum> GLDrawBuffers;
    for (size_t i = 0; i < DrawBuffers.size(); i++)
    {
        GLDrawBuffers.push_back(GL_COLOR_ATTACHMENT0 + DrawBuffers[i]);
    }

    glDrawBuffers(GLDrawBuffers.size(), GLDrawBuffers.data());

    UnBind();
    return true;
}

void flora::XOpenGLFrameBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, Handle);

}

void flora::XOpenGLFrameBuffer::UnBind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

bool flora::XOpenGLFrameBuffer::IsComplete()
{
    Bind();
    return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

//void flora::XOpenGLFrameBuffer::ClearDepthStencil(float d, int i)
//{
//    Bind();
//    glClearBufferfi(GL_DEPTH_STENCIL, 0, d, i);
//}

void flora::XOpenGLFrameBuffer::ClearBuffer(EClearBufferBit InBufferBit)
{
    glClear(ToGLClearBufferBit(InBufferBit));
}

void flora::XOpenGLFrameBuffer::ClearColor(int i, float r, float g, float b, float a)
{
	Bind();
    const GLfloat Color[4] = { r, g, b, a };

    glClearBufferfv(GL_COLOR, i, Color);
}
