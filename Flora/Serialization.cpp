#include "Serialization.h"
#include <iostream> 
#include <fstream> 
#include <string> 
#include <sstream>
using namespace std;
FJson ReadJsonFile(const std::string& filename)
{
	ifstream ifile;
	ifile.open(filename);

	Json::CharReaderBuilder ReaderBuilder;
	ReaderBuilder["emitUTF8"] = true;

	Json::Value root;
	string n = "-";
	FJson Json(n);
	root.toStyledString();

	string strerr;
	bool ok = Json::parseFromStream(ReaderBuilder, ifile, &root, &strerr);
	if (!ok) {
		cerr << "json parse failed";
	}
	return root;
}

void WriteJsonFile(const std::string& filename, const FJson& root)
{
	Json::StreamWriterBuilder writebuild;
	writebuild["emitUTF8"] = true;

	string document = Json::writeString(writebuild, root);

	ofstream ofile(filename);
	ofile << document;
	ofile.close();
}

void ISerialization::SaveToFile(const std::string& filepath)
{
	FJson in;
	Serialize(in);
	WriteJsonFile(filepath, in);
}

void ISerialization::LoadFromFile(const std::string& filepath)
{
	FJson out;
	out = ReadJsonFile(filepath);
	Parse(out);
}
