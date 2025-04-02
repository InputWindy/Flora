#include <Window/Window.h>

#ifdef WITH_GLFW
#include <Window/GLFW/GlfwWindow.h>
template<>
std::shared_ptr<XVerse::XWindow> XVerse::XWindow::InitWindow<XVerse::EWindowType::WT_Glfw>(const XVerse::XWindowCreateDesc& InDesc)
{
	return std::make_shared<XGlfwWindow>(InDesc);
}
#endif

#ifdef WITH_EGL
#include <Window/EGL/EglWindow.h>
template<>
std::shared_ptr<XVerse::XWindow> XVerse::XWindow::InitWindow<XVerse::EWindowType::WT_Egl>(const XVerse::XWindowCreateDesc& InDesc)
{
	return std::make_shared<XEglWindow>(InDesc);
}
#endif