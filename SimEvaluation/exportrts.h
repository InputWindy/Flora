#pragma once 
#include <CoreMinimal.h>

struct XExportRTS:public flora::ISerialization
{
	XExportRTS()
	{}

	struct MeshInfo
	{
		struct ActorInfo
		{
			std::string ActorName;

			float X, Y, Z;
			float Pitch, Yaw, Roll;
			float ScaleX, ScaleY, ScaleZ;
		};

		int SelectedLOD = 0;
		int OriginTriNum = 0;
		int CurTriNum = 0;
		std::vector<float> Loss;
		bool bUpdate = true;
		std::vector<ActorInfo> ActorInfos;
		
		bool Parse(const json11::Json& In)
		{
			ActorInfos.clear();

			SelectedLOD = In["SelectedLOD"].int_value();
			bUpdate = In["bUpdate"].bool_value();
			OriginTriNum = In["OriginTriNum"].int_value();
			CurTriNum = In["CurTriNum"].int_value();

			for (auto& Item : In["ActorList"].array_items())
			{
				ActorInfo NewInfo;

				NewInfo.ActorName = Item["ActorName"].string_value();
				NewInfo.X = Item["LocationX"].number_value();
				NewInfo.Y = Item["LocationY"].number_value();
				NewInfo.Z = Item["LocationZ"].number_value();
				NewInfo.Pitch = Item["RotationPitch"].number_value();
				NewInfo.Yaw = Item["RotationYaw"].number_value();
				NewInfo.Roll = Item["RotationRoll"].number_value();
				NewInfo.ScaleX = Item["ScaleX"].number_value();
				NewInfo.ScaleY = Item["ScaleY"].number_value();
				NewInfo.ScaleZ = Item["ScaleZ"].number_value();

				ActorInfos.emplace_back(NewInfo);
			}

			Loss.clear();

			for (auto& Item : In["Loss"].array_items())
			{
				Loss.push_back(Item.number_value());
			}
			
			return true;
		}

		bool Serialize(json11::Json& Out)
		{
			json11::Json::array Arr;

			for (ActorInfo Info : ActorInfos)
			{
				json11::Json::object obj
				{
					{"ActorName",Info.ActorName},
					{"LocationX",Info.X},
					{"LocationY",Info.Y},
					{"LocationZ",Info.Z},
					{"RotationPitch",Info.Pitch},
					{"RotationYaw",Info.Yaw},
					{"RotationRoll",Info.Roll},
					{"ScaleX",Info.ScaleX},
					{"ScaleY",Info.ScaleY},
					{"ScaleZ",Info.ScaleZ},
				};
				Arr.push_back(obj);
			}

			json11::Json::array LossArr;
			for (float loss : Loss)
			{
				LossArr.push_back(loss);
			}


			Out = json11::Json::object
			{
				{"SelectedLOD",SelectedLOD},
				{"OriginTriNum",OriginTriNum},
				{"CurTriNum",CurTriNum},
				{"Loss",Loss},
				{"bUpdate",bUpdate},
				{"ActorList",Arr},
				{"Loss",LossArr},
			};

			return true;
		}
	};

	virtual bool Parse(const json11::Json& In)
	{
		StaticMeshInfos.clear();

		for (auto& Item : In.object_items())
		{
			MeshInfo NewInfo;
			NewInfo.Parse(Item.second);

			StaticMeshInfos.insert({ Item.first, NewInfo });
		}

		return true;
	}

	virtual bool Serialize(json11::Json& Out) 
	{
		std::map<std::string, json11::Json> StaticMeshList;

		for (auto Info : StaticMeshInfos)
		{
			json11::Json Obj;
			Info.second.Serialize(Obj);

			StaticMeshList.insert({ Info.first, Obj });
		}

		Out = json11::Json::object
		{
			StaticMeshList
		};

		return true;
	}


	std::map<std::string, MeshInfo> StaticMeshInfos;
};
