#include <CoreMinimal.h>
#include "Editor.h"

#include "Scene/Allocator.h"

#include <iostream>

#define STR_CAT(Str1,Str2) Str1##Str2

std::string flora::ExcutePath;

using namespace flora;


struct FTestAlloc:public IAllocable
{
	DECLARE_TYPE(FTestAlloc)

	FTestAlloc() {

	}

	FTestAlloc(int a,float& b,double&& c)
	{
		//std::cout << a << " " << b << " " << c << std::endl;
	}

	~FTestAlloc()
	{
		std::cout << "free" << std::endl;
	}


	bool bb1 = true;
	bool bb2 = true;
	bool bb3 = true;

};

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

		//TODO: Init Your Application (Import Demo Scene or Other Resources)
		{
			int a = 0;
			float b = 1.0f;
			double c = 2.0f;
			//FTestAlloc Test(a, b, std::move(c));

			std::vector<FTestAlloc*> Tests;
			
			for (size_t i = 0; i < 12; i++)
			{
				++a;
				++b;
				++c;
				Tests.push_back(TAllocator<FTestAlloc>::Alloc(a,b,std::move(c)));
			}

			for (auto T : TAllocator<FTestAlloc>::GetUsed())
			{
				std::cout << "Used:" << (unsigned int)T << " ";
			}

			std::cout << std::endl;

			for (FTestAlloc* T : Tests)
			{
				std::cout << "Addr:" << (unsigned int)T << " ";
			}

			std::cout << "=======1========" << std::endl;

			TAllocator<FTestAlloc>::Dealloc(Tests[3]);
			TAllocator<FTestAlloc>::Dealloc(Tests[4]);
			TAllocator<FTestAlloc>::Dealloc(Tests[7]);
			TAllocator<FTestAlloc>::Dealloc(Tests[9]);

			for (auto T : TAllocator<FTestAlloc>::GetUsed())
			{
				std::cout << "Used:" << (unsigned int)T << " ";
			}

			std::cout << std::endl;

			for (FTestAlloc* T : Tests)
			{
				std::cout << "Addr:" << (unsigned int)T << " ";
			}

			std::cout << "=======2========" << std::endl;

			for (size_t i = 0; i < 8; i++)
			{
				++a;
				++b;
				++c;
				Tests.push_back(TAllocator<FTestAlloc>::Alloc(a, b, std::move(c)));
			}

			for (auto T : TAllocator<FTestAlloc>::GetUsed())
			{
				std::cout << "Used:" << (unsigned int)T << " ";
			}

			std::cout << std::endl;

			for (FTestAlloc* T : Tests)
			{
				std::cout << "Addr:" << (unsigned int)T << " ";
			}

			std::cout << "=======3========" << std::endl;

			std::cout << TAllocator<FTestAlloc>::MaxSize << std::endl;
			std::cout << SMALL_MEMORY_POOL << std::endl;



			/*auto [NumUsed, NumUnused] = TAllocator<FTestAlloc>::GetNum();

			std::cout << "Mem Num:" << NumUsed << " " << NumUnused << std::endl;*/

			
		}

		system("pause");

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

