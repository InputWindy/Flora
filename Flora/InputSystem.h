#pragma once
#include "Core.h"
#include "Event.h"
#include <unordered_map>
using std::unordered_map;

enum
{
	F_KEY_RELEASE = 0,
	F_KEY_PRESS = 1,
	F_KEY_REPEAT = 2,
};

enum
{
	/* Printable keys */
	F_KEY_SPACE = 32,
	F_KEY_APOSTROPHE = 39, /* ' */
	F_KEY_COMMA = 44, /* , */
	F_KEY_MINUS = 45, /* - */
	F_KEY_PERIOD = 46, /* . */
	F_KEY_SLASH = 47, /* / */
	F_KEY_0 = 48,
	F_KEY_1 = 49,
	F_KEY_2 = 50,
	F_KEY_3 = 51,
	F_KEY_4 = 52,
	F_KEY_5 = 53,
	F_KEY_6 = 54,
	F_KEY_7 = 55,
	F_KEY_8 = 56,
	F_KEY_9 = 57,
	F_KEY_SEMICOLON = 59, /* ; */
	F_KEY_EQUAL = 61, /* = */
	F_KEY_A = 65,
	F_KEY_B = 66,
	F_KEY_C = 67,
	F_KEY_D = 68,
	F_KEY_E = 69,
	F_KEY_F = 70,
	F_KEY_G = 71,
	F_KEY_H = 72,
	F_KEY_I = 73,
	F_KEY_J = 74,
	F_KEY_K = 75,
	F_KEY_L = 76,
	F_KEY_M = 77,
	F_KEY_N = 78,
	F_KEY_O = 79,
	F_KEY_P = 80,
	F_KEY_Q = 81,
	F_KEY_R = 82,
	F_KEY_S = 83,
	F_KEY_T = 84,
	F_KEY_U = 85,
	F_KEY_V = 86,
	F_KEY_W = 87,
	F_KEY_X = 88,
	F_KEY_Y = 89,
	F_KEY_Z = 90,
	F_KEY_LEFT_BRACKET = 91, /* [ */
	F_KEY_BACKSLASH = 92, /* \ */
	F_KEY_RIGHT_BRACKET = 93, /* ] */
	F_KEY_GRAVE_ACCENT = 96, /* ` */
	F_KEY_WORLD_1 = 161, /* non-US #1 */
	F_KEY_WORLD_2 = 162, /* non-US #2 */

	/* Function keys */
	F_KEY_ESCAPE = 256,
	F_KEY_ENTER = 257,
	F_KEY_TAB = 258,
	F_KEY_BACKSPACE = 259,
	F_KEY_INSERT = 260,
	F_KEY_DELETE = 261,
	F_KEY_RIGHT = 262,
	F_KEY_LEFT = 263,
	F_KEY_DOWN = 264,
	F_KEY_UP = 265,
	F_KEY_PAGE_UP = 266,
	F_KEY_PAGE_DOWN = 267,
	F_KEY_HOME = 268,
	F_KEY_END = 269,
	F_KEY_CAPS_LOCK = 280,
	F_KEY_SCROLL_LOCK = 281,
	F_KEY_NUM_LOCK = 282,
	F_KEY_PRINT_SCREEN = 283,
	F_KEY_PAUSE = 284,
	F_KEY_F1 = 290,
	F_KEY_F2 = 291,
	F_KEY_F3 = 292,
	F_KEY_F4 = 293,
	F_KEY_F5 = 294,
	F_KEY_F6 = 295,
	F_KEY_F7 = 296,
	F_KEY_F8 = 297,
	F_KEY_F9 = 298,
	F_KEY_F10 = 299,
	F_KEY_F11 = 300,
	F_KEY_F12 = 301,
	F_KEY_F13 = 302,
	F_KEY_F14 = 303,
	F_KEY_F15 = 304,
	F_KEY_F16 = 305,
	F_KEY_F17 = 306,
	F_KEY_F18 = 307,
	F_KEY_F19 = 308,
	F_KEY_F20 = 309,
	F_KEY_F21 = 310,
	F_KEY_F22 = 311,
	F_KEY_F23 = 312,
	F_KEY_F24 = 313,
	F_KEY_F25 = 314,
	F_KEY_KP_0 = 320,
	F_KEY_KP_1 = 321,
	F_KEY_KP_2 = 322,
	F_KEY_KP_3 = 323,
	F_KEY_KP_4 = 324,
	F_KEY_KP_5 = 325,
	F_KEY_KP_6 = 326,
	F_KEY_KP_7 = 327,
	F_KEY_KP_8 = 328,
	F_KEY_KP_9 = 329,
	F_KEY_KP_DECIMAL = 330,
	F_KEY_KP_DIVIDE = 331,
	F_KEY_KP_MULTIPLY = 332,
	F_KEY_KP_SUBTRACT = 333,
	F_KEY_KP_ADD = 334,
	F_KEY_KP_ENTER = 335,
	F_KEY_KP_EQUAL = 336,
	F_KEY_LEFT_SHIFT = 340,
	F_KEY_LEFT_CONTROL = 341,
	F_KEY_LEFT_ALT = 342,
	F_KEY_LEFT_SUPER = 343,
	F_KEY_RIGHT_SHIFT = 344,
	F_KEY_RIGHT_CONTROL = 345,
	F_KEY_RIGHT_ALT = 346,
	F_KEY_RIGHT_SUPER = 347,
	F_KEY_MENU = 348,

	F_KEY_LAST = F_KEY_MENU,
	F_MOD_SHIFT = 0x0001,
	F_MOD_CONTROL = 0x0002,
	F_MOD_ALT = 0x0004,
	F_MOD_SUPER = 0x0008,
	F_MOD_CAPS_LOCK = 0x0010,
	F_MOD_NUM_LOCK = 0x0020,
	F_MOUSE_BUTTON_1 = 0,
	F_MOUSE_BUTTON_2 = 1,
	F_MOUSE_BUTTON_3 = 2,
	F_MOUSE_BUTTON_4 = 3,
	F_MOUSE_BUTTON_5 = 4,
	F_MOUSE_BUTTON_6 = 5,
	F_MOUSE_BUTTON_7 = 6,
	F_MOUSE_BUTTON_8 = 7,
	F_MOUSE_BUTTON_LAST = F_MOUSE_BUTTON_8,
	F_MOUSE_BUTTON_LEFT = F_MOUSE_BUTTON_1,
	F_MOUSE_BUTTON_RIGHT = F_MOUSE_BUTTON_2,
	F_MOUSE_BUTTON_MIDDLE = F_MOUSE_BUTTON_3
};

class FLORA_API FInputSystem
{
	friend class FSceneManager;
	friend class FScene;
public:
	FInputSystem() = default;
	~FInputSystem() = default;

	static inline FInputSystem& Get() { static FInputSystem msInput; return msInput; };
public:
	//glfw
	bool IsKeyPressed(uint32_t);
	bool IsKeyReleased(uint32_t);
	bool IsKeyTyped(uint32_t);

	float MouseDeltaX()const;
	float MouseDeltaY()const;
public:
	//imgui
	bool IsMouseDown(uint32_t);
	bool IsMouseClicked(uint32_t);
	bool IsMouseDoubleClicked(uint32_t);
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

