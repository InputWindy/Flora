#include <Application/Application.h>

namespace flora
{
	void IApp::StartUp()
	{
		XLogger::InitLogger(LogPath);
		if (bUseGPU)
		{
#ifdef WITH_GLFW
			MainWindow = XWindow::InitWindow<EWindowType::WT_Glfw>(WindowDesc);
#elif WITH_EGL
			MainWindow = XWindow::InitWindow<EWindowType::WT_Egl>(WindowDesc);
#endif // WITH_GLFW

			Rhi = XRHI::InitRHI<ERHIFeatureLevel::FL_OpenGL>(MainWindow->GetProcAddressCallbackFunc());

#ifdef WITH_GUI
			Editor = IEditor::InitEditor<EEditorType::ET_ImGui>(MainWindow->GetHandle());
#endif // WITH_GUI
		}
	}


	void IApp::Shutdown()
	{
		if (bUseGPU)
		{
			if (Editor)
			{
				Editor->Terminate();
			}

			MainWindow->Terminate();
		}
	}
}