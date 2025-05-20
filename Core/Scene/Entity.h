#pragma once
#include <vector>
#include <map>
#include <string>
#include "Allocator.h"

namespace flora
{
	class ISystem;

	class IEntity:public IAllocable
	{
		friend class IScene;
	protected:
		IEntity() = default;
	public:
		virtual~IEntity() = default;

	private:
		friend class ISystem;

		inline bool IsEnable()const { return bEnable; }
		inline void Enable(bool e) { bEnable = e; }

		inline void Mark(uint32_t M) { Flags |= M; }
		inline void UnMark(uint32_t M) { Flags &= ~M; }
		inline bool HasMark(uint32_t M) { return Flags & M; }

	private:
		std::map<std::string, uint32_t>Comps;

	private:
		bool bEnable = true;

		uint32_t ID = -1;
		uint32_t Flags = 0;

	protected:
		IScene* Scene = nullptr;

	};
}