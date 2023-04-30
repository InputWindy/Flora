#pragma once
#include "Core.h"
#include "Texture.h"
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
	FViewport() = default;
	virtual ~FViewport() = default;

	//query 
	bool   IsCursorInsideViewport()	const;
	ImVec2 GetCursorPos()			const;
	ImVec2 GetCursorDelta()			const;
	ImRect GetImGuiRect()			const;
	ImVec2 GetImGuiWindowSize()		const;
	uint32_t GetW()const;
	uint32_t GetH()const;

	bool   ResetWindow(const ImGuiWindow*);

protected:
	const ImGuiWindow* NativeWindow = nullptr;
};

//display game scene
class FLORA_API FGameScene :public FViewport
{
public:
	FGameScene();
	virtual ~FGameScene() = default;

	void SetBackBuffer(Ref<FTexture>);
	void Display();
public:
	static FGameScene& GetMainScene();
private:
	Ref<FTexture> BackBuffer = nullptr;
};
