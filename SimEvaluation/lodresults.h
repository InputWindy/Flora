#pragma once 
#include <CoreMinimal.h>

struct XLodResults:public flora::ISerialization
{
	XLodResults()
	{}

	struct Result 
	{
		int CurLod = 0;
		bool Parse(const json11::Json& In)
		{
			CurLod = In["CurLod"].int_value();
			return true;
		}

		bool Serialize(json11::Json& Out)
		{
			Out = json11::Json::object
			{
				{ "CurLod", CurLod},
			};

			return true;
		}
	};

	virtual bool Parse(const json11::Json& In)
	{
		Results.clear();

		for (auto& Item : In.object_items())
		{
			Result R;
			R.Parse(Item.second);

			Results.insert({ Item.first, R });
		}

		return true;
	}

	virtual bool Serialize(json11::Json& Out) 
	{
		std::map<std::string, json11::Json> ActorList;

		for (auto& S : Results)
		{
			json11::Json Actor;
			S.second.Serialize(Actor);

			ActorList.insert({ S.first,Actor });
		}


		Out = json11::Json::object
		{
			ActorList
		};

		return true;
	}


	std::map<std::string, Result> Results;
};
