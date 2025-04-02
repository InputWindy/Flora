#ifdef WITH_GLFW
#include <Window/GLFW/GlfwWindow.h>
#include <GLFW/glfw3.h>

flora::XGlfwWindow::XGlfwWindow(const XWindowCreateDesc& InDesc)
{
    Desc = InDesc;

    glfwInit();
    glfwDefaultWindowHints();
    if (Desc.bHideWindow)
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    }

    Handle = glfwCreateWindow(Desc.Width, Desc.Height, Desc.Name.c_str(), NULL, NULL);
    glfwMakeContextCurrent((GLFWwindow*)Handle);
}

void flora::XGlfwWindow::Terminate()
{
    glfwDestroyWindow((GLFWwindow*)Handle);
    glfwTerminate();
}

void flora::XGlfwWindow::SwapBuffers()
{
    glfwSwapBuffers((GLFWwindow*)Handle);
}

void flora::XGlfwWindow::PollEvents()
{
    glfwPollEvents();
}

bool flora::XGlfwWindow::IsWindowShouldClose()
{
    return glfwWindowShouldClose((GLFWwindow*)Handle);
}

void flora::XGlfwWindow::SetSwapInterval(int interval)
{
    glfwSwapInterval(interval);
}

void* flora::XGlfwWindow::GetProcAddressCallbackFunc()
{
    return (void*)glfwGetProcAddress;
}

double flora::XGlfwWindow::GetTime()
{
    return glfwGetTime();
}

void flora::XGlfwWindow::MakeContextCurrent()
{
    glfwMakeContextCurrent((GLFWwindow*)Handle);
}

int flora::XGlfwWindow::GetKey(int key)
{
    return glfwGetKey((GLFWwindow*)Handle, key);
}

int flora::XGlfwWindow::GetMouseButton(int key)
{
    return glfwGetMouseButton((GLFWwindow*)Handle, key);
}

void flora::XGlfwWindow::GetCursorPos(double* xpos, double* ypos)
{
    glfwGetCursorPos((GLFWwindow*)Handle, xpos, ypos);

}

void* flora::XGlfwWindow::GetHandle()
{
    return Handle;
}

flora::EWindowType flora::XGlfwWindow::GetType() const
{
    return EWindowType::WT_Glfw;
}
#endif