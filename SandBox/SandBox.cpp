#include "EntryPoint.h"
#include "Flora.h"
#include "Editor.h"
#include "DebugScene_0.h"

class FloraEditor :public FApplication
{
public:
	FloraEditor()
	{
		InitProjectSettings();
		LayerStack.push_back(new EditorLayer);
		LayerStack.push_back(new DebugScene_0);
	};

	virtual void InitProjectSettings()override
	{
		//use opengl render interface
		AppName = "Sand Box";
		Width = 1280;
		Height = 720;

		RHIType = E_RHI_OpenGL;
	}
};

void CreateApplication()
{
	FApplication::CreateContext(new FloraEditor);
}

