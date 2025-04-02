#include <Common/Serialize.h>
#include <Common/Common.h>
#include <filesystem>
#include <string>
#include <fstream>

using std::string;

bool flora::ISerialization::Parse(const json11::Json&)
{
	return true;
}

bool flora::ISerialization::Serialize(json11::Json&)
{
	return true;
}

void flora::ISerialization::SaveToFile(const std::string& filepath)
{
	json11::Json in;
	Serialize(in);

	std::filesystem::path Path(filepath);

	flora::GlobalUtils::CreateDirectoryRecursive(Path.parent_path().generic_string());

	WriteJsonFile(filepath, in);
}
void flora::ISerialization::LoadFromFile(const std::string& filepath)
{
	std::filesystem::path Path(filepath);
	if (std::filesystem::exists(Path) && std::filesystem::is_regular_file(Path))
	{
		json11::Json out;
		out = ReadJsonFile(filepath);
		Parse(out);
	}
}
json11::Json flora::ISerialization::ReadJsonFile(const std::string& filepath)
{
	std::ifstream File;
	std::stringstream Stream;

	File.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	File.open(filepath);

	Stream << File.rdbuf();

	string json_string = Stream.str();

	std::string err;
	json11::Json mJson = json11::Json::parse(json_string, err);

	File.close();

	return mJson;
}

void flora::ISerialization::WriteJsonFile(const std::string& filename, const json11::Json& root)
{
	string string_json = root.dump();
	std::ofstream ofile(filename);
	ofile << string_json;
	ofile.close();
}
