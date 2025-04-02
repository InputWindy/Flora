#include <Window/Window.h>

#ifdef WITH_GLFW
#include <Window/GLFW/GlfwWindow.h>
template<>
std::shared_ptr<flora::XWindow> flora::XWindow::InitWindow<flora::EWindowType::WT_Glfw>(const flora::XWindowCreateDesc& InDesc)
{
	return std::make_shared<XGlfwWindow>(InDesc);
}
#endif

#ifdef WITH_EGL
#include <Window/EGL/EglWindow.h>
template<>
std::shared_ptr<flora::XWindow> flora::XWindow::InitWindow<flora::EWindowType::WT_Egl>(const flora::XWindowCreateDesc& InDesc)
{
	return std::make_shared<XEglWindow>(InDesc);
}
#endif