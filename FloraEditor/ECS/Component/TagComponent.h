#pragma once
#include <CoreMinimal.h>

namespace flora
{
	struct FTagComponent :public IComponent
	{
		DECLARE_TYPE(FTagComponent)

		virtual ~FTagComponent() {}

		std::unordered_set<std::string> Tags;
	};
}
