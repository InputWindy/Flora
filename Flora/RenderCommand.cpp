#include "RenderCommand.h"
#include "OpenGLInterface.h"
FRenderCommand* FRenderCommand::Create(ERenderCommandType Type)
{
	switch (Type)
	{
	case E_RHI_OpenGL:return new FOpenGLInterface;
	case E_RHI_Vulkan:
	case E_RHI_Dx12:
	case E_RHI_Metal:
	default:
		break;
	}
    return nullptr;
}
