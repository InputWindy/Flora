#include "Actor.h"

bool flora::XCameraActor::Parse(const json11::Json& In)
{
	Name = In["Name"].string_value();
	ID = In["ID"].int_value();
	Parent = In["Parent"].int_value();
	for (const auto& Child : In["Children"].array_items())
	{
		Children.push_back(Child.int_value());
	}

	TagComponent->Parse(In["TagComponent"]);
	CameraComponent->Parse(In["CameraComponent"]);
	TransformComponent->Parse(In["TransformComponent"]);

    return true;
}

bool flora::XCameraActor::Serialize(json11::Json& Out)
{
	auto array = json11::Json::array();
	for (const auto& Child : Children)
	{
		array.push_back(int(Child));
	}

	json11::Json InTagComponent;
	TagComponent->Serialize(InTagComponent);

	json11::Json InCameraComponent;
	CameraComponent->Serialize(InCameraComponent);

	json11::Json InTransformComponent;
	TransformComponent->Serialize(InTransformComponent);

	Out = json11::Json::object
	{
		{ "ActorType","CameraActor"},
		{ "Name", Name},
		{ "ID", int(ID)},
		{ "Parent", int(Parent)},
		{ "Children", array},
		{ "TagComponent",InTagComponent.object_items()},
		{ "CameraComponent",InCameraComponent.object_items()},
		{ "TransformComponent",InTransformComponent.object_items()}
	};

    return true;
}

bool flora::XModelActor::Parse(const json11::Json& In)
{
	Name = In["Name"].string_value();
	ID = In["ID"].int_value();
	Parent = In["Parent"].int_value();
	for (const auto& Child : In["Children"].array_items())
	{
		Children.push_back(Child.int_value());
	}

	TagComponent->Parse(In["TagComponent"]);
	ModelMeshComponent->Parse(In["ModelMeshComponent"]);
	TransformComponent->Parse(In["TransformComponent"]);

	return true;
}

bool flora::XModelActor::Serialize(json11::Json& Out)
{
	auto array = json11::Json::array();
	for (const auto& Child : Children)
	{
		array.push_back(int(Child));
	}

	json11::Json InTagComponent;
	TagComponent->Serialize(InTagComponent);

	json11::Json InModelMeshComponent;
	ModelMeshComponent->Serialize(InModelMeshComponent);

	json11::Json InTransformComponent;
	TransformComponent->Serialize(InTransformComponent);

	Out = json11::Json::object
	{
		{ "ActorType","ModelActor"},
		{ "Name", Name},
		{ "ID", int(ID)},
		{ "Parent", int(Parent)},
		{ "Children", array},
		{ "TagComponent",InTagComponent.object_items()},
		{ "ModelMeshComponent",InModelMeshComponent.object_items()},
		{ "TransformComponent",InTransformComponent.object_items()}
	};

	return true;
}
