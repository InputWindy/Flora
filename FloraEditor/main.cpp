#include <CoreMinimal.h>
#include "Editor.h"



#include <iostream>

#define STR_CAT(Str1,Str2) Str1##Str2

std::string flora::ExcutePath;

using namespace flora;

DECLARE_SURFACE_SHADER(FSurfaceShader,"Surface.vert","Surface.frag")

struct FloraEditorApp : public flora::IApp
{
	FloraEditorApp(int argc, char* argv[])
	{
		WindowDesc.Name = "FloraEditor";
		WindowDesc.Width = 1024;
		WindowDesc.Height = 1024;
		WindowDesc.bHideWindow = true;

		ExcutePath = std::filesystem::canonical(std::filesystem::current_path() / std::filesystem::path(argv[0])).parent_path().generic_string();
		LogPath = "log.txt";

		ParseCmdline(argc, argv);
	}

	virtual void StartUp()override
	{
		IApp::StartUp();
		XMaterial::ImportShaderHeaderFiles(SHADER_ROOT_DIR);
		XMaterial::ImportShaderHeaderFiles(STR_CAT(SHADER_ROOT_DIR, "Core/"));

		XMaterial::ImportShaderHeaderFiles(ExcutePath + "/Shaders/");
		XMaterial::ImportShaderHeaderFiles(ExcutePath + "/Shaders/Core/");

		FScopeShader<FSurfaceShader> SurfaceShader;
		if (SurfaceShader.IsValid())
		{
			FLORA_CORE_INFO("test")
		}
	}

	//Main Loop
	virtual void Run()override
	{
		//TODO:Do Something Before Run

		//while (BeginFrame())
		//{
		//	//UpdateCamera();
		//	//UpdateGameScene();
		//	//RenderScene();

		//#ifdef WITH_GUI
		//	Editor->BeginRender();
		//	Editor->Render();
		//	Editor->EndRender();
		//#endif
		//	EndFrame();
		//}

		//TODO:Do Something After Run
	}

	private:
	virtual void ParseCmdline(int argc, char* argv[])
	{
		
	};
};

#ifdef PY_MODULE_NAME
#include "PyRegister.h"
PY_REGISTER(FloraEditor,

	.def(py::init<>())

	.def("start_up", &FloraEditorApp::StartUp)
	.def("run", &FloraEditorApp::Run)
	.def("shutdown", &FloraEditorApp::Shutdown)
	, FloraEditorApp
)
PYBIND11_MODULE(PY_MODULE_NAME, m) 
{
    m.doc() =
        R"(
            xpydec - flora model decimator python library .    
        )";

    PyRegister::Get().Exec(m);
}
#else
#include <EntryPoint.h>
IMPL_APP(FloraEditorApp)
#endif

