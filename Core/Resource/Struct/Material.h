#pragma once
#include <Common/Refl.hpp>
#include<Common/Serialize.h>
#include <Render/RHI/RHI.h>
#include <Common/GlobalType.h>
#include <Resource/ResourceManager.h>

#include <map>
#include <string>
namespace flora
{
	struct MaterialIFlags
	{
		MaterialIFlags(){}
		MaterialIFlags(const char* InName) :Name(InName) {};
		
		std::string Name;
		std::map<EShaderType, std::string> Files;
	};

	struct MaterialOFlags
	{

	};

	//IO Settings
	using MaterialImportSettings = XImportSetting<MaterialIFlags, EFileType::FT_MATERIAL>;
	using MaterialExportSettings = XExportSetting<MaterialOFlags, EFileType::FT_MATERIAL>;

	class XMaterial : public flora::IResource<>, ISerialization
	{
		IMPL_RESOURCE(Material)
	public:
		friend class XImporter<MaterialImportSettings, XMaterial::BaseType>;
		friend class XExporter<MaterialExportSettings, XMaterial>;

	public:
		XMaterial() = default;
		virtual ~XMaterial() = default;

		XMaterial(const std::string& InFilePath) :flora::IResource<>(InFilePath) {  }

		std::map<EShaderType, std::string>& GetShaderMap(){ return ShaderMap; };
		XMaterialState& GetMaterialState(){ return MaterialState; };
	protected:
		std::map<EShaderType, std::string> ShaderMap;
		XMaterialState MaterialState = XMaterialState::Opacity();


	public:
		static void ImportShaderHeaderFiles(const std::string& ShaderRootPath);
		static void ParseAndReplaceIncludes(const char* Sufix, std::string& Inout);
	protected:
		static inline std::unordered_map<std::string, std::string> GIncludeMap;
	};

}
