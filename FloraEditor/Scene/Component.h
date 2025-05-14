#pragma once
#include <string>

namespace flora
{
	class IComponent
	{
		friend class IScene;
	protected:
		IComponent() = default;
	public:
		virtual~IComponent() = default;

		virtual std::string GetType()const = 0;

	public:
		inline void Enable(bool NewEnable) { bEnable = NewEnable; }
		inline bool IsEnable()const { return bEnable ; }

	private:
		bool bEnable = true;

		uint32_t Entt = -1;
	};

}