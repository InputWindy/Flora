#pragma once
#include "Core.h"
#include "Texture.h"
#include <vector>
class FLORA_API FCanvas
{
protected:
	FCanvas() = default;
public:
	virtual ~FCanvas() = default;

	inline uint32_t GetW()const { return Width; };
	inline uint32_t GetH()const { return Height; };
public:
	virtual void Attach()  = 0;
	virtual void Dettach() = 0;

	virtual void ClearZ() = 0;
	virtual void ClearC() = 0;
	virtual void ClearS() = 0;
	virtual void ClearAll() = 0;

	virtual void EnableOutput(std::vector<uint8_t>&) = 0;
	virtual void BindOutput(
		/*fragment shader output*/Ref<FTexture> RenderTexture,
		/*output location*/uint8_t OutputSlot,
		/*output face(only for cube map)*/uint8_t Face = 0) = 0;
protected:
	uint32_t Width  = 0;
	uint32_t Height = 0;
};

