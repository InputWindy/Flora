#ifdef WITH_GLFW
#include <Window/GLFW/GlfwWindow.h>
#include <GLFW/glfw3.h>

XVerse::XGlfwWindow::XGlfwWindow(const XWindowCreateDesc& InDesc)
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

void XVerse::XGlfwWindow::Terminate()
{
    glfwDestroyWindow((GLFWwindow*)Handle);
    glfwTerminate();
}

void XVerse::XGlfwWindow::SwapBuffers()
{
    glfwSwapBuffers((GLFWwindow*)Handle);
}

void XVerse::XGlfwWindow::PollEvents()
{
    glfwPollEvents();
}

bool XVerse::XGlfwWindow::IsWindowShouldClose()
{
    return glfwWindowShouldClose((GLFWwindow*)Handle);
}

void XVerse::XGlfwWindow::SetSwapInterval(int interval)
{
    glfwSwapInterval(interval);
}

void* XVerse::XGlfwWindow::GetProcAddressCallbackFunc()
{
    return (void*)glfwGetProcAddress;
}

double XVerse::XGlfwWindow::GetTime()
{
    return glfwGetTime();
}

void XVerse::XGlfwWindow::MakeContextCurrent()
{
    glfwMakeContextCurrent((GLFWwindow*)Handle);
}

int XVerse::XGlfwWindow::GetKey(int key)
{
    return glfwGetKey((GLFWwindow*)Handle, key);
}

int XVerse::XGlfwWindow::GetMouseButton(int key)
{
    return glfwGetMouseButton((GLFWwindow*)Handle, key);
}

void XVerse::XGlfwWindow::GetCursorPos(double* xpos, double* ypos)
{
    glfwGetCursorPos((GLFWwindow*)Handle, xpos, ypos);

}

void* XVerse::XGlfwWindow::GetHandle()
{
    return Handle;
}

XVerse::EWindowType XVerse::XGlfwWindow::GetType() const
{
    return EWindowType::WT_Glfw;
}
#endif