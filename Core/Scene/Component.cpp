#include "Component.h"

bool XVerse::XTagComponent::Parse(const json11::Json& In)
{
    for (const auto& Tag : In.array_items())
    {
        AddTag(Tag.string_value());
    }
    return true;
}

bool XVerse::XTagComponent::Serialize(json11::Json& Out)
{
    auto array = json11::Json::array();
    for (const auto& Tag : Tags)
	{
		array.emplace_back(Tag);
    }
    Out = array;
    return true;
}

bool XVerse::XTransformComponent::Parse(const json11::Json& In)
{
    Transform.Parse(In);
    return true;
}

bool XVerse::XTransformComponent::Serialize(json11::Json& Out)
{
	Transform.Serialize(Out);
    return true;
}

bool XVerse::XCameraComponent::Parse(const json11::Json& In)
{
    ECameraType CameraType = In["Type"].string_value() == "Perspective" ? ECameraType::CT_PERSPECTIVE : ECameraType::CT_ORTHOGRAPHIC;
    SwitchCameraType(CameraType);
    Camera->Parse(In["Data"]);
    return true;
}

bool XVerse::XCameraComponent::Serialize(json11::Json& Out)
{
    std::string CameraType = Camera->GetType() == ECameraType::CT_PERSPECTIVE ? "Perspective" : "Orthographic";

    json11::Json InSerializeCamera;
    Camera->Serialize(InSerializeCamera);

    Out = json11::Json::object
    {
        { "Type", CameraType.c_str()},
        { "Data",InSerializeCamera.object_items()}
    };
    return true;
}

bool XVerse::XModelMeshComponent::Parse(const json11::Json& In)
{
    ModelMesh = In["ModelMesh"].int_value();
    for (const auto& Material : In["ModelMesh"].array_items())
    {
        MaterialSlots.push_back(Material.int_value());
    }
    return true;
}

bool XVerse::XModelMeshComponent::Serialize(json11::Json& Out)
{
    auto data = json11::Json::array();
    for (const auto& Material : MaterialSlots)
    {
        data.push_back(int(Material));
    }

    Out = json11::Json::object
    {
        { "ModelMesh", int(ModelMesh)},
        { "MaterialSlots", data }
    };
    return true;
}
