#include <Render/Renderer.h>

void XVerse::IRenderBase::SetOwner(IPass* InPass)
{
	InPass->SetOwner(this);
}
