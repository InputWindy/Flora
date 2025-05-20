#pragma once
#include <string>
#include "Allocator.h"

namespace flora
{
	class IScene;
	class ISystem;

	class IComponent:public IAllocable
	{
		friend class IScene;
	protected:
		IComponent() = default;
	public:
		virtual~IComponent() = default;

	private:
		friend class ISystem;

		inline bool IsEnable()const { return bEnable ; }
		inline void Enable(bool e) { bEnable = e; }

		inline void Mark(uint32_t M) { Flags |= M; }
		inline void UnMark(uint32_t M) { Flags &= ~M; }
		inline bool HasMark(uint32_t M) { return Flags & M; }

	private:
		bool bEnable = true;

		uint32_t Entt = -1;

		uint32_t ID = -1;
		uint32_t Flags = 0;
	};

}