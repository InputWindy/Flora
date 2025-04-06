#include <CoreMinimal.h>
#include "Editor.h"

#define STR_CAT(Str1,Str2) Str1##Str2

std::string XVerse::ExcutePath;

using namespace XVerse;

struct GIApp : public XVerse::IApp
{
	GIApp()
	{
		WindowDesc.Name = "GI";
		WindowDesc.Width = 1024;
		WindowDesc.Height = 1024;
		WindowDesc.bHideWindow = false;

		ExcutePath = std::filesystem::canonical(std::filesystem::current_path()).generic_string();
		LogPath = "log.txt";

	}

	GIApp(int argc, char* argv[])
	{
		WindowDesc.Name = "GI";
		WindowDesc.Width = 1024;
		WindowDesc.Height = 1024;
		WindowDesc.bHideWindow = false;

		ExcutePath = std::filesystem::canonical(std::filesystem::current_path() / std::filesystem::path(argv[0])).parent_path().generic_string();
		LogPath = "log.txt";

		ParseCmdline(argc, argv);
	}

	virtual ~GIApp()override
	{
		
	}

	virtual void StartUp()override
	{
		IApp::StartUp();
		XMaterial::ImportShaderHeaderFiles(SHADER_ROOT_DIR);
		XMaterial::ImportShaderHeaderFiles(STR_CAT(SHADER_ROOT_DIR, "Core/"));

		XMaterial::ImportShaderHeaderFiles(ExcutePath + "/Shaders/");
		XMaterial::ImportShaderHeaderFiles(ExcutePath + "/Shaders/Core/");

		//TODO: Init Your Application (Import Demo Scene or Other Resources)
		{

		}
	}

	virtual void Shutdown()override
	{
		//TODO: Clear Your Resources
		{

		}
		IApp::Shutdown();
	}

	//Main Loop
	virtual void Run()override
	{
		//TODO:Do Something Before Run

		while (BeginFrame())
		{
			//UpdateCamera();
			//UpdateGameScene();
			//RenderScene();

		#ifdef WITH_GUI
			Editor->BeginRender();
			Editor->Render();
			Editor->EndRender();
		#endif
			EndFrame();
		}

		//TODO:Do Something After Run
	}

	private:
	virtual void ParseCmdline(int argc, char* argv[])
	{
		
	};
};

#ifdef PY_MODULE_NAME
#include "PyRegister.h"
PY_REGISTER(GI,

	.def(py::init<>())

	.def("start_up", &GIApp::StartUp)
	.def("run", &GIApp::Run)
	.def("shutdown", &GIApp::Shutdown)
	, GIApp
)
PYBIND11_MODULE(PY_MODULE_NAME, m) 
{
    m.doc() =
        R"(
            xpydec - xverse model decimator python library .    
        )";

    PyRegister::Get().Exec(m);
}
#else
#include <EntryPoint.h>
IMPL_APP(GIApp)
#endif

