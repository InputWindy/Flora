#pragma once
#include <Common/Common.h>
#include <Scene/Component.h>
namespace XVerse
{
	enum class EActorType
	{
		AT_CameraActor,
		AT_ModelActor,

		AT_MAX_COUNT
	};

	class XScene;

	class XActor :public ISharable,public ISerialization
	{
	protected:
		XActor() = default;
		XActor(const XScene* InScene, Guid InID) :ID(InID),Scene(InScene) 
		{
			Name += '_';
			Name += std::to_string(ID);

			struct MyStruct1 :public XTagComponent
			{
				MyStruct1() {}
				MyStruct1(XActor* InOwner) :XTagComponent(InOwner) {}
			};
			TagComponent.reset(new MyStruct1(this));
		}
	public:
		virtual ~XActor() = default;
		virtual EActorType GetType()const = 0;
	protected:
		std::string Name = "Actor";

		Guid ID = 0;
		const XScene* Scene = nullptr;
	public:
		inline void SetName(const std::string& InName) { Name = InName; }
		inline const std::string& GetName()const { return Name; }
		inline Guid GetID()const { return ID; }
		inline const XScene* GetScene()const { return Scene; }
		 
		inline void SetParent(Guid InParent) { Parent = InParent; }
		inline void SetParent(XActor* InParent) { if (InParent) Parent = InParent->ID; }
		 
		inline void AddChild(Guid InParent) { Children.push_back(InParent); }
		inline void AddChild(XActor* InParent) { if (InParent) Children.push_back(InParent->ID); }

		XTagComponent* GetTagComponent() { return TagComponent.get(); }
	protected:
		Guid Parent = 0;
		std::vector<Guid> Children;

		std::unique_ptr<XTagComponent> TagComponent;
	};

	class XCameraActor :public XActor
	{
	protected:
		XCameraActor() = default;
		XCameraActor(const XScene* InScene, Guid InID) :XActor(InScene,InID)
		{
			struct MyStruct1 :public XCameraComponent 
			{
				MyStruct1() {}
				MyStruct1(XActor* InOwner) :XCameraComponent(InOwner) {}
			};
			struct MyStruct2 :public XTransformComponent
			{
				MyStruct2() {}
				MyStruct2(XActor* InOwner) :XTransformComponent(InOwner) {}
			};
			CameraComponent.reset(new MyStruct1(this));
			TransformComponent.reset(new MyStruct2(this));
		};
	public:
		virtual ~XCameraActor() = default;
		virtual EActorType GetType()const { return EActorType::AT_CameraActor; }

		inline XCameraComponent* GetCameraComponent() { return CameraComponent.get(); }
		inline XTransformComponent* GetTransformComponent() { return TransformComponent.get(); }
	public:
		virtual bool Parse(const json11::Json&);
		virtual bool Serialize(json11::Json&);
	protected:
		std::unique_ptr<XCameraComponent> CameraComponent;
		std::unique_ptr<XTransformComponent> TransformComponent;
	};

	class XModelActor :public XActor
	{
	protected:
		XModelActor() = default;
		XModelActor(const XScene* InScene, Guid InID) :XActor(InScene, InID)
		{
			struct MyStruct1 :public XModelMeshComponent
			{
				MyStruct1() {}
				MyStruct1(XActor* InOwner) :XModelMeshComponent(InOwner) {}
			};
			struct MyStruct2 :public XTransformComponent
			{
				MyStruct2() {}
				MyStruct2(XActor* InOwner) :XTransformComponent(InOwner) {}
			};
			ModelMeshComponent.reset(new MyStruct1(this));
			TransformComponent.reset(new MyStruct2(this));
		};
	public:
		virtual bool Parse(const json11::Json& In);
		virtual bool Serialize(json11::Json& Out);
	public:
		virtual ~XModelActor() = default;
		virtual EActorType GetType()const { return EActorType::AT_ModelActor; }

		inline XModelMeshComponent* GetModelMeshComponent() { return ModelMeshComponent.get(); }
		inline XTransformComponent* GetTransformComponent() { return TransformComponent.get(); }
	protected:
		std::unique_ptr<XModelMeshComponent> ModelMeshComponent;
		std::unique_ptr<XTransformComponent> TransformComponent;
	};
}