#include "RenderCommand.h"
#include "OpenGLInterface.h"
FRenderCommand* FRenderCommand::Create(ERenderCommandType Type)
{
	FRenderCommand* Res = nullptr;
	switch (Type)
	{
	case E_RHI_OpenGL:Res = new FOpenGLInterface; break;
	case E_RHI_Vulkan:assert(0, "Unimpl Vk Interface"); break;
	case E_RHI_Dx12:assert(0, "Unimpl Dx12 Interface"); break;
	case E_RHI_Metal:assert(0, "Unimpl MT Interface"); break;
	default:assert(0, "Undefined Render Interface"); break;
	}
	//Res->InitResources();
    return Res;
}
