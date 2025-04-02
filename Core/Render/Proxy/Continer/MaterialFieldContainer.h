#pragma once 
#include <vector>
#include <memory>
#include <Render/RHI/RHI.h>
namespace flora
{
	class XRHIShaderProgram;

    struct XMaterialFieldContainer
	{
		XMaterialFieldContainer() = default;

		void SetupMaterialState();

		std::shared_ptr<XRHIShaderProgram> ShaderProgramResource;
		XMaterialState MaterialState;
	};
}