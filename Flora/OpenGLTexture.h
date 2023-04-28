#pragma once
#include "Texture.h"
class FLORA_API FOpenGLTexture :public FTexture
{
public:
	/// <summary>
	/// generate an empty texture
	/// </summary>
	FOpenGLTexture(
		IN const char*,		/*Name*/
		IN uint16_t,		/*Width*/
		IN uint16_t,		/*Height*/
		IN uint16_t,		/*Depth*/
		IN ETextureTarget,	/*Target*/
		IN EInternalFormat, /*Internal*/
		IN FTextureInfo Info = FTextureInfo(),/*Texture Params*/
		IN uint32_t = 4 /*Samples*/
	);
	virtual ~FOpenGLTexture();

public:
	virtual uint32_t GetHandle()	const final;
	virtual uint32_t GetShortCut()	const final;

	virtual void Update() final;
	virtual void SetImageData(Ref<FImage> img) final;

	//cube map can not be serialized and displayed in the editor now ( but it work )
	[[deprecated]] virtual void SetCubeImageData(Ref<FImage> img, ETextureTarget target = ETextureTarget_MAX) final;
public:
	uint32_t Texture  = 0;
	uint32_t ShortCut = 0;
};

