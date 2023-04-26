#pragma once
#include "Core.h"
#include <imgui.h>
#include <imgui_internal.h>
/*						x
(0,0)¡¤---------------¡ú
	 |
	 |
	 |		ImGui Coordinate System
	 |
	 ¡ý
	 y

	 y
	 ¡ü
	 |
	 |
	 |		Flora Coordinate System
	 |
	 |
(0,0)¡¤------------------¡úx
*/

//contains a ImGuiWindow
class FLORA_API FViewport
{
public:
	FViewport(const ImGuiWindow*);
	virtual ~FViewport() = default;

	//query 
	bool   IsCursorInsideViewport()	const;
	ImVec2 GetCursorPos()			const;
	ImVec2 GetCursorDelta()			const;
	ImRect GetImGuiRect()			const;
	ImVec2 GetImGuiWindowSize()		const;

	void   ResetWindow(const ImGuiWindow*);

protected:
	const ImGuiWindow* NativeWindow = nullptr;
};

//display game scene
class FLORA_API FGameScene :public FViewport
{
public:
	FGameScene(ImGuiWindow*,uint32_t);
	virtual ~FGameScene() = default;

	inline uint32_t GetBackBuffer()const { return BackBuffer; };
private:
	//current frame color
	uint32_t BackBuffer = 0;
};
