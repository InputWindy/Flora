#include <Resource/Struct/Material.h>
#include <Resource/Struct/Texture.h>
#include <Common/GlobalType.h>

#include <Common/ForwardDeclare.h>

#include <fstream>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>

std::string LoadFileToString(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return "";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

template<>
std::shared_ptr<flora::XMaterial::BaseType> flora::XImporter<flora::MaterialImportSettings, flora::XMaterial::BaseType>::Exec(const std::string& InFilepath, const flora::MaterialImportSettings& InSettings)
{
	std::shared_ptr<flora::XMaterial> Ret = std::make_shared<XMaterial>();

	bool bExist = true;

	for (auto File : InSettings.Files)
	{
		std::filesystem::path TmpPath(InFilepath + File.second);
		bExist = bExist && std::filesystem::exists(TmpPath);
	}

	if (!bExist)
	{
		return nullptr;
	}

	for (auto File : InSettings.Files)
	{
		std::string Tmp = LoadFileToString(InFilepath + File.second);

		XMaterial::ParseAndReplaceIncludes(".glsl", Tmp);
		XMaterial::ParseAndReplaceIncludes(".h", Tmp);
		Ret->GetShaderMap()[File.first] = Tmp;

		if (File.first == EShaderType::ST_COMPUTE_SHADER)
		{
			Ret->FilePath = InFilepath + File.second;
		}
		else if (File.first == EShaderType::ST_FRAGMENT_SHADER)
		{
			Ret->FilePath = InFilepath + File.second;
		}
	};
	return Ret;
}

namespace fs = std::filesystem;

bool FindGLSLFiles(const std::string& folderPath, std::vector<std::string>& Out)
{
	fs::path rootPath(folderPath);
	if (!fs::exists(rootPath))return false;

	for (const auto& entry : fs::recursive_directory_iterator(rootPath))
	{
		if (entry.is_regular_file() && entry.path().extension() == ".glsl" || entry.is_regular_file() && entry.path().extension() == ".h")
		{
			fs::path relativePath = fs::relative(entry.path(), rootPath);

			Out.push_back(relativePath.generic_string());
		}
	}

	return true;
}


void flora::XMaterial::ImportShaderHeaderFiles(const std::string& ShaderRootPath)
{
	std::vector<std::string> Filenames;
	if (!FindGLSLFiles(ShaderRootPath, Filenames))return;

	for (const std::string& Filename : Filenames)
	{
		std::string ShaderCode = LoadFileToString(ShaderRootPath + Filename);

		if (ShaderCode != "")
		{
			GIncludeMap.insert({ Filename, ShaderCode });
		};
	};
}


void ReplaceSubstring(std::string& originalString, const std::string& oldSubstring, const std::string& newSubstring)
{
	size_t pos = originalString.find(oldSubstring);
	if (pos != std::string::npos) 
	{
		originalString.replace(pos, oldSubstring.length(), newSubstring);
	}
}

void flora::XMaterial::ParseAndReplaceIncludes(const char* Sufix,std::string& Inout)
{
	std::string reg = "#include//s+";
	reg += '"';
	reg += "([^";
	reg += '"';
	reg += "]+)";
	reg += Sufix;
	reg += '"';

	std::regex includePattern(reg);
	std::smatch match;
	std::string Dst = Inout;

	while (std::regex_search(Dst, match, includePattern))
	{
		std::string includeDirective = match[0];
		std::string includeFilename = match[1];

		includeFilename += Sufix;

		auto it = GIncludeMap.find(includeFilename);
		if (it != GIncludeMap.end())
		{
			std::string includeContent = it->second;
			ReplaceSubstring(Dst, includeDirective, includeContent);
		}
		else
		{
			FLORA_CORE_WARN("Include file not found: {0}", includeFilename);
			break;
		}
	}

	Inout = Dst;
}

void flora::XMaterial::ImportEngineDefaultResource()
{
	std::vector<std::pair<std::shared_ptr<XRHITexture2D>, std::string>> TexturesMap = 
	{
		{White,ExcutePath + "/Content/Engine/Texture/Default/White.jpg"},
		{MR,ExcutePath + "/Content/Engine/Texture/Default/MR.jpg"},
		{Normal,ExcutePath + "/Content/Engine/Texture/Default/Normal.jpg"},
		{Grid,ExcutePath + "/Content/Engine/Texture/Default/Grid.tga"},
		{BRDF_Schilk,ExcutePath + "/Content/Engine/Texture/Global/brdf_schilk.hdr"},
		{ColorGradingLUT,ExcutePath + "/Content/Engine/Texture/LUT/color_grading_lut.jpg"},
		{ColorGradingLUT01,ExcutePath + "/Content/Engine/Texture/LUT/color_grading_lut_01.png"},
		{ColorGradingLUT02,ExcutePath + "/Content/Engine/Texture/LUT/color_grading_lut_02.png"},
		{ColorGradingLUT03,ExcutePath + "/Content/Engine/Texture/LUT/color_grading_lut_03.png"},
		{ColorGradingLUT04,ExcutePath + "/Content/Engine/Texture/LUT/color_grading_lut_04.png"},
		{ColorGradingLUT05,ExcutePath + "/Content/Engine/Texture/LUT/color_grading_lut_05.png"},
		{ColorGradingLUT06,ExcutePath + "/Content/Engine/Texture/LUT/color_grading_lut_06.png"},
	};


	std::shared_ptr<IResource> Object = nullptr;

	for(auto& Item : TexturesMap)
	{
		Object = IResource<>::Open<XImporter>(Item.second, Texture2DImportSettings());

		if (Object->GetTypeString() == "Texture2D")
		{
			XTexture2D* Texture2D = reinterpret_cast<XTexture2D*>(Object.get());

			XRHITextureCreateInfo Info;
			Info.SizeX = Texture2D->GetSizeX();
			Info.SizeY = Texture2D->GetSizeY();

			Info.InternalFormat = MatchFormat(Texture2D->GetFormat(), Texture2D->GetDataType());
			Info.MipLevels = 1;
			Info.TextureType = ETextureTarget::TT_TEXTURE_2D;
			Info.DefaultSamplerInfo = {};

			Item.first = GetRHI()->CreateTexture(Info)->DynamicPointerCast<XRHITexture2D>();
			Item.first->SetPixels(0, Texture2D->GetFormat(), Texture2D->GetDataType(), Texture2D->GetImageData());
		}
	}
}


template<>
bool flora::XExporter<flora::MaterialImportSettings, flora::XMaterial>::Exec(flora::XMaterial& Material, const std::string& Filepath, const flora::MaterialImportSettings& Settings)
{
	bool Ret = false;

	return Ret;
}