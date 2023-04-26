#pragma once
#include "Texture.h"
class FLORA_API FOpenGLTexture :public FTexture
{
public:
	FOpenGLTexture() = default;
	static Ref<FOpenGLTexture> Generate(const char*, uint16_t, uint16_t, uint16_t, ETextureTarget, EInternalFormat, FTextureInfo);
	static Ref<FOpenGLTexture> Generate(FImage, bool bMultisample = false, FTextureInfo TexInfo = FTextureInfo());
	static Ref<FOpenGLTexture> Generate(uint32_t, uint32_t);

	virtual ~FOpenGLTexture();

	virtual uint32_t		GetHandle()			 const override;
	virtual uint32_t		GetShortCut()		 const override;

	virtual void			Update(FTextureInfo) override;
	
	virtual void			SetData(FImage) override;
	virtual void			SetData(ETextureTarget, FImage) override;

	virtual void			Reload()override;
public:
	uint32_t Texture  = 0;
	uint32_t ShortCut = 0;
};

