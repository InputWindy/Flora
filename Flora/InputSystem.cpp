#include "InputSystem.h"
#include <imgui.h>
#include <imgui_internal.h>
bool FInputSystem::IsKeyPressed(uint32_t KeyCode)
{
	if (ImGui::IsKeyPressed(KeyCode))return true;

	return false;
}
bool FInputSystem::IsKeyReleased(uint32_t KeyCode)
{
	if (ImGui::IsKeyReleased(KeyCode))return true;

	return false;
}
bool FInputSystem::IsKeyTyped(uint32_t KeyCode)
{
	if (ImGui::IsKeyDown(KeyCode))return true;

	return false;
}
void FInputSystem::GatherInputEvent()
{
	ImGuiIO& io = ImGui::GetIO();
	KeyPressedEvents.clear();
	KeyReleasedEvents.clear();
	KeyTypedEvents.clear();
	MouseButtonPressedEvents.clear();
	MouseButtonReleasedEvents.clear();

	MouseMovedEvent = { io.MouseDelta.x, io.MouseDelta.y };
	MouseScrolledEvent = { 0, io.MouseWheel };

	{
		for (uint32_t KeyCode = ImGuiKey_Tab; KeyCode <= ImGuiKey_ModSuper; ++KeyCode)
		{
			if (ImGui::IsKeyDown(KeyCode))
				KeyTypedEvents.insert({ KeyCode,{KeyCode} });
			if (ImGui::IsKeyPressed(KeyCode))
				KeyPressedEvents.insert({ KeyCode,{KeyCode,ImGui::GetKeyPressedAmount(KeyCode,0.01,1)} });
			if (ImGui::IsKeyReleased(KeyCode))
				KeyReleasedEvents.insert({ KeyCode,{KeyCode} });
		}

		for (uint8_t Btn = 0; Btn < 5; ++Btn)
		{
			if (io.MouseDown[Btn])MouseButtonPressedEvents.insert({ Btn ,{Btn} });
			else MouseButtonReleasedEvents.insert({ Btn,{Btn} });
		}
	}
}
