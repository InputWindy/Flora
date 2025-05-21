#pragma once
#include <string>
#include "Allocator.h"

namespace flora
{
	class IComponent;
	class IEntity;
	class IScene;

	class ISystem : public IAllocable, public ISerialization
	{
		friend class IScene;
	protected:
		ISystem() = default;
	public:
		virtual~ISystem() = default;
	protected:
		virtual void OnUpdate(IScene*, float /*delta time*/) = 0;
		virtual void OnStart(IScene*) = 0;
		virtual void OnEnd(IScene*) = 0;
	};
}