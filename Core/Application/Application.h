#pragma once
#include <Window/Window.h>
#include <GUI/EditorBase.h>
#include <Render/RHI/RHI.h>

#define IMPL_APP(AppType) \
flora::IApp* flora::CreatApp(int argc, char* argv[]) \
{ \
	return new AppType(argc, argv); \
};

namespace flora
{
	class IApp
	{
	public:
		IApp() {}
		virtual ~IApp() {}

		virtual void StartUp();
		
		virtual void Run() = 0;

		virtual void Shutdown();
	protected:
		inline bool BeginFrame()
		{
			return MainWindow->IsWindowShouldClose() == false;
		}

		inline void EndFrame()
		{
			MainWindow->PollEvents();
			MainWindow->SwapBuffers();
		}
	protected:
		virtual void ParseCmdline(int argc, char* argv[]) = 0;
	protected:
		XWindowCreateDesc WindowDesc;
	protected:
		std::shared_ptr<XWindow> MainWindow = nullptr;
		std::shared_ptr<XRHI> Rhi = nullptr;
		std::shared_ptr<IEditor> Editor = nullptr;

		std::string LogPath;

		bool bUseGPU = true;
	};

	extern "C" IApp * CreatApp(int argc, char* argv[]);
}

