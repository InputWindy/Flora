#pragma once
#include <Scene/Actor.h>
#include <Common/Serialize.h>
namespace flora
{
	class XScene :public ISerialization
	{
	public:
		XScene()
		{
			struct MyStruct :public XCameraActor
			{
				MyStruct(const XScene* InScene, Guid InID) :XCameraActor(InScene, InID) {};
			};
			Actors.push_back(std::make_shared<MyStruct>(this,0));
		};
		XScene(const std::string& InName):Name(InName)
		{
			struct MyStruct :public XCameraActor
			{
				MyStruct(const XScene* InScene, Guid InID) :XCameraActor(InScene, InID) {};
			};
			Actors.push_back(std::make_shared<MyStruct>(this, 0));
		};
		~XScene() = default;
	public:
		template<typename T>
		T* SpawnActor()
		{
			struct MyStruct:public T
			{
				MyStruct(const XScene* InScene, Guid InID) :T(InScene, InID) {};
			};
			Guid NextID = GetNextActorID();
			Actors[NextID] = std::make_shared<MyStruct>(this, NextID);
			return Actors[NextID]->DynamicCast<T>();
		};

		void RemoveActor(Guid InID)
		{
			if (InID == 0 || InID >= Actors.size())return;

			Actors[InID] = nullptr;
		}

		void RemoveActor(XActor* InActor)
		{
			if (!InActor)return;

			RemoveActor(InActor->GetID());
		}

		XActor* FindActor(Guid InID)
		{
			if (InID == 0 || InID >= Actors.size())return nullptr;

			return Actors[InID].get();
		}

		XActor* FindActor(XActor* InActor)
		{
			if (!InActor)return nullptr;

			return FindActor(InActor->GetID());
		}

		XActor* FindActorByName(const std::string& InName)
		{
			for (size_t i = 1; i < Actors.size(); i++)
			{
				if (Actors[i]->GetName() == InName)
				{
					return Actors[i].get();
				}
			}
		}

		template<typename T>
		std::vector<T*> FindActors()
		{
			std::vector<T*> Ret;
			
			for (size_t i = 1; i < Actors.size(); i++)
			{
				if (T* Actor = Actors[i]->DynamicCast<T>())
				{
					Ret.push_back(Actor);
				}
			}
			return Ret;
		}

		std::vector<XActor*> FindActorsByTag(const std::string& InTag)
		{
			std::vector<XActor*> Ret;

			for (size_t i = 1; i < Actors.size(); i++)
			{
				if (Actors[i]->GetTagComponent()->FindTag(InTag))
				{
					Ret.push_back(Actors[i].get());
				}
			}
			return Ret;
		}

	public:
		virtual bool Parse(const json11::Json& In);
		virtual bool Serialize(json11::Json& Out);
	public:
		XCameraActor* GetDefaultCameraActor() { return Actors[0]->DynamicCast<XCameraActor>(); };
		const std::vector<std::shared_ptr<XActor>>& GetActors()const { return Actors; };
	private:
		Guid GetNextActorID();
	protected:
		std::string Name;
		//Actor[0] is default camera
		std::vector<std::shared_ptr<XActor>> Actors;
	};
}