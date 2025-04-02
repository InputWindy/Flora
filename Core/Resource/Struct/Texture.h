#pragma once
#include <Common/Refl.hpp>
#include<Common/Serialize.h>
#include <Common/GlobalType.h>
#include <Render/RHI/RHI.h>

#include <Resource/ResourceManager.h>

#include <map>
#include <string>
namespace XVerse
{
	//TextureBaseType
	class XTexture : public XVerse::IResource<>, ISerialization
	{
	protected:
		XTexture() = default;
		XTexture(const std::string& InFilePath) :XVerse::IResource<>(InFilePath) {  }

	public:
		virtual ~XTexture() = default;

		int GetMipLevels()const { return MipLevels; }
	protected:
		int MipLevels = 1;
	};
#pragma region Texture2D

	struct Texture2DIOFlags
	{
		bool bIsFlipVerticallyOnLoad = true;

		int JpgQuality = 100;//1 ~ 100 only for jpg
	};

	//IO Settings
	using Texture2DImportSettings = XImportSetting<Texture2DIOFlags, EFileType::FT_TEXTURE_2D>;
	using Texture2DExportSettings = XExportSetting<Texture2DIOFlags, EFileType::FT_TEXTURE_2D>;

	class XTexture2D :public XTexture
	{
		IMPL_RESOURCE(Texture2D)
	public:
		friend class XImporter<Texture2DImportSettings, XTexture2D::BaseType>;
		friend class XExporter<Texture2DExportSettings, XTexture2D>;

	public:
		XTexture2D() {}
		XTexture2D(const std::string& InFilePath) :XTexture(InFilePath) {  }
		virtual ~XTexture2D()
		{
			if (ImageData)
			{
				free(ImageData);
			}
		}

	public:
		void* GetImageData() { return ImageData; }
		int GetSizeX() { return Width; }
		int GetSizeY() { return Height; }
		int GetChannelNum() { return ChannelNum; }
		EFormat GetFormat() { return Format; }
		EDataType GetDataType() { return DataType; }

	protected:
		void* ImageData = nullptr;
		int Width = 0;
		int Height = 0;
		int ChannelNum = 0;

		EFormat Format = EFormat::F_RGB;
		EDataType DataType = EDataType::DT_UNSIGNED_BYTE;
	};

	struct VolumeTextureIOFlags
	{

	};

	//IO Settings
	using VolumeTextureImportSettings = XImportSetting<VolumeTextureIOFlags, EFileType::FT_TEXTURE_3D>;
	using VolumeTextureExportSettings = XExportSetting<VolumeTextureIOFlags, EFileType::FT_TEXTURE_3D>;

	class XVolumeTexture :public XTexture
	{
		IMPL_RESOURCE(VolumeTexture)
	public:
		friend class XImporter<Texture2DImportSettings, XTexture2D::BaseType>;
		friend class XExporter<Texture2DExportSettings, XTexture2D>;
	public:
		XVolumeTexture() = default;
		XVolumeTexture(const std::string& InFilePath) :XTexture(InFilePath) {  }
		virtual ~XVolumeTexture()
		{
			if (ImageData)
			{
				free(ImageData);
			}
		}
	public:
		void* GetImageData() { return ImageData; }
		int GetSizeX() { return Width; }
		int GetSizeY() { return Height; }
		int GetChannelNum() { return ChannelNum; }
		EFormat GetFormat() { return Format; }
		EDataType GetDataType() { return DataType; }
	protected:
		void* ImageData = nullptr;
		int Width = 0;
		int Height = 0;
		int Depth = 0;
		int ChannelNum = 0;

		EFormat Format = EFormat::F_RGB;
		EDataType DataType = EDataType::DT_UNSIGNED_BYTE;
	};

	struct TextureCubeIOFlags
	{

	};

	//IO Settings
	using TextureCubeImportSettings = XImportSetting<TextureCubeIOFlags, EFileType::FT_TEXTURE_CUBE>;
	using TextureCubeExportSettings = XExportSetting<TextureCubeIOFlags, EFileType::FT_TEXTURE_CUBE>;

	class XTextureCube :public XTexture
	{
		IMPL_RESOURCE(TextureCube)
	public:
		friend class XImporter<TextureCubeImportSettings, XTextureCube::BaseType>;
		friend class XExporter<TextureCubeExportSettings, XTextureCube>;
	public:
		XTextureCube() = default;
		XTextureCube(const std::string& InFilePath) :XTexture(InFilePath) {  }
		virtual ~XTextureCube()
		{
			if (ImageData)
			{
				free(ImageData);
			}
		}
	public:
		void* GetImageData() { return ImageData; }
		int GetSizeX() { return Width; }
		int GetSizeY() { return Height; }
		int GetChannelNum() { return ChannelNum; }
		EFormat GetFormat() { return Format; }
		EDataType GetDataType() { return DataType; }
	protected:
		void* ImageData = nullptr;
		int Width = 0;
		int Height = 0;
		int ChannelNum = 0;

		EFormat Format = EFormat::F_RGB;
		EDataType DataType = EDataType::DT_UNSIGNED_BYTE;
	};


#pragma endregion

}