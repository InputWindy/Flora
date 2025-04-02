#pragma once
#include <Scene/Struct/Camera.h>
#include <Scene/Struct/Transform.h>
#include <Resource/Struct/ModelMesh.h>
#include <Common/Common.h>
namespace flora
{
	enum class EComponentType
	{
		CT_Tag,
		CT_Camera,
		CT_Transform,
		CT_ModelMesh,
		
		CT_MAX_COUNT
	};

	class XActor;

	class XComponent :public ISharable,public ISerialization
	{
	protected:
		XComponent() = default;
		XComponent(XActor* InOwner) :Owner(InOwner) {}
	public:
		virtual ~XComponent() = default;
		virtual EComponentType GetType()const = 0;
	public:

	protected:
		const XActor* Owner = nullptr;
	};

	class XTagComponent :public XComponent
	{
	protected:
		XTagComponent() = default;
		XTagComponent(XActor* InOwner) :XComponent(InOwner) {}
	public:
		virtual ~XTagComponent() = default;
		virtual EComponentType GetType()const { return EComponentType::CT_Tag; }

		bool FindTag(const std::string& InTag)
		{
			for (uint32_t i = 0; i < Tags.size(); i++)
			{
				if (Tags[i] == InTag)
				{
					return true;
				}
			}
			return false;
		}
		void AddTag(const std::string& InTag)
		{
			if (FindTag(InTag) == false)
			{
				Tags.push_back(InTag);
			}
		}
		void RemoveTag(const std::string& InTag)
		{
			auto Iter = Tags.begin();
			while (Iter != Tags.end())
			{
				if ((*Iter) == InTag)
				{
					Tags.erase(Iter);
					return;
				}
				++Iter;
			}
		}
	public:
		virtual bool Parse(const json11::Json& In);
		virtual bool Serialize(json11::Json& Out);
	protected:
		std::vector<std::string> Tags;
	};

	class XTransformComponent :public XComponent
	{
	protected:
		XTransformComponent() = default;
		XTransformComponent(XActor* InOwner) :XComponent(InOwner) {}
	public:
		virtual ~XTransformComponent() = default;
		virtual EComponentType GetType()const { return EComponentType::CT_Transform; }

		inline XTransform& GetTransform() { return Transform; }
	public:
		virtual bool Parse(const json11::Json& Out);
		virtual bool Serialize(json11::Json& In);
	protected:
		XTransform Transform;
	};

	class XCameraComponent :public XComponent
	{
	protected:
		XCameraComponent()
		{
			struct MyStruct:public XPerspectiveCamera{};
			Camera = std::make_unique<MyStruct>();
			Camera->Update();
		}
		XCameraComponent(XActor* InOwner) :XComponent(InOwner) 
		{
			struct MyStruct :public XPerspectiveCamera {};
			Camera = std::make_unique<MyStruct>();
			Camera->Update();
		}
	public:
		virtual ~XCameraComponent() = default;
		virtual EComponentType GetType()const { return EComponentType::CT_Camera; }

		template<typename T>
		T* GetCamera() { return dynamic_cast<T*>(Camera.get()); }

		void SwitchCameraType(ECameraType InType) 
		{
			if (Camera->GetType() != InType)
			{
				if (InType == ECameraType::CT_PERSPECTIVE)
				{
					Camera.release();
					struct MyStruct :public XPerspectiveCamera {};
					Camera = std::make_unique<MyStruct>();
					Camera->Update();
				}
				else if (InType == ECameraType::CT_ORTHOGRAPHIC)
				{
					Camera.release();
					struct MyStruct :public XOrthographicCamera {};
					Camera = std::make_unique<MyStruct>();
					Camera->Update();
				}
				else
				{
					assert(0);
				}
			}
		}
	public:
		virtual bool Parse(const json11::Json& Out);
		virtual bool Serialize(json11::Json& In);
	protected:
		/// <summary>
		/// default camera type
		/// </summary>
		std::unique_ptr<XCamera> Camera;
	};

	class XModelMeshComponent :public XComponent
	{
	protected:
		XModelMeshComponent() = default;
		XModelMeshComponent(XActor* InOwner) :XComponent(InOwner) {}
	public:
		virtual ~XModelMeshComponent() = default;
		virtual EComponentType GetType()const { return EComponentType::CT_ModelMesh; }

		void SetModelMesh(Guid InID) { ModelMesh = InID; }
		void SetModelMesh(XModelMesh* InModelMesh) { if (InModelMesh) ModelMesh = InModelMesh->GetID(); }

		Guid GetModelMesh()const { return ModelMesh; }
	public:
		virtual bool Parse(const json11::Json& In);
		virtual bool Serialize(json11::Json& Out);
	protected:
		Guid ModelMesh = 0;
		std::vector<Guid> MaterialSlots;
	};
}