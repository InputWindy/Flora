#pragma once
#include "Core.h"
#include "Texture.h"
class FLORA_API FRenderer
{
public:
	FRenderer(uint32_t, uint32_t);

	inline Ref<FTexture> GetBackBuffer() { return BackBuffer; };
protected:
	Ref<FTexture> BackBuffer = nullptr;
};

