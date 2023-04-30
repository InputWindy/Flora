#include "Viewport.h"
#include "UI.h"
#include <unordered_map>
using namespace std;

bool FViewport::IsCursorInsideViewport() const
{
	bool bCursorInsideRegion = false;
	if (NativeWindow)
	{
		ImRect ImGuiRegion = NativeWindow->Rect();
		ImVec2 ImGuiCursorScreenPos = ImGui::GetMousePos();
		if (ImGuiCursorScreenPos.x > ImGuiRegion.Min.x &&
			ImGuiCursorScreenPos.y > ImGuiRegion.Min.y &&
			ImGuiCursorScreenPos.x < ImGuiRegion.Max.x &&
			ImGuiCursorScreenPos.y < ImGuiRegion.Max.y)
			bCursorInsideRegion = true;
	}
	else
	{
		assert(0, "ImGuiWindow Is Missing For Flora Viewport!");
	}
	return bCursorInsideRegion;
}

ImVec2 FViewport::GetCursorPos() const
{
	if (NativeWindow)
	{
		ImRect ImGuiRegion = NativeWindow->Rect();
		ImVec2 ImGuiCursorScreenPos = ImGui::GetMousePos();
		ImVec2 ImGuiWindowSize = NativeWindow->Size;
		ImVec2 CursorPos = { ImGuiCursorScreenPos.x - ImGuiRegion.Min.x,ImGuiCursorScreenPos.y - ImGuiRegion.Min.y };
		CursorPos.y = ImGuiWindowSize.y - CursorPos.y;
		return CursorPos;
	}
	else
	{
		assert(0, "ImGuiWindow Is Missing For Flora Viewport!");
		return ImVec2();
	}
}

ImVec2 FViewport::GetCursorDelta() const
{
	ImVec2 CursorDelta = ImGui::GetIO().MouseDelta;
	CursorDelta.y *= -1;
	return CursorDelta;
}

ImRect FViewport::GetImGuiRect() const
{
	if (NativeWindow)return NativeWindow->Rect();
	else
	{
		assert(0, "ImGuiWindow Is Missing For Flora Viewport!");
		return ImRect();
	}
}

ImVec2 FViewport::GetImGuiWindowSize() const
{
	if (NativeWindow)return NativeWindow->Size;
	else
	{
		//assert(0, "ImGuiWindow Is Missing For Flora Viewport!");
		return ImVec2();
	}
}

uint32_t FViewport::GetW() const
{
	ImVec2 Size = GetImGuiWindowSize();
	return Size.x;
}

uint32_t FViewport::GetH() const
{
	ImVec2 Size = GetImGuiWindowSize();
	return Size.y;
}

bool FViewport::ResetWindow(const ImGuiWindow* Window)
{
	NativeWindow = Window;
	return true;
};

static FGameScene MainScene;

FGameScene::FGameScene()
{
}

void FGameScene::SetBackBuffer(Ref<FTexture> backbuffer)
{
	BackBuffer = backbuffer;
};

void FGameScene::Display()
{
	FUI::Image((ImTextureID)(BackBuffer ? BackBuffer->GetHandle() : 0), FUI::GetContentRegionAvail());
}

FGameScene& FGameScene::GetMainScene()
{
	return MainScene;
}
