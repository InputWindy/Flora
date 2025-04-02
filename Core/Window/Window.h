#pragma once

#include <string>
#include <vector>
#include <memory>

/*
	key codes (glfw key codes)
*/
namespace flora
{
#define X_TRUE                   1
	/*! @brief Zero.
	*
	*  This is only semantic sugar for the number 0.  You can instead use `0` or
	*  `false` or `_False` or `GL_FALSE` or `VK_FALSE` or anything else that is
	*  equal to zero.
	*
	*  @ingroup init
	*/
#define X_FALSE                  0

	/*! @name Key and button actions
	*  @{ */
	/*! @brief The key or mouse button was released.
	*
	*  The key or mouse button was released.
	*
	*  @ingroup input
	*/
#define X_RELEASE                0
	/*! @brief The key or mouse button was pressed.
	*
	*  The key or mouse button was pressed.
	*
	*  @ingroup input
	*/
#define X_PRESS                  1
	/*! @brief The key was held down until it repeated.
	*
	*  The key was held down until it repeated.
	*
	*  @ingroup input
	*/
#define X_REPEAT                 2
	/*! @} */

	/*! @defgroup hat_state Joystick hat states
	*  @brief Joystick hat states.
	*
	*  See [joystick hat input](@ref joystick_hat) for how these are used.
	*
	*  @ingroup input
	*  @{ */

	/* The unknown key */
#define X_KEY_UNKNOWN            -1

/* Printable keys */
#define X_KEY_SPACE              32
#define X_KEY_APOSTROPHE         39  /* ' */
#define X_KEY_COMMA              44  /* , */
#define X_KEY_MINUS              45  /* - */
#define X_KEY_PERIOD             46  /* . */
#define X_KEY_SLASH              47  /* / */
#define X_KEY_0                  48
#define X_KEY_1                  49
#define X_KEY_2                  50
#define X_KEY_3                  51
#define X_KEY_4                  52
#define X_KEY_5                  53
#define X_KEY_6                  54
#define X_KEY_7                  55
#define X_KEY_8                  56
#define X_KEY_9                  57
#define X_KEY_SEMICOLON          59  /* ; */
#define X_KEY_EQUAL              61  /* = */
#define X_KEY_A                  65
#define X_KEY_B                  66
#define X_KEY_C                  67
#define X_KEY_D                  68
#define X_KEY_E                  69
#define X_KEY_F                  70
#define X_KEY_G                  71
#define X_KEY_H                  72
#define X_KEY_I                  73
#define X_KEY_J                  74
#define X_KEY_K                  75
#define X_KEY_L                  76
#define X_KEY_M                  77
#define X_KEY_N                  78
#define X_KEY_O                  79
#define X_KEY_P                  80
#define X_KEY_Q                  81
#define X_KEY_R                  82
#define X_KEY_S                  83
#define X_KEY_T                  84
#define X_KEY_U                  85
#define X_KEY_V                  86
#define X_KEY_W                  87
#define X_KEY_X                  88
#define X_KEY_Y                  89
#define X_KEY_Z                  90
#define X_KEY_LEFT_BRACKET       91  /* [ */
#define X_KEY_BACKSLASH          92  /* \ */
#define X_KEY_RIGHT_BRACKET      93  /* ] */
#define X_KEY_GRAVE_ACCENT       96  /* ` */
#define X_KEY_WORLD_1            161 /* non-US #1 */
#define X_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define X_KEY_ESCAPE             256
#define X_KEY_ENTER              257
#define X_KEY_TAB                258
#define X_KEY_BACKSPACE          259
#define X_KEY_INSERT             260
#define X_KEY_DELETE             261
#define X_KEY_RIGHT              262
#define X_KEY_LEFT               263
#define X_KEY_DOWN               264
#define X_KEY_UP                 265
#define X_KEY_PAGE_UP            266
#define X_KEY_PAGE_DOWN          267
#define X_KEY_HOME               268
#define X_KEY_END                269
#define X_KEY_CAPS_LOCK          280
#define X_KEY_SCROLL_LOCK        281
#define X_KEY_NUM_LOCK           282
#define X_KEY_PRINT_SCREEN       283
#define X_KEY_PAUSE              284
#define X_KEY_F1                 290
#define X_KEY_F2                 291
#define X_KEY_F3                 292
#define X_KEY_F4                 293
#define X_KEY_F5                 294
#define X_KEY_F6                 295
#define X_KEY_F7                 296
#define X_KEY_F8                 297
#define X_KEY_F9                 298
#define X_KEY_F10                299
#define X_KEY_F11                300
#define X_KEY_F12                301
#define X_KEY_F13                302
#define X_KEY_F14                303
#define X_KEY_F15                304
#define X_KEY_F16                305
#define X_KEY_F17                306
#define X_KEY_F18                307
#define X_KEY_F19                308
#define X_KEY_F20                309
#define X_KEY_F21                310
#define X_KEY_F22                311
#define X_KEY_F23                312
#define X_KEY_F24                313
#define X_KEY_F25                314
#define X_KEY_KP_0               320
#define X_KEY_KP_1               321
#define X_KEY_KP_2               322
#define X_KEY_KP_3               323
#define X_KEY_KP_4               324
#define X_KEY_KP_5               325
#define X_KEY_KP_6               326
#define X_KEY_KP_7               327
#define X_KEY_KP_8               328
#define X_KEY_KP_9               329
#define X_KEY_KP_DECIMAL         330
#define X_KEY_KP_DIVIDE          331
#define X_KEY_KP_MULTIPLY        332
#define X_KEY_KP_SUBTRACT        333
#define X_KEY_KP_ADD             334
#define X_KEY_KP_ENTER           335
#define X_KEY_KP_EQUAL           336
#define X_KEY_LEFT_SHIFT         340
#define X_KEY_LEFT_CONTROL       341
#define X_KEY_LEFT_ALT           342
#define X_KEY_LEFT_SUPER         343
#define X_KEY_RIGHT_SHIFT        344
#define X_KEY_RIGHT_CONTROL      345
#define X_KEY_RIGHT_ALT          346
#define X_KEY_RIGHT_SUPER        347
#define X_KEY_MENU               348

#define X_KEY_LAST               X_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
*  @brief Modifier key flags.
*
*  See [key input](@ref input_key) for how these are used.
*
*  @ingroup input
*  @{ */

/*! @brief If this bit is set one or more Shift keys were held down.
*
*  If this bit is set one or more Shift keys were held down.
*/
#define X_MOD_SHIFT           0x0001
/*! @brief If this bit is set one or more Control keys were held down.
*
*  If this bit is set one or more Control keys were held down.
*/
#define X_MOD_CONTROL         0x0002
/*! @brief If this bit is set one or more Alt keys were held down.
*
*  If this bit is set one or more Alt keys were held down.
*/
#define X_MOD_ALT             0x0004
/*! @brief If this bit is set one or more Super keys were held down.
*
*  If this bit is set one or more Super keys were held down.
*/
#define X_MOD_SUPER           0x0008
/*! @brief If this bit is set the Caps Lock key is enabled.
*
*  If this bit is set the Caps Lock key is enabled and the @ref
*  X_LOCK_KEY_MODS input mode is set.
*/
#define X_MOD_CAPS_LOCK       0x0010
/*! @brief If this bit is set the Num Lock key is enabled.
*
*  If this bit is set the Num Lock key is enabled and the @ref
*  X_LOCK_KEY_MODS input mode is set.
*/
#define X_MOD_NUM_LOCK        0x0020

/*! @} */

/*! @defgroup buttons Mouse buttons
*  @brief Mouse button IDs.
*
*  See [mouse button input](@ref input_mouse_button) for how these are used.
*
*  @ingroup input
*  @{ */
#define X_MOUSE_BUTTON_1         0
#define X_MOUSE_BUTTON_2         1
#define X_MOUSE_BUTTON_3         2
#define X_MOUSE_BUTTON_4         3
#define X_MOUSE_BUTTON_5         4
#define X_MOUSE_BUTTON_6         5
#define X_MOUSE_BUTTON_7         6
#define X_MOUSE_BUTTON_8         7
#define X_MOUSE_BUTTON_LAST      X_MOUSE_BUTTON_8
#define X_MOUSE_BUTTON_LEFT      X_MOUSE_BUTTON_1
#define X_MOUSE_BUTTON_RIGHT     X_MOUSE_BUTTON_2
#define X_MOUSE_BUTTON_MIDDLE    X_MOUSE_BUTTON_3
/*! @} */

/*! @defgroup joysticks Joysticks
*  @brief Joystick IDs.
*
*  See [joystick input](@ref joystick) for how these are used.
*
*  @ingroup input
*  @{ */
#define X_JOYSTICK_1             0
#define X_JOYSTICK_2             1
#define X_JOYSTICK_3             2
#define X_JOYSTICK_4             3
#define X_JOYSTICK_5             4
#define X_JOYSTICK_6             5
#define X_JOYSTICK_7             6
#define X_JOYSTICK_8             7
#define X_JOYSTICK_9             8
#define X_JOYSTICK_10            9
#define X_JOYSTICK_11            10
#define X_JOYSTICK_12            11
#define X_JOYSTICK_13            12
#define X_JOYSTICK_14            13
#define X_JOYSTICK_15            14
#define X_JOYSTICK_16            15
#define X_JOYSTICK_LAST          X_JOYSTICK_16
/*! @} */

/*! @defgroup gamepad_buttons Gamepad buttons
*  @brief Gamepad buttons.
*
*  See @ref gamepad for how these are used.
*
*  @ingroup input
*  @{ */
#define X_GAMEPAD_BUTTON_A               0
#define X_GAMEPAD_BUTTON_B               1
#define X_GAMEPAD_BUTTON_X               2
#define X_GAMEPAD_BUTTON_Y               3
#define X_GAMEPAD_BUTTON_LEFT_BUMPER     4
#define X_GAMEPAD_BUTTON_RIGHT_BUMPER    5
#define X_GAMEPAD_BUTTON_BACK            6
#define X_GAMEPAD_BUTTON_START           7
#define X_GAMEPAD_BUTTON_GUIDE           8
#define X_GAMEPAD_BUTTON_LEFT_THUMB      9
#define X_GAMEPAD_BUTTON_RIGHT_THUMB     10
#define X_GAMEPAD_BUTTON_DPAD_UP         11
#define X_GAMEPAD_BUTTON_DPAD_RIGHT      12
#define X_GAMEPAD_BUTTON_DPAD_DOWN       13
#define X_GAMEPAD_BUTTON_DPAD_LEFT       14
#define X_GAMEPAD_BUTTON_LAST            X_GAMEPAD_BUTTON_DPAD_LEFT

#define X_GAMEPAD_BUTTON_CROSS       X_GAMEPAD_BUTTON_A
#define X_GAMEPAD_BUTTON_CIRCLE      X_GAMEPAD_BUTTON_B
#define X_GAMEPAD_BUTTON_SQUARE      X_GAMEPAD_BUTTON_X
#define X_GAMEPAD_BUTTON_TRIANGLE    X_GAMEPAD_BUTTON_Y
/*! @} */

/*! @defgroup gamepad_axes Gamepad axes
*  @brief Gamepad axes.
*
*  See @ref gamepad for how these are used.
*
*  @ingroup input
*  @{ */
#define X_GAMEPAD_AXIS_LEFT_X        0
#define X_GAMEPAD_AXIS_LEFT_Y        1
#define X_GAMEPAD_AXIS_RIGHT_X       2
#define X_GAMEPAD_AXIS_RIGHT_Y       3
#define X_GAMEPAD_AXIS_LEFT_TRIGGER  4
#define X_GAMEPAD_AXIS_RIGHT_TRIGGER 5
#define X_GAMEPAD_AXIS_LAST          X_GAMEPAD_AXIS_RIGHT_TRIGGER
}

namespace flora
{
	enum class EWindowType
	{
		WT_Glfw,
		WT_Egl,

		WT_MAX_COUNT,
	};

	struct XWindowCreateDesc
	{
		std::string Name = "XTestWindow";

		int GpuIndex{ 0 };

		int Width = 1280;
		int Height = 720;

		bool bHideWindow = false;
	};

	class XWindow
	{
	protected:
		XWindow() = default;
	public:
		virtual ~XWindow() = default;

		/*
			non-copyable
		*/

		XWindow(const XWindow&) = delete;
		XWindow(XWindow&&) = delete;

		void operator=(const XWindow&) = delete;
		void operator=(XWindow&&) = delete;
	public:
		template<EWindowType Type>
		static std::shared_ptr<XWindow> InitWindow(const XWindowCreateDesc&);
	public:
		virtual void Terminate() = 0;

		virtual void SwapBuffers() = 0;
		virtual void PollEvents() = 0;

		virtual bool IsWindowShouldClose() = 0;

		virtual void SetSwapInterval(int) = 0;

		virtual void* GetProcAddressCallbackFunc() = 0;

		virtual double GetTime() = 0;

		virtual void MakeContextCurrent() = 0;

		virtual int GetKey(int) = 0;

		virtual int GetMouseButton(int) = 0;

		virtual void GetCursorPos(double*, double*) = 0;

		virtual void* GetHandle() = 0;
	public:
		virtual EWindowType GetType()const = 0;
		inline const XWindowCreateDesc& GetDesc() const{ return Desc; };
	protected:
		XWindowCreateDesc Desc;
	};
}