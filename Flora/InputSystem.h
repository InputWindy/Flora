#pragma once
#include "Core.h"
#include "Event.h"
#include <unordered_map>
using std::unordered_map;
class FLORA_API FInputSystem
{
	friend class FSceneManager;
	friend class FScene;
public:
	FInputSystem() = default;
	~FInputSystem() = default;

	static inline FInputSystem& Get() { static FInputSystem msInput; return msInput; };
	bool IsKeyPressed(uint32_t);
	bool IsKeyReleased(uint32_t);
	bool IsKeyTyped(uint32_t);
protected:
	unordered_map<uint32_t, FKeyPressedEvent>  KeyPressedEvents;
	unordered_map<uint32_t, FKeyReleasedEvent> KeyReleasedEvents;
	unordered_map<uint32_t, FKeyTypedEvent>    KeyTypedEvents;

	unordered_map<uint8_t, FMouseButtonPressedEvent>  MouseButtonPressedEvents;
	unordered_map<uint8_t, FMouseButtonReleasedEvent> MouseButtonReleasedEvents;

	FMouseMoveEvent     MouseMovedEvent;
	FMouseScrolledEvent MouseScrolledEvent;
private:
	void GatherInputEvent();
};

