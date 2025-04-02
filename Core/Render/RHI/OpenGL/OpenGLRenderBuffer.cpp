#include "OpenGLRenderBuffer.h"
#include <Render/RHI/OpenGL/OpenGLRHI.h>

XVerse::XOpenGLRenderBuffer::~XOpenGLRenderBuffer()
{
    ReleaseRHI();
}

bool XVerse::XOpenGLRenderBuffer::IsValid()
{
    Bind();
    return glIsRenderbuffer(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLRenderBuffer::InitRHI()
{
    glGenRenderbuffers(1, &Handle);
    return IsValid();
}

bool XVerse::XOpenGLRenderBuffer::ReleaseRHI()
{
    glDeleteRenderbuffers(1, &Handle);
    return true;
}

bool XVerse::XOpenGLRenderBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool XVerse::XOpenGLRenderBuffer::UpdateRHI()
{
    Bind();
    glRenderbufferStorage(GL_RENDERBUFFER, XVerse::ToGLInternalFormat(InternalFormat), SizeX, SizeY);
    return true;
}

void XVerse::XOpenGLRenderBuffer::Bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, Handle);
}

void XVerse::XOpenGLRenderBuffer::UnBind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
