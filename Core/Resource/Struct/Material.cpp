#include <Resource/Struct/Material.h>
#include <Common/GlobalType.h>
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
std::shared_ptr<XVerse::XMaterial::BaseType> XVerse::XImporter<XVerse::MaterialImportSettings, XVerse::XMaterial::BaseType>::Exec(const std::string& InFilepath, const XVerse::MaterialImportSettings& InSettings)
{
	std::shared_ptr<XVerse::XMaterial> Ret = std::make_shared<XMaterial>();

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


void XVerse::XMaterial::ImportShaderHeaderFiles(const std::string& ShaderRootPath)
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

void XVerse::XMaterial::ParseAndReplaceIncludes(const char* Sufix,std::string& Inout)
{
	std::string reg = "#include\\s+";
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
			XVERSE_CORE_WARN("Include file not found: {0}", includeFilename);
			break;
		}
	}

	Inout = Dst;
}


template<>
bool XVerse::XExporter<XVerse::MaterialImportSettings, XVerse::XMaterial>::Exec(XVerse::XMaterial& Material, const std::string& Filepath, const XVerse::MaterialImportSettings& Settings)
{
	bool Ret = false;

	return Ret;
}