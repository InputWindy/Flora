#pragma once
#include "Core.h"
#include "RenderCommand.h"
#include <vector>
#include <string>
class FLORA_API FLayer
{
protected:
	FLayer() = default;
public:
	virtual ~FLayer() = default;

	virtual void OnAwake() = 0;
public:
	virtual void OnUpdate() = 0;
	virtual void OnGUI() = 0;
};

class FLORA_API FApplication
{
protected:
	FApplication();
public:
	//virtual
	virtual ~FApplication();

	static inline void CreateContext(FApplication* app);
	static inline FApplication*   GetApp();
	static inline FRenderCommand* GetRHI();
public:
	//un virtual
	void MainLoop();
public:
	//inline
	inline std::string	GetAppName();
	inline double		GetAppTime();
	inline uint32_t		GetWindowWidth();
	inline uint32_t		GetWindowHeight();
private:
	void BeginFrame();
	void EndFrame();

	void RenderScene();
	void UpdateScene();
protected:
	std::vector<FLayer*> LayerStack;

private:
	void*	WindowHandle = 0;
protected:
	//================//
	//	  Settings    //
	//================//
	std::string			AppName = "Flora";
	uint32_t			Width = 1280;
	uint32_t			Height = 720;
	ERenderCommandType  RHIType = E_RHI_OpenGL;

	virtual void InitProjectSettings() = 0;
private:
	void LoadProjectSettings();
};

