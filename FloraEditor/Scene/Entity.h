#pragma once
#include <vector>
#include <map>
#include <string>

#include "Scene.h"

namespace flora
{
	class IEntity
	{
		friend class IScene;
	protected:
		IEntity() = default;
	public:
		virtual~IEntity() = default;

		virtual std::string GetType()const = 0;

	private:
		std::map<std::string, uint32_t>Comps;

		uint32_t Scene = -1;
	};
}