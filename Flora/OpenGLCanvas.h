#pragma once
#include "Canvas.h"
class FLORA_API FOpenGLCanvas:public FCanvas
{
public:
	FOpenGLCanvas(uint32_t, uint32_t);
	virtual ~FOpenGLCanvas();
public:
	virtual void Attach() final;
	virtual void Dettach() final;

	virtual void ClearZ() final;
	virtual void ClearC() final;
	virtual void ClearS() final;
	virtual void ClearAll() final;

	virtual void EnableOutput(std::vector<uint8_t>&) final;
	virtual void BindOutput(
		/*fragment shader output*/Ref<FTexture> RenderTexture,
		/*output location*/uint8_t OutputSlot,
		/*output face(only for cube map)*/uint8_t Face = 0) final;
protected:
	uint32_t	FrameBuffer = 0;//fbo handle
	uint32_t	ZSBuffer	= 0;//depth24 + stencil8
};

