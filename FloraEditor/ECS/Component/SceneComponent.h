#pragma once
#include <CoreMinimal.h>

namespace flora
{
	struct FSceneComponent :public IComponent
	{
		DECLARE_TYPE(FSceneComponent)

		virtual ~FSceneComponent() {}

		XTransform Trans;
	};
}