#ifdef WITH_EGL

#include <Window/EGL/EglWindow.h>

//#include <GLFW/src/egl_context.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

static inline EGLDisplay _sDisplay;
static inline EGLContext _sContext;
static inline EGLSurface _sSurface;

XVerse::EWindowType XVerse::XEglWindow::GetType() const
{
    return EWindowType::WT_Egl;
}

XVerse::XEglWindow::XEglWindow(const XWindowCreateDesc& InDesc)
{
	// std::lock_guard<std::mutex> lock(egl_mu_);
	// select a card and create context
	static const int MAX_DEVICES = 10;
	EGLDeviceEXT eglDevs[MAX_DEVICES];
	EGLint numDev;
	PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT = (PFNEGLQUERYDEVICESEXTPROC)eglGetProcAddress("eglQueryDevicesEXT");
	eglQueryDevicesEXT(MAX_DEVICES, eglDevs, &numDev);
	PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT =
		(PFNEGLGETPLATFORMDISPLAYEXTPROC)
		eglGetProcAddress("eglGetPlatformDisplayEXT");
	//  选择显卡、双显卡0、1可以创建display，会有2、3的额外设备，但是创建出的display为空。
	_sDisplay = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, eglDevs[InDesc.GpuIndex], 0);

	EGLint major, minor;

	EGLint num_con;
	EGLint attrlist[] = {
		EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_NONE };
	EGLint contextAttrs[] = {
		EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT,
		EGL_CONTEXT_MAJOR_VERSION, 4,
		EGL_CONTEXT_MINOR_VERSION, 6,
		EGL_NONE };

	eglInitialize(_sDisplay, &major, &minor);

	EGLConfig config1;
	eglChooseConfig(_sDisplay, attrlist, &config1, 1, &num_con);
	eglBindAPI(EGL_OPENGL_API);
	_sSurface = eglCreatePbufferSurface(_sDisplay, config1, nullptr);
	_sContext = eglCreateContext(_sDisplay, config1, EGL_NO_CONTEXT, contextAttrs);
	bool res;
	res = eglMakeCurrent(_sDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, _sContext);
	// if (!res) {
	// 	throw InitException(InitErrno::EGL, "init egl failed");
	// }
}

void XVerse::XEglWindow::Terminate()
{
	eglMakeCurrent(_sDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroySurface(_sDisplay, _sSurface);
	eglDestroyContext(_sDisplay, _sContext);
	eglReleaseThread();
	eglTerminate(_sDisplay);
}

void XVerse::XEglWindow::SwapBuffers()
{
	eglSwapBuffers(_sDisplay, _sSurface);

}

void XVerse::XEglWindow::PollEvents()
{
	eglWaitGL();

}

bool XVerse::XEglWindow::IsWindowShouldClose()
{
    return false;
}

void XVerse::XEglWindow::SetSwapInterval(int interval)
{
	eglSwapInterval(_sDisplay, interval);

}

void* XVerse::XEglWindow::GetProcAddressCallbackFunc()
{
	return (void*)eglGetProcAddress;
}

double XVerse::XEglWindow::GetTime()
{
    return 0.0;
}

void XVerse::XEglWindow::MakeContextCurrent()
{
	eglMakeCurrent(_sDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, _sContext);

}

int XVerse::XEglWindow::GetKey(int)
{
    return 0;
}

int XVerse::XEglWindow::GetMouseButton(int)
{
    return 0;
}

void XVerse::XEglWindow::GetCursorPos(double*, double*)
{
}
void* XVerse::XEglWindow::GetHandle()
{
	return Handle;
}
#endif
