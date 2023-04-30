#include "OpenGLCanvas.h"
#include "OpenGLInterface.h"
#include <glad.h>
FOpenGLCanvas::FOpenGLCanvas(uint32_t w, uint32_t h)
{
	Width = w;
	Height = h;

	glGenFramebuffers(1, &FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

	glGenRenderbuffers(1, &ZSBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, ZSBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, ZSBuffer);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		glDeleteFramebuffers(1, &FrameBuffer);
		glDeleteRenderbuffers(1, &ZSBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FOpenGLCanvas::~FOpenGLCanvas()
{
	glDeleteFramebuffers(1, &FrameBuffer);
	glDeleteRenderbuffers(1, &ZSBuffer);
}

void FOpenGLCanvas::Attach()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
}

void FOpenGLCanvas::Dettach()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FOpenGLCanvas::ClearZ()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void FOpenGLCanvas::ClearC()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void FOpenGLCanvas::ClearS()
{
	glClear(GL_STENCIL_BUFFER_BIT);
}

void FOpenGLCanvas::ClearAll()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void FOpenGLCanvas::EnableOutput(std::vector<uint8_t>& output)
{
	std::vector<GLenum> Buffers;
	for (auto& o : output)
	{
		Buffers.push_back(GL_COLOR_ATTACHMENT0 + o);
	};
	glDrawBuffers(Buffers.size(), Buffers.data());
}

void FOpenGLCanvas::BindOutput(Ref<FTexture> RenderTexture, uint8_t OutputSlot, uint8_t Face)
{
	if (!RenderTexture)return;

	switch (RenderTexture->GetTextureType())
	{
	case ETextureTarget_1D: {glFramebufferTexture1D(GL_FRAMEBUFFER, FOpenGLInterface::ColorAttachment[OutputSlot], GL_TEXTURE_1D, RenderTexture->GetHandle(), 0); break; };
	case ETextureTarget_2D: {glFramebufferTexture2D(GL_FRAMEBUFFER, FOpenGLInterface::ColorAttachment[OutputSlot], GL_TEXTURE_2D, RenderTexture->GetHandle(), 0); break; };
	case ETextureTarget_2D_Multisample: {glFramebufferTexture2D(GL_FRAMEBUFFER, FOpenGLInterface::ColorAttachment[OutputSlot], GL_TEXTURE_2D_MULTISAMPLE, RenderTexture->GetHandle(), 0); break; };
	case ETextureTarget_3D:break;
	case ETextureTarget_3D_Multisample:break;
	case ETextureTarget_Cubic:break;
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, FOpenGLInterface::ColorAttachment[OutputSlot], GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face, RenderTexture->GetHandle(), 0);
		break;
	}
	default:assert(0, "Undefined RenderTarget");break;
	}
}
