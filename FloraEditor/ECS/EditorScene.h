#pragma once
#include <CoreMinimal.h>

namespace flora
{
	struct FEditorScene :public IScene
	{
		DECLARE_TYPE(FEditorScene)

		FEditorScene()
		{
			
		}

		virtual ~FEditorScene() {}
	};
}
