#pragma once
#include "Core.h"
#include "json.h"
#include <string>
using FJson = Json::Value;

class FLORA_API ISerialization
{
public:
	virtual bool Parse(IN FJson&) = 0;
	virtual bool Serialize(OUT FJson&) = 0;

	void SaveToFile(const std::string& filepath);
	void LoadFromFile(const std::string& filepath);
};