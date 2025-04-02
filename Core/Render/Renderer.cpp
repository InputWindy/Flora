#include <Render/Renderer.h>

void flora::IRenderBase::SetOwner(IPass* InPass)
{
	InPass->SetOwner(this);
}
