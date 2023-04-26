#include "Texture.h"
#include "OpenGLTexture.h"
#include "OpenGLInterface.h"
#include "ResourceManager.h"
#include "Application.h"
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
using namespace std;
FImage::FImage(const std::string& root, const std::string& relative)
	:root_path(root), relative_path(relative)
{
	std::filesystem::path path(relative);
	name = path.stem().generic_string();
	extension = path.extension().generic_string();
	directory = relative.substr(0, relative.find_last_of('/'));
}

FImage::~FImage()
{
	
}

void FImage::Load(bool b_hdr, bool b_flip)
{
	hdr = b_hdr;
	flip = b_flip;

	stbi_set_flip_vertically_on_load(b_flip);

	string filepath = root_path + relative_path;
	data = b_hdr ? stbi_loadf(filepath.c_str(), &w, &h, &comp, 0) : (void*)stbi_load(filepath.c_str(), &w, &h, &comp, 0);
}

void FImage::Free()
{
	if (data)
	{
		stbi_image_free(data);
		data = nullptr;
	}
}

FTexture::FTexture()
{
	Type = EResourceType::Texture;
}

bool FTexture::Parse(IN FJson& in)
{
	FRenderCommand* Rhi = FApplication::GetRHI();

	Name   = in["Name"].asString();
	Width  = in["Width"].asInt();
	Height = in["Height"].asInt();
	Depth  = in["Depth"].asInt();
	
	RootPath = in["RootPath"].asString();
	Directory = in["Directory"].asString();
	RelativePath = in["RelativePath"].asString();
	Extension = in["Extension"].asString();

	bHdr = in["bHdr"].asBool();
	bFlip = in["bFlip"].asBool();
	
	TextureTarget = Rhi->StringToTextureTarget(in["TextureTarget"].asCString());
	InternalFormat = Rhi->StringToInternalFormat(in["InternalFormat"].asCString());

	Info.MaxMipLevel = in["MaxMipLevel"].asInt();
	Info.MinMipLevel = in["MinMipLevel"].asInt();

	Info.WrapModeR = Rhi->StringToWrapMode(in["WrapModeR"].asCString());
	Info.WrapModeS = Rhi->StringToWrapMode(in["WrapModeS"].asCString());
	Info.WrapModeT = Rhi->StringToWrapMode(in["WrapModeT"].asCString());

	Info.MinFilterMode = Rhi->StringToFilterMode(in["MinFilterMode"].asCString());
	Info.MagFilterMode = Rhi->StringToFilterMode(in["MinFilterMode"].asCString());

	Reload();
	return true;
}

bool FTexture::Serialize(OUT FJson& out)
{
	FRenderCommand* Rhi = FApplication::GetRHI();

	out["Name"] = Name;
	out["Width"] = Width;
	out["Height"] = Height;
	out["Depth"] = Depth;

	out["RootPath"] = RootPath;
	out["Directory"] = Directory;
	out["RelativePath"] = RelativePath;
	out["Extension"] = Extension;

	out["bHdr"] = bHdr;
	out["bFlip"] = bFlip;

	out["TextureTarget"] = Rhi->TextureTargetToString(TextureTarget);
	out["InternalFormat"] = Rhi->TextureTargetToString(InternalFormat);


	out["MaxMipLevel"] = Info.MaxMipLevel;
	out["MinMipLevel"] = Info.MinMipLevel;

	out["WrapModeR"] = Rhi->WrapModeToString(Info.WrapModeR);
	out["WrapModeS"] = Rhi->WrapModeToString(Info.WrapModeS);
	out["WrapModeT"] = Rhi->WrapModeToString(Info.WrapModeT);

	out["MinFilterMode"] = Rhi->FilterModeToString(Info.MinFilterMode);
	out["MagFilterMode"] = Rhi->FilterModeToString(Info.MagFilterMode);
	return true;
}

void FTexture::Register()
{
	FResourceManager& ResourceManager = FResourceManager::Get();
	if (!ResourceManager.FindObject<FTexture>(Name))
	{
		ResourceManager.Register<FTexture>(shared_from_this());
	};
}


Ref<FTexture> FTexture::Generate(const char* Name, uint16_t W, uint16_t H, uint16_t Z, ETextureTarget Type, EInternalFormat InForm, FTextureInfo Info)
{
	return FOpenGLTexture::Generate(Name,W,H,Z,Type, InForm,Info);
};
Ref<FTexture> FTexture::Generate(FImage img, bool bMultisample, FTextureInfo TexInfo)
{
	return FOpenGLTexture::Generate(img, bMultisample, TexInfo);
};
Ref<FTexture> FTexture::Generate(uint32_t shortcut, uint32_t handle)
{
	return FOpenGLTexture::Generate(shortcut,handle);
};