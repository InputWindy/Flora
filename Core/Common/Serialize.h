#pragma once 
#include <json11.hpp>
namespace flora
{
	class ISerialization
	{
	public:
		virtual bool Parse(const json11::Json&);
		virtual bool Serialize(json11::Json&);
	public:
		void SaveToFile(const std::string& filepath);
		void LoadFromFile(const std::string& filepath);
	private:
		json11::Json ReadJsonFile(const std::string& filepath);
		void WriteJsonFile(const std::string& filename, const json11::Json& root);
	};
}