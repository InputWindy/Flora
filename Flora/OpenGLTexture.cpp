#include "OpenGLTexture.h"
#include "OpenGLInterface.h"
#include <assert.h>
#include <glad.h>

FOpenGLTexture::FOpenGLTexture(IN const char* name, IN uint16_t w, IN uint16_t h, IN uint16_t z, 
	IN ETextureTarget target, IN EInternalFormat inform, IN FTextureInfo info ,IN uint32_t samples)
{
	bool bValid = name and target != ETextureTarget_MAX;

	assert(bValid,"Apply For An InValid Texture!");

	if (bValid)
	{
		Rename(name);

		Width = w;
		Height = h;
		Depth = z;
		TextureTarget = target;
		InternalFormat = inform;

		MultisampleNum = samples;

		Info.MaxMipLevel = info.MaxMipLevel;
		Info.MinMipLevel = info.MinMipLevel;

		Info.WrapModeR = info.WrapModeR;
		Info.WrapModeS = info.WrapModeS;
		Info.WrapModeT = info.WrapModeT;

		Info.MinFilterMode = info.MinFilterMode;
		Info.MagFilterMode = info.MagFilterMode;

		Info.BorderColor = info.BorderColor;

		GLenum texture_type = FOpenGLInterface::TextureTarget[TextureTarget];

		glCreateTextures(texture_type, 1, &Texture);
		glBindTexture(texture_type, Texture);

		switch (texture_type)
		{
		case GL_TEXTURE_1D:
		{
			glTexImage1D(GL_TEXTURE_1D, info.MinMipLevel, FOpenGLInterface::InternalFormat[InternalFormat],Width, 0, GL_RGBA, GL_FLOAT, nullptr);
			break;
		}
		case GL_TEXTURE_2D:
		{
			glTexImage2D(GL_TEXTURE_2D, info.MinMipLevel, FOpenGLInterface::InternalFormat[InternalFormat],Width, Height, 0, GL_RGBA, GL_FLOAT, nullptr);
			break;
		}
		case GL_TEXTURE_2D_MULTISAMPLE:
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MultisampleNum, FOpenGLInterface::InternalFormat[InternalFormat],Width,Height,true);
			break;
		}
		case GL_TEXTURE_3D:
		{
			glTexImage3D(GL_TEXTURE_3D, info.MinMipLevel, FOpenGLInterface::InternalFormat[InternalFormat], Width, Height, Depth, 0, GL_RGBA, GL_FLOAT, nullptr);
			break;
		}
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY:
		{
			glTexImage3DMultisample(GL_TEXTURE_2D_MULTISAMPLE_ARRAY,MultisampleNum, FOpenGLInterface::InternalFormat[InternalFormat], Width, Height, Depth,true);
			break;
		}
		case GL_TEXTURE_CUBE_MAP:
		{
			for (unsigned int Face = 0; Face < 6; ++Face)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + Face, info.MinMipLevel, FOpenGLInterface::InternalFormat[InternalFormat], Width, Height, 0, GL_RGBA, GL_FLOAT, nullptr);
			}
			break;
		}
		default:assert(0, "InValid Texture Target!"); break;
		}

		Update();

		ShortCut = Texture;

		glBindTexture(texture_type, 0);

		assert(Texture,"Fail To Generate Texture");
	}
}

FOpenGLTexture::~FOpenGLTexture()
{
	glDeleteTextures(1, &Texture);
	Image = nullptr;
}

uint32_t FOpenGLTexture::GetHandle() const
{
	return Texture;
}

uint32_t FOpenGLTexture::GetShortCut() const
{
	return ShortCut;
}

void FOpenGLTexture::Update()
{
	GLenum texture_type = FOpenGLInterface::TextureTarget[TextureTarget];

	glBindTexture(texture_type, Texture);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, (GLenum)FOpenGLInterface::WrapMode[Info.WrapModeR]);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, (GLenum)FOpenGLInterface::WrapMode[Info.WrapModeS]);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_R, (GLenum)FOpenGLInterface::WrapMode[Info.WrapModeT]);

	glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, (GLenum)FOpenGLInterface::FilterMode[Info.MagFilterMode]);
	glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, (GLenum)FOpenGLInterface::FilterMode[Info.MinFilterMode]);

	glTexParameteri(texture_type, GL_TEXTURE_BASE_LEVEL, (GLenum)Info.MinMipLevel);
	glTexParameteri(texture_type, GL_TEXTURE_MAX_LEVEL, (GLenum)Info.MaxMipLevel);

	glTexParameterfv(texture_type, GL_TEXTURE_BORDER_COLOR, &(Info.BorderColor[0]));

	glBindTexture(texture_type, 0);
}

void FOpenGLTexture::SetCubeImageData(Ref<FImage> img, ETextureTarget target)
{
	bool bValid = target >= ETextureTarget_Cubic_NX and target <= ETextureTarget_Cubic_PZ;
	assert(bValid, "InValid Cubemap Face");
	if (bValid)
	{
		GLenum format = 0;
		switch (img->comp)
		{
		case 1:format = GL_RED; break;
		case 2:format = GL_RG; break;
		case 3:format = GL_RGB; break;
		case 4:format = GL_RGBA; break;
		default:
			break;
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + target - ETextureTarget_Cubic_NX,
			Info.MinMipLevel, FOpenGLInterface::InternalFormat[InternalFormat], Width, Height, 0, format, img->hdr ? GL_FLOAT : GL_UNSIGNED_BYTE, (img->data));
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}

void FOpenGLTexture::SetImageData(Ref<FImage> img)
{
	Image = img;

	Width = img->w;
	Height = img->h;

	GLenum texture_type = FOpenGLInterface::TextureTarget[TextureTarget];
	GLenum format = 0;
	switch (img->comp)
	{
	case 1:format = GL_RED; break;
	case 2:format = GL_RG; break;
	case 3:format = GL_RGB; break;
	case 4:format = GL_RGBA; break;
	default:
		break;
	}

	glBindTexture(texture_type, Texture);
	glTexImage2D(texture_type, Info.MinMipLevel,
		FOpenGLInterface::InternalFormat[InternalFormat], Width, Height, 0, format, img->hdr ? GL_FLOAT : GL_UNSIGNED_BYTE, (img->data));
	glBindTexture(texture_type, 0);
}
