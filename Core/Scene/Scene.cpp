#include "Scene.h"
#include "System.h"
#include "Component.h"
#include "Entity.h"

void flora::IScene::Update()
{
	for (auto& Sys : Syss)
		if (Sys.second)Sys.second->OnUpdate(this, DeltaTime);
}

void flora::IScene::Start()
{
	for (auto& Sys : Syss)
		if (Sys.second)Sys.second->OnStart(this);
}

void flora::IScene::End()
{
	for (auto& Sys : Syss)
		if (Sys.second)Sys.second->OnEnd(this);
}

void flora::IScene::Attach(IEntity* entt, IComponent* comp)
{
	if (!entt || !comp)return;
	entt->Comps[comp->GetType()] = comp->ID;
}

void flora::IScene::Detach(IEntity* entt, IComponent* comp)
{
	if (!entt || !comp)return;
	entt->Comps.erase(comp->GetType());
}
