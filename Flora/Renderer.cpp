#include "Renderer.h"
#include "Application.h"
FRenderer::FRenderer(uint32_t Wid, uint32_t Hei)
{
	FRenderCommand* Cmd = FApplication::GetRHI();
	BackBuffer = Cmd->GenerateTexture("BackBuffer",Wid,Hei,0,ETextureTarget_2D,EInternalFormat_RGBA32F);

}
