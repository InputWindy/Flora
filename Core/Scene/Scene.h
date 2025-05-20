#pragma once
#include <vector>
#include <map>
#include <string>
#include <list>

#include "Allocator.h"
#include "Component.h"
#include "Entity.h"

namespace flora
{
	class IComponent;
	class IEntity;
	class ISystem;

	class IScene:public IAllocable
	{
	protected:
		IScene() = default;
	public:
		virtual~IScene() = default;

		void Update();
		void Start();
		void End();

	public:
		template<typename TComp,typename ...TParams>
		TComp* AllocC(TParams&& ... Param)
		{
			uint32_t Idx = 0;

			for (IComponent*& C : Comps[TComp::GetStaticType()])
			{
				if (!C)
				{
					C = reinterpret_cast<IComponent*>(TAllocator<TComp>::Alloc(std::forward<TParams&&>(Param)...));
					C->ID = Idx;

					return reinterpret_cast<TComp*>(C);
				}

				++Idx;
			}

			TComp* NewC = TAllocator<TComp>::Alloc(std::forward<TParams&&>(Param)...);

			if (NewC)
			{
				IComponent* NC = reinterpret_cast<IComponent*>(NewC);
				Comps[TComp::GetStaticType()].push_back(NC);
				NC->ID = Comps[TComp::GetStaticType()].size() - 1;
			}

			return NewC;
		}

		template<typename TComp>
		void DeallocC(uint32_t ID)
		{
			if (ID > -1 && ID < Comps[TComp::GetStaticType()].size())
			{
				Comps[TComp::GetStaticType()][ID] = nullptr;
			}
		}

		template<typename TComp>
		TComp* GetC(uint32_t ID)
		{
			TComp* C = nullptr;

			if (ID > -1 && ID < Comps[TComp::GetStaticType()].size())
				C = Comps[TComp::GetStaticType()][ID];

			return C;
		}

		template<typename TComp,uint32_t Mask = 0>
		std::vector<TComp*> GetComps()
		{
			std::vector<TComp*> Ret;

			for (IComponent* C : Comps[TComp::GetStaticType()])
				if (C && C->IsEnable() && C->HasMark(Mask))Ret.push_back(reinterpret_cast<TComp*>(C));
			
			return std::move(Ret);
		}

		std::map<std::string, std::vector<IComponent*>> Comps;

	public:
		template<typename TEntity, typename ...TParams>
		TEntity* AllocE(TParams&& ... Param)
		{
			uint32_t Idx = 0;

			for (IEntity*& C : Entts[TEntity::GetStaticType()])
			{
				if (!C)
				{
					C = reinterpret_cast<IEntity*>(TAllocator<TEntity>::Alloc(std::forward<TParams&&>(Param)...));
					C->ID = Idx;
					C->Scene = this;

					return reinterpret_cast<TEntity*>(C);
				}

				++Idx;
			}

			TEntity* NewC = TAllocator<TEntity>::Alloc(std::forward<TParams&&>(Param)...);

			if (NewC)
			{
				IEntity* NC = reinterpret_cast<IEntity*>(NewC);
				Entts[TEntity::GetStaticType()].push_back(NC);
				NC->ID = Entts[TEntity::GetStaticType()].size() - 1;
				NC->Scene = this;
			}

			return NewC;
		}

		template<typename TEntity>
		void DeallocE(uint32_t ID)
		{
			if (ID > -1 && ID < Entts[TEntity::GetStaticType()].size())
			{
				Entts[TEntity::GetStaticType()][ID] = nullptr;
			}
		}

		template<typename TEntity>
		TEntity* GetE(uint32_t ID)
		{
			TEntity* C = nullptr;

			if (ID > -1 && ID < Entts[TEntity::GetStaticType()].size())
				C = Entts[TEntity::GetStaticType()][ID];

			return C;
		}

		template<typename TEntity, uint32_t Mask = 0>
		std::vector<TEntity*> GetEntts()
		{
			std::vector<TEntity*> Ret;

			for (IEntity* C : Comps[TEntity::GetStaticType()])
				if (C && C->IsEnable() && C->HasMark(Mask))Ret.push_back(reinterpret_cast<TEntity*>(C));

			return std::move(Ret);
		}

		void Attach(IEntity*, IComponent*);
		void Detach(IEntity*, IComponent*);

		template<typename TComp>
		void Attach(IEntity* entt, TComp* comp)
		{
			Attach(entt, reinterpret_cast<IComponent*>(comp));
		};

		template<typename TComp>
		void Detach(IEntity* entt, TComp* comp)
		{
			Detach(entt, reinterpret_cast<IComponent*>(comp));
		};

		std::map<std::string, std::vector<IEntity*>> Entts;


	protected:
		template<typename TSys>
		void AddSystem()
		{
			Syss[TSys::GetStaticType()] = TAllocator<TSys>::Alloc();
		}

		std::map<std::string, ISystem*> Syss;
	private:
		float DeltaTime = 0.0;
	};
}