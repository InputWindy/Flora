#pragma once 
#include <Common/Layout.h>
#include <vector>
#include <memory>
#include <set>

#include <Render/RHI/RHI.h>

namespace flora
{
	class XRHISampler;
	class XRHITexture;

	struct XTextureFieldContainer
	{
		XTextureFieldContainer() = default;

		std::shared_ptr<XRHITexture> TextureResource;
	};
}