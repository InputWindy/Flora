#include "Texture.h"
#include "OpenGLTexture.h"
#include "OpenGLInterface.h"
#include "ResourceManager.h"
#include "Application.h"
#include "Console.h"
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
using namespace std;

FImage::FImage(const std::string& root, const std::string& relative, bool b_hdr, bool b_flip)
	:root_path(root), relative_path(relative)
{
	std::filesystem::path path(relative);
	name = path.stem().generic_string();
	extension = path.extension().generic_string();
	directory = relative.substr(0, relative.find_last_of('/'));

	Load(b_hdr, b_flip);
}

FImage::~FImage()
{
	Free();
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

bool FImage::Parse(IN FJson& In)
{
	root_path = In["RootPath"].asString();
	directory = In["Directory"].asString();
	relative_path = In["RelativePath"].asString();
	name = In["Name"].asString();
	extension = In["Extension"].asString();

	w = In["Width"].asInt();
	h = In["Height"].asInt();
	comp = In["Component"].asInt();

	hdr = In["Hdr"].asBool();
	flip = In["Flip"].asBool();

	Load(hdr,flip);

	/*FJson& Data = In["Data"];
	if (hdr)
	{
		int data_num = w * h * comp;
		int id = 0;

		float* p_f = (float*)malloc(sizeof(float) * data_num);
		for (auto& d : Data)
		{
			p_f[id++] = d.as<float>();
		}
	}
	else
	{
		int data_num = w * h * comp;
		int id = 0;

		unsigned char* p_uc = (unsigned char*)malloc(sizeof(unsigned char) * data_num);
		for (auto& d : Data)
		{
			p_uc[id++] = d.asUInt();
		}
	}*/

	return true;
}

bool FImage::Serialize(OUT FJson& Out)
{
	Out["RootPath"] = root_path;
	Out["Directory"] = directory;
	Out["RelativePath"] = relative_path;
	Out["Name"] = name;
	Out["Extension"] = extension;

	Out["Width"] = w;
	Out["Height"] = h;
	Out["Component"] = comp;

	Out["Hdr"] = hdr;
	Out["Flip"] = flip;
	//Out["DataNum"] = w * h * comp;

	/*FJson& Data = Out["Data"];
	if (hdr)
	{
		float* p_f = (float*)data;
		for (size_t i = 0; i < w * h * comp; i++)
		{
			Data.append(p_f[i]);
		}
		data = p_f;
	}
	else
	{
		unsigned char* p_uc = (unsigned char*)data;
		for (size_t i = 0; i < w * h * comp; i++)
		{
			Data.append(p_uc[i]);
		}
		data = p_uc;
	}*/

	return true;
}

bool FTexture::Parse(IN FJson& in)
{
	FRenderCommand* Rhi = FApplication::GetRHI();
	Rename(in["Name"].asString());

	Width  = in["Width"].asInt();
	Height = in["Height"].asInt();
	Depth  = in["Depth"].asInt();

	TextureTarget = Rhi->StringToTextureTarget(in["TextureTarget"].asCString());
	InternalFormat = Rhi->StringToInternalFormat(in["InternalFormat"].asCString());

	Info.MaxMipLevel = in["MaxMipLevel"].asInt();
	Info.MinMipLevel = in["MinMipLevel"].asInt();

	Info.WrapModeR = Rhi->StringToWrapMode(in["WrapModeR"].asCString());
	Info.WrapModeS = Rhi->StringToWrapMode(in["WrapModeS"].asCString());
	Info.WrapModeT = Rhi->StringToWrapMode(in["WrapModeT"].asCString());

	Info.MinFilterMode = Rhi->StringToFilterMode(in["MinFilterMode"].asCString());
	Info.MagFilterMode = Rhi->StringToFilterMode(in["MinFilterMode"].asCString());

	if (in["ImageDesc"].isNull() == false)
	{
		Image = make_shared<FImage>();
		Image->Parse(in["ImageDesc"]);
		SetImageData(Image);
	}

	return true;
}

bool FTexture::Serialize(OUT FJson& out)
{
	FRenderCommand* Rhi = FApplication::GetRHI();

	out["Name"] = Name;
	out["Width"] = Width;
	out["Height"] = Height;
	out["Depth"] = Depth;

	out["TextureTarget"] = Rhi->TextureTargetToString(TextureTarget);
	out["InternalFormat"] = Rhi->InternalFormatToString(InternalFormat);


	out["MaxMipLevel"] = Info.MaxMipLevel;
	out["MinMipLevel"] = Info.MinMipLevel;

	out["WrapModeR"] = Rhi->WrapModeToString(Info.WrapModeR);
	out["WrapModeS"] = Rhi->WrapModeToString(Info.WrapModeS);
	out["WrapModeT"] = Rhi->WrapModeToString(Info.WrapModeT);

	out["MinFilterMode"] = Rhi->FilterModeToString(Info.MinFilterMode);
	out["MagFilterMode"] = Rhi->FilterModeToString(Info.MagFilterMode);

	//image info
	FJson& img_info = out["ImageDesc"];
	if (Image)
	{
		Image->Serialize(img_info);
	}
	return true;
}

void FTexture::Register()
{
	FResourceManager& ResourceManager = FResourceManager::Get();
	if (ResourceManager.FindObject<FTexture>(LastName))
	{
		ResourceManager.RemoveObject<FTexture>(LastName);
	}
	ResourceManager.Register<FTexture>(shared_from_this());
}

FTexture::FTexture()
{
	Type = EResourceType::Texture;
}

void FTexture::Rename(const string& name)
{
	LastName = Name;
	Name = name;
	CachePath = "/Cache/Texture/" + Name + ".ftexture";
}
