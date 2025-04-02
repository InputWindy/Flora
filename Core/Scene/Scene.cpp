#include "Scene.h"

bool XVerse::XScene::Parse(const json11::Json& In)
{
	Name = In["Name"].string_value();
	for (const auto& Actor : In["Actors"].array_items())
	{
		std::string ActorType = Actor["ActorType"].string_value();
		if (ActorType == "CameraActor")
		{
		   	SpawnActor<XCameraActor>()->Parse(Actor);
		}
		else if (ActorType == "ModelActor")
		{
			SpawnActor<XModelActor>()->Parse(Actor);
		}
		else
		{
			assert(0);
		}
	}
	return true;
}

bool XVerse::XScene::Serialize(json11::Json& Out)
{
	auto array = json11::Json::array();
	for (int i = 1;i < Actors.size();++i)
	{
		json11::Json o;
		Actors[i]->Serialize(o);
		array.push_back(o);
	}
	Out = json11::Json::object
	{
		{ "Name",Name },
		{ "Actors",array }
	};
	return true;
}

XVerse::Guid XVerse::XScene::GetNextActorID()
{
	for (size_t i = 1; i < Actors.size(); i++)
		if (Actors[i] == nullptr)return i;

	Actors.push_back(nullptr);

	return Actors.size() - 1;
}
