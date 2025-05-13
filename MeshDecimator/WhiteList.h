#pragma once 
#include <CoreMinimal.h>

// Filter out actors that do not require decimation by name.
struct XWhiteList:public flora::ISerialization
{
	virtual bool Parse(const json11::Json& In)
	{
		WhiteLists.clear();

		for (const auto& Name : In["WhiteLists"].array_items())
		{
			std::string WhiteListActorName = Name.string_value();

			if (std::find(WhiteLists.begin(), WhiteLists.end(), WhiteListActorName) == WhiteLists.end())
			{
				WhiteLists.emplace_back(WhiteListActorName);
			}
		}

		return true;
	}

	virtual bool Serialize(json11::Json& Out) 
	{
		WhiteLists = { "1", "2", "3", "4", "5", "6", "7", };

		auto array = json11::Json::array();
		for (const auto& WhiteList : WhiteLists)
		{
			array.push_back(WhiteList);
		}
		Out = json11::Json::object
		{
			{ "WhiteLists",array},
		};
		return true;
	}

	std::vector<std::string> WhiteLists;
};
