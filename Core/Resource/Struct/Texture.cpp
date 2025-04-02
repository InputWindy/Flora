#include <Resource/Struct/Texture.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <Common/Common.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

bool IsImage(const std::string& Extension)
{
	return
		Extension == ".jpg" || Extension == ".JPG" ||
		Extension == ".jpeg" || Extension == ".JPEG" ||
		Extension == ".png" || Extension == ".PNG" ||
		Extension == ".bmp" || Extension == ".BMP" ||
		Extension == ".gif" || Extension == ".GIF" ||
		Extension == ".tga" || Extension == ".TGA" ||
		Extension == ".hdr" || Extension == ".HDR";
}

template<>
std::shared_ptr<flora::XTexture2D::BaseType> flora::XImporter<flora::Texture2DImportSettings, flora::XTexture2D::BaseType>::Exec(const std::string& InFilepath, const flora::Texture2DImportSettings& InSettings)
{
	std::shared_ptr<flora::XTexture2D> Ret = nullptr;
	
	std::filesystem::path Path(InFilepath);

	if (std::filesystem::exists(InFilepath) && IsImage(Path.extension().generic_string()))
	{
		Ret = std::make_shared<XTexture2D>(InFilepath);

		stbi_set_flip_vertically_on_load(InSettings.bIsFlipVerticallyOnLoad ? 1 : 0);
		
		bool bHdr = Path.extension().generic_string() == ".hdr" or Path.extension().generic_string() == ".HDR";

		Ret->ImageData =
			bHdr ?
			(void*)stbi_loadf(InFilepath.c_str(), &Ret->Width, &Ret->Height, &Ret->ChannelNum, 0) :
			(void*)stbi_load(InFilepath.c_str(), &Ret->Width, &Ret->Height, &Ret->ChannelNum, 0);

		Ret->DataType = bHdr ? EDataType::DT_FLOAT : EDataType::DT_UNSIGNED_BYTE;

		if (Ret->ChannelNum == 0)Ret->Format = EFormat::F_MAX_COUNT;
		if (Ret->ChannelNum == 1)Ret->Format = EFormat::F_RED;
		if (Ret->ChannelNum == 2)Ret->Format = EFormat::F_RG;
		if (Ret->ChannelNum == 3)Ret->Format = EFormat::F_RGB;
		if (Ret->ChannelNum == 4)Ret->Format = EFormat::F_RGBA;

	}

	return Ret;
}

template<>
bool flora::XExporter<flora::Texture2DExportSettings, flora::XTexture2D>::Exec(flora::XTexture2D& Texture2D, const std::string& Filepath, const flora::Texture2DExportSettings& Settings)
{
	bool Ret = false;
	std::filesystem::path Path(Filepath);

	if (std::filesystem::exists(Path.parent_path()) == false)flora::GlobalUtils::CreateDirectoryRecursive(Path.parent_path().generic_string());

	if (std::filesystem::exists(Path.parent_path()))
	{
		stbi_flip_vertically_on_write(Settings.bIsFlipVerticallyOnLoad);

		std::string Extension = Path.extension().generic_string();

		int SizeX = Texture2D.GetSizeX();
		int SizeY = Texture2D.GetSizeY();
		int ChannelNum = Texture2D.GetChannelNum();
		void* Data = Texture2D.GetImageData();

		if (Extension == ".jpg" || Extension == ".JPG")
		{
			stbi_write_jpg(Filepath.c_str(), SizeX, SizeY, ChannelNum, Data, Settings.JpgQuality);
		}
		else if (Extension == ".png" || Extension == ".PNG")
		{
			stbi_write_png(Filepath.c_str(), SizeX, SizeY, ChannelNum, Data, SizeX * ChannelNum);
		}
		else if (Extension == ".tga" || Extension == ".TGA")
		{
			stbi_write_tga(Filepath.c_str(), SizeX, SizeY, ChannelNum, Data);
		}
		else if (Extension == ".bmp" || Extension == ".BMP")
		{
			stbi_write_bmp(Filepath.c_str(), SizeX, SizeY, ChannelNum, Data);
		}
		else if (Extension == ".hdr" || Extension == ".HDR")
		{
			stbi_write_hdr(Filepath.c_str(), SizeX, SizeY, ChannelNum,(const float*)Data);
		}
		else assert(0);
	}
	return Ret;
}

template<>
bool flora::XExporter<flora::TextureCubeExportSettings, flora::XTextureCube>::Exec(flora::XTextureCube& TextureCube, const std::string& Filepath, const flora::TextureCubeExportSettings& Settings)
{
	bool Ret = false;
	std::filesystem::path Path(Filepath);

	if (std::filesystem::exists(Path.parent_path()) == false)flora::GlobalUtils::CreateDirectoryRecursive(Path.parent_path().generic_string());

	if (std::filesystem::exists(Path.parent_path()))
	{
		/*auto FacePX = XRHI::CubemapFaceToTexture2D(ECubeFace_::PX, TextureCube);
		auto FaceNX = XRHI::CubemapFaceToTexture2D(ECubeFace_::NX, TextureCube);
		auto FacePY = XRHI::CubemapFaceToTexture2D(ECubeFace_::PY, TextureCube);
		auto FaceNY = XRHI::CubemapFaceToTexture2D(ECubeFace_::NY, TextureCube);
		auto FacePZ = XRHI::CubemapFaceToTexture2D(ECubeFace_::PZ, TextureCube);
		auto FaceNZ = XRHI::CubemapFaceToTexture2D(ECubeFace_::NZ, TextureCube);*/
	}

	return Ret;
}