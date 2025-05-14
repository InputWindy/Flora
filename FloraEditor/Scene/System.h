#pragma once
#include <string>

namespace flora
{
	class ISystem
	{
	protected:
		ISystem() = default;
	public:
		virtual~ISystem() = default;

		virtual std::string GetType()const = 0;
	};
}