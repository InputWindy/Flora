#include "OpenGLRenderBuffer.h"
#include <Render/RHI/OpenGL/OpenGLRHI.h>

flora::XOpenGLRenderBuffer::~XOpenGLRenderBuffer()
{
    ReleaseRHI();
}

bool flora::XOpenGLRenderBuffer::IsValid()
{
    Bind();
    return glIsRenderbuffer(Handle) == GL_TRUE;
}

bool flora::XOpenGLRenderBuffer::InitRHI()
{
    glGenRenderbuffers(1, &Handle);
    return IsValid();
}

bool flora::XOpenGLRenderBuffer::ReleaseRHI()
{
    glDeleteRenderbuffers(1, &Handle);
    return true;
}

bool flora::XOpenGLRenderBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool flora::XOpenGLRenderBuffer::UpdateRHI()
{
    Bind();
    glRenderbufferStorage(GL_RENDERBUFFER, flora::ToGLInternalFormat(InternalFormat), SizeX, SizeY);
    return true;
}

void flora::XOpenGLRenderBuffer::Bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, Handle);
}

void flora::XOpenGLRenderBuffer::UnBind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
