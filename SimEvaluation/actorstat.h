#pragma once 
#include <CoreMinimal.h>

enum ENccUpdateStat
{
	WaitingForUpdate = 0,
	NccUpdateReady = 1
};

struct XStaticMeshResults:public flora::ISerialization
{
	XStaticMeshResults()
	{}

	struct Stat 
	{


		std::string ActorName;
		ENccUpdateStat NccUpdateStat;
		int LodNum = 1;
		int CurLod = 0;

		std::vector<float> Loss;

		bool Parse(const json11::Json& In)
		{
			ActorName = In["ActorName"].string_value();
			NccUpdateStat = ENccUpdateStat(In["NccUpdateStat"].int_value());
			LodNum = In["LodNum"].int_value();
			CurLod = In["CurLod"].int_value();

			for (auto& L : In["Loss"].array_items())
			{
				Loss.emplace_back(L.number_value());
			}
			return true;
		}

		bool Serialize(json11::Json& Out)
		{
			auto array = json11::Json::array();
			for (const auto& L : Loss)
			{
				array.push_back(L);
			}

			Out = json11::Json::object
			{
				{ "ActorName",ActorName},
				{ "NccUpdateStat", int(NccUpdateStat)},
				{ "CurLod", CurLod},
				{ "Loss",array},
			};

			return true;
		}
	};

	virtual bool Parse(const json11::Json& In)
	{
		ActorStats.clear();

		for (auto& Item : In["ActorStats"].object_items())
		{
			Stat S;
			S.Parse(Item.second);

			ActorStats.insert({ Item.first, S });
		}

		return true;
	}

	virtual bool Serialize(json11::Json& Out) 
	{
		std::map<std::string, json11::Json> ActorList;

		for (auto& S : ActorStats)
		{
			json11::Json Actor;
			S.second.Serialize(Actor);

			ActorList.insert({ S.first,Actor });
		}


		Out = json11::Json::object
		{
			{"ActorStats",ActorList},
			{"ActorNum",int(ActorList.size())}
		};

		return true;
	}


	std::map<std::string, Stat> ActorStats;
};
