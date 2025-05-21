#pragma once
#include <CoreMinimal.h>

namespace flora
{
	struct FMeshRendererComponent :public IComponent
	{
		DECLARE_TYPE(FMeshRendererComponent)

		virtual ~FMeshRendererComponent() {}

		Guid Model;
		Guid Material;
	};
}