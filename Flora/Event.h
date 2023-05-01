#pragma once
#include "Type.h"
#include <functional>
enum
{
	ET_None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
	AppTick, AppUpdate, AppRender,
	KeyPressed, KeyReleased, KeyTyped,
	MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

struct FEvent
{
	bool bHandled = false;

	virtual uint32_t GetEventType() const = 0;
};

#pragma region KeyEvent
struct FKeyEvent :public FEvent
{
	uint32_t Key = 0;

	virtual uint32_t GetEventType() const = 0;
};

struct FKeyPressedEvent :public FKeyEvent
{
	FKeyPressedEvent() {};
	FKeyPressedEvent(uint32_t key, int cnt)
	{
		Key = key;
		RepeatCnt = cnt;
	}
	int RepeatCnt = 0;

	virtual uint32_t GetEventType() const
	{
		return KeyPressed;
	}
};

struct FKeyReleasedEvent :public FKeyEvent
{
	FKeyReleasedEvent() {};
	FKeyReleasedEvent(uint32_t key)
	{
		Key = key;
	};
	virtual uint32_t GetEventType() const
	{
		return KeyReleased;
	}
};

struct FKeyTypedEvent :public FKeyEvent
{
	FKeyTypedEvent() {};
	FKeyTypedEvent(uint32_t key)
	{
		Key = key;
	};
	virtual uint32_t GetEventType() const
	{
		return KeyTyped;
	}
};
#pragma endregion

#pragma region MouseEvent
struct FMouseMoveEvent :public FEvent
{
	FMouseMoveEvent() {};
	FMouseMoveEvent(float x, float y)
	{
		MouseX = x;
		MouseY = y;
	};

	float MouseX = 0.0f;
	float MouseY = 0.0f;

	virtual uint32_t GetEventType() const { return MouseMoved; };
};

struct FMouseScrolledEvent :public FEvent
{
	FMouseScrolledEvent() {};
	FMouseScrolledEvent(float x, float y)
	{
		MouseOffsetX = x;
		MouseOffsetY = y;
	};

	float MouseOffsetX = 0.0f;
	float MouseOffsetY = 0.0f;

	virtual uint32_t GetEventType() const { return MouseScrolled; };
};

struct FMouseButtonEvent :public FEvent
{
	FMouseButtonEvent() {};
	FMouseButtonEvent(int btn)
	{
		Button = btn;
	}
	virtual uint32_t GetEventType() const = 0;
	int Button = 0;
};

struct FMouseButtonPressedEvent :public FMouseButtonEvent
{
	FMouseButtonPressedEvent() {};
	FMouseButtonPressedEvent(int btn)
		:FMouseButtonEvent(btn) {};
	virtual uint32_t GetEventType() const
	{
		return MouseButtonPressed;
	};
};

struct FMouseButtonReleasedEvent :public FMouseButtonEvent
{
	FMouseButtonReleasedEvent() {};
	FMouseButtonReleasedEvent(int btn)
		:FMouseButtonEvent(btn) {};
	virtual uint32_t GetEventType() const
	{
		return MouseButtonReleased;
	};
};
#pragma endregion

class FEventDispatcher
{
	template<typename EventType>
	using EventHandler = std::function<bool(EventType&)>;
public:
	FEventDispatcher(FEvent& e) :Event(e) {};

	template<typename EventType>
	bool Dispatch(EventHandler<EventType> Handler);
private:
	FEvent& Event;
};

