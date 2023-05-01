#include "Event.h"
#include "Event.h"
template<>
bool FEventDispatcher::Dispatch<FKeyPressedEvent>(EventHandler<FKeyPressedEvent> Handler)
{
	if (dynamic_cast<FKeyPressedEvent*>(&Event))
	{
		Event.bHandled = Handler(*(FKeyPressedEvent*)&Event);
		return true;
	}
	return false;
}

template<>
bool FEventDispatcher::Dispatch<FKeyReleasedEvent>(EventHandler<FKeyReleasedEvent> Handler)
{
	if (dynamic_cast<FKeyReleasedEvent*>(&Event))
	{
		Event.bHandled = Handler(*(FKeyReleasedEvent*)&Event);
		return true;
	}
	return false;
}

template<>
bool FEventDispatcher::Dispatch<FKeyTypedEvent>(EventHandler<FKeyTypedEvent> Handler)
{
	if (dynamic_cast<FKeyTypedEvent*>(&Event))
	{
		Event.bHandled = Handler(*(FKeyTypedEvent*)&Event);
		return true;
	}
	return false;
}

template<>
bool FEventDispatcher::Dispatch<FMouseMoveEvent>(EventHandler<FMouseMoveEvent> Handler)
{
	if (dynamic_cast<FMouseMoveEvent*>(&Event))
	{
		Event.bHandled = Handler(*(FMouseMoveEvent*)&Event);
		return true;
	}
	return false;
}

template<>
bool FEventDispatcher::Dispatch<FMouseScrolledEvent>(EventHandler<FMouseScrolledEvent> Handler)
{
	if (dynamic_cast<FMouseScrolledEvent*>(&Event))
	{
		Event.bHandled = Handler(*(FMouseScrolledEvent*)&Event);
		return true;
	}
	return false;
}

template<>
bool FEventDispatcher::Dispatch<FMouseButtonPressedEvent>(EventHandler<FMouseButtonPressedEvent> Handler)
{
	if (dynamic_cast<FMouseButtonPressedEvent*>(&Event))
	{
		Event.bHandled = Handler(*(FMouseButtonPressedEvent*)&Event);
		return true;
	}
	return false;
}

template<>
bool FEventDispatcher::Dispatch<FMouseButtonReleasedEvent>(EventHandler<FMouseButtonReleasedEvent> Handler)
{
	if (dynamic_cast<FMouseButtonReleasedEvent*>(&Event))
	{
		Event.bHandled = Handler(*(FMouseButtonReleasedEvent*)&Event);
		return true;
	}
	return false;
}

