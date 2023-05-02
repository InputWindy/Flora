#include "InputSystem.h"
#include <imgui.h>
#include <imgui_internal.h>
#include <glfw3.h>
#include "Application.h"
extern FApplication* GEngine;
bool FInputSystem::IsKeyPressed(uint32_t KeyCode)
{
	return glfwGetKey((GLFWwindow*)GEngine->WindowHandle, KeyCode) == F_KEY_PRESS;
}
bool FInputSystem::IsKeyReleased(uint32_t KeyCode)
{
	return glfwGetKey((GLFWwindow*)GEngine->WindowHandle, KeyCode) == F_KEY_RELEASE;
}
bool FInputSystem::IsKeyTyped(uint32_t KeyCode)
{
	return glfwGetKey((GLFWwindow*)GEngine->WindowHandle, KeyCode) == F_KEY_REPEAT;
}
bool FInputSystem::IsMouseDown(uint32_t KeyCode)
{
	if (ImGui::IsMouseDown(KeyCode))return true;

	return false;
}
bool FInputSystem::IsMouseClicked(uint32_t KeyCode)
{
	if (ImGui::IsMouseClicked(KeyCode))return true;
	return false;
}
bool FInputSystem::IsMouseDoubleClicked(uint32_t KeyCode)
{
	if (ImGui::IsMouseDoubleClicked(KeyCode))return true;
	return false;
}
float FInputSystem::MouseDeltaX() const
{
	return ImGui::GetIO().MouseDelta.x;
}
float FInputSystem::MouseDeltaY() const
{
	return ImGui::GetIO().MouseDelta.y;
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
		for (uint32_t KeyCode = F_KEY_SPACE; KeyCode <= F_KEY_LAST; ++KeyCode)
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
