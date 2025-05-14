#pragma once
#include <vector>
#include <map>
#include <string>

namespace flora
{
	class IComponent;
	class IEntity;
	class ISystem;

	class IScene
	{
	protected:
		IScene() = default;
	public:
		virtual~IScene() = default;

	private:
		/*
			数据存储并不是内存连续的，需要实现一个Allocator
		*/
		std::map<std::string, std::vector<IComponent*>> Comps;
		std::map<std::string, std::vector<IEntity*>> Entts;
		std::map<std::string, std::vector<ISystem*>> Syss;
	};
}