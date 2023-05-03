#include "EntryPoint.h"
#include "Flora.h"
#include "EditorScript.h"

class FloraEditor :public FApplication
{
public:
	FloraEditor()
	{
		InitProjectSettings();
		LayerStack.push_back(new EditorScript);
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

