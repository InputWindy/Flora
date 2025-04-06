#pragma once 
#include <CoreMinimal.h>

#include <iostream>

using namespace flora;

class XEditor:public flora::IEditor
{
public:
	XEditor() = default;
	XEditor(void* InWindowHandle);
	virtual ~XEditor() = default;

public:
	virtual void Terminate()override;
	virtual void BeginRender()override;
	virtual void Render()override;
	virtual void EndRender()override;

protected:
	void MainMenu();
};
