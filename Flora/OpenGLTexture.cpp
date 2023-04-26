#include "OpenGLTexture.h"
#include "OpenGLInterface.h"
#include <assert.h>
#include <glad.h>
Ref<FOpenGLTexture> FOpenGLTexture::Generate(const char* Name, uint16_t W, uint16_t H, uint16_t Z, ETextureTarget Type, EInternalFormat InForm, FTextureInfo info)
{
	Ref<FOpenGLTexture> Res = make_shared<FOpenGLTexture>();

	Res->Name = Name;
	Res->Width = W;
	Res->Height = H;
	Res->Depth = Z;
	Res->TextureTarget = Type;
	Res->InternalFormat = InForm;
	Res->ShortCut = 0;

	GLenum texture_type = FOpenGLInterface::TextureTarget[Type];

	glCreateTextures(texture_type, 1, &Res->Texture);
	glBindTexture(texture_type, Res->Texture);
	if (texture_type == GL_TEXTURE_CUBE_MAP)
	{
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, info.MinMipLevel, FOpenGLInterface::InternalFormat[InForm], Res->Width, Res->Height, 0, GL_RGBA, GL_FLOAT, nullptr);
		}
	}
	else if (texture_type == GL_TEXTURE_1D)
	{
		glTexImage1D(texture_type, info.MinMipLevel, FOpenGLInterface::InternalFormat[InForm], Res->Width, 0, GL_RGBA, GL_FLOAT, nullptr);
	}
	else if (texture_type == GL_TEXTURE_2D_MULTISAMPLE || texture_type == GL_TEXTURE_2D)
	{
		glTexImage2D(texture_type, info.MinMipLevel, FOpenGLInterface::InternalFormat[InForm], Res->Width, Res->Height, 0, GL_RGBA, GL_FLOAT, nullptr);
	}
	else if (texture_type == GL_TEXTURE_2D_MULTISAMPLE_ARRAY || texture_type == GL_TEXTURE_3D)
	{
		glTexImage3D(texture_type, info.MinMipLevel, FOpenGLInterface::InternalFormat[InForm], Res->Width, Res->Height, Res->Depth, 0, GL_RGBA, GL_FLOAT, nullptr);
	}
	else assert(0);

	glBindTexture(texture_type, 0);

	Res->Update(info);

	return Res;
}

Ref<FOpenGLTexture> FOpenGLTexture::Generate(FImage img, bool bMultisample, FTextureInfo TexInfo)
{
	if (!img.data)return nullptr;

	Ref<FOpenGLTexture> Res = Generate(img.name.c_str(), img.w, img.h, 0,
		bMultisample ? ETextureTarget_2D_Multisample : ETextureTarget_2D,
		img.hdr ? EInternalFormat_RGBA16F : EInternalFormat_RGBA, TexInfo);

	Res->SetData(img);

	Res->ShortCut = Res->Texture;

	return Res;
}

Ref<FOpenGLTexture> FOpenGLTexture::Generate(uint32_t shortcut, uint32_t handle)
{
	Ref<FOpenGLTexture> Res = make_shared<FOpenGLTexture>();
	Res->ShortCut = shortcut;
	Res->Texture = handle;
	return Res;
}

FOpenGLTexture::~FOpenGLTexture()
{
	glDeleteTextures(1, &Texture);
	//glDeleteTextures(1, &ShortCut);
}

uint32_t FOpenGLTexture::GetHandle() const
{
	return Texture;
}

uint32_t FOpenGLTexture::GetShortCut() const
{
	return ShortCut;
}

void FOpenGLTexture::Update(FTextureInfo info)
{
	Info.WrapModeR = info.WrapModeR;
	Info.WrapModeS = info.WrapModeS;
	Info.WrapModeT = info.WrapModeT;

	Info.MinFilterMode = info.MinFilterMode;
	Info.MagFilterMode = info.MagFilterMode;

	Info.MaxMipLevel = info.MaxMipLevel;
	Info.MinMipLevel = info.MinMipLevel;

	GLenum texture_type = FOpenGLInterface::TextureTarget[TextureTarget];

	GLenum wrap_r = 0, wrap_s = 0, wrap_t = 0;
	GLenum min_filter = 0, mag_filter = 0;
	{
		mag_filter = FOpenGLInterface::FilterMode[Info.MagFilterMode];
		min_filter = FOpenGLInterface::FilterMode[Info.MinFilterMode];

		wrap_r = FOpenGLInterface::WrapMode[Info.WrapModeR];
		wrap_s = FOpenGLInterface::WrapMode[Info.WrapModeS];
		wrap_t = FOpenGLInterface::WrapMode[Info.WrapModeT];
	}
	glBindTexture(texture_type, Texture);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_S, (GLenum)wrap_s);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_T, (GLenum)wrap_t);
	glTexParameteri(texture_type, GL_TEXTURE_WRAP_R, (GLenum)wrap_r);

	glTexParameteri(texture_type, GL_TEXTURE_MIN_FILTER, (GLenum)min_filter);
	glTexParameteri(texture_type, GL_TEXTURE_MAG_FILTER, (GLenum)mag_filter);

	glTexParameteri(texture_type, GL_TEXTURE_BASE_LEVEL, (GLenum)Info.MinMipLevel);
	glTexParameteri(texture_type, GL_TEXTURE_MAX_LEVEL, (GLenum)Info.MaxMipLevel);
	glBindTexture(texture_type, 0);
}

void FOpenGLTexture::SetData(FImage img)
{
	RootPath     = img.root_path;
	Directory    = img.directory;
	RelativePath = img.relative_path;
	Extension	 = img.extension;
	

	bHdr  = img.hdr;
	bFlip = img.flip;

	GLenum texture_type = FOpenGLInterface::TextureTarget[TextureTarget];

	GLenum format = 0;
	switch (img.comp)
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
		FOpenGLInterface::InternalFormat[InternalFormat], Width, Height, 0, format, img.hdr ? GL_FLOAT : GL_UNSIGNED_BYTE, (img.data));
	glBindTexture(texture_type, 0);
}
void FOpenGLTexture::SetData(ETextureTarget target, FImage img)
{
	RootPath     = img.root_path;
	Directory    = img.directory;
	RelativePath = img.relative_path;
	Extension    = img.extension;

	bHdr  = img.hdr;
	bFlip = img.flip;

	GLenum texture_type = FOpenGLInterface::TextureTarget[TextureTarget];
	GLenum texture_target = FOpenGLInterface::TextureTarget[target];

	GLenum format = 0;
	switch (img.comp)
	{
	case 1:format = GL_RED; break;
	case 2:format = GL_RG; break;
	case 3:format = GL_RGB; break;
	case 4:format = GL_RGBA; break;
	default:
		break;
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, Texture);
	for (unsigned int i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, Info.MinMipLevel, FOpenGLInterface::InternalFormat[InternalFormat], Width, Height, 0, format, img.hdr ? GL_FLOAT : GL_UNSIGNED_BYTE, (img.data));
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void FOpenGLTexture::Reload()
{
	glDeleteTextures(1, &Texture);

	FImage img(RootPath, RelativePath);
	img.Load(bHdr, bFlip);

	Ref<FOpenGLTexture> Tmp = FOpenGLTexture::Generate(img,false,Info);

	Texture = Tmp->Texture;
	ShortCut = Tmp->ShortCut;

	CachePath	 = "/Cache/Texture/" + img.name + ".ftexture";

	Tmp->Texture = 0;
	Tmp->ShortCut = 0;

	img.Free();

}
