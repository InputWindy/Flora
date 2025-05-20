#include <CoreMinimal.h>
#include "Editor.h"

#include "Scene/Allocator.h"
#include "Scene/Scene.h"
#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Scene/System.h"

#include <iostream>

#define STR_CAT(Str1,Str2) Str1##Str2

std::string flora::ExcutePath;

using namespace flora;


struct Component:public IScene
{
	DECLARE_TYPE(Component)

	virtual ~Component(){}

	XTransform Trans;
};

struct Entity :public IEntity
{
	DECLARE_TYPE(Entity)

		Entity()
	{
		Component* C = Scene->AllocC<Component>();
		C->Trans;

		Scene->Attach(this, C);
	}

	virtual ~Entity() {}

};

struct System:public ISystem
{
	DECLARE_TYPE(System)
	virtual void OnUpdate(IScene*, float /*delta time*/)
	{
		std::cout << "System OnUpdate" << std::endl;
	};
	virtual void OnStart(IScene*) 
	{
		std::cout << "System OnStart" << std::endl;
	};;
	virtual void OnEnd(IScene*)
	{
		std::cout << "System OnEnd" << std::endl;
	};;
};

struct System2 :public ISystem
{
	DECLARE_TYPE(System2)
		virtual void OnUpdate(IScene*, float /*delta time*/)
	{
		std::cout << "System2 OnUpdate" << std::endl;
	};
	virtual void OnStart(IScene*)
	{
		std::cout << "System2 OnStart" << std::endl;
	};;
	virtual void OnEnd(IScene*)
	{
		std::cout << "System2 OnEnd" << std::endl;
	};;
};

struct Scene :public IScene
{
	DECLARE_TYPE(Scene)

		Scene()
	{
		AddSystem<System>();
		AddSystem<System2>();
	}

	virtual ~Scene() {}
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

		Scene* scene = TAllocator<Scene>::Alloc();
		scene->Start();
		scene->Update();
		scene->End();


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

