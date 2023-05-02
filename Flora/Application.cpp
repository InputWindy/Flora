#include "Application.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Render.h"
#include "Type.h"
#include "Misc.h"
#include <glad.h>
#include <glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <implot.h>
#include <ImGuizmo.h>
#include <imgui_node_editor.h>
#pragma comment(lib,"glfw3.lib")
#pragma comment(lib,"ImGui.lib")

FApplication* GEngine;
FRenderCommand* GRhi;
FApplication::FApplication()
{
	//create native window
	glfwInit();
	glfwDefaultWindowHints();

	WindowHandle = glfwCreateWindow(Width,Height, AppName.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent((GLFWwindow*)WindowHandle);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	//init imgui
	IMGUI_CHECKVERSION();
	if (!ImGui::CreateContext())assert(0);
	if (!ImPlot::CreateContext())assert(0);

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags = ImGuiBackendFlags_HasSetMousePos | ImGuiBackendFlags_HasMouseCursors;
	io.ConfigFlags = ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

	ImFont* Chinese = io.Fonts->AddFontFromFileTTF(
		"C:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)WindowHandle, true);
	ImGui_ImplOpenGL3_Init("#version 450");

}

FApplication::~FApplication()
{
	for (FLayer* Layer : LayerStack)
	{
		delete Layer;
	}

	//FResourceManager::Get().Save();

	delete GRhi;
	GRhi = nullptr;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	glfwDestroyWindow((GLFWwindow*)WindowHandle);
	glfwTerminate();
}

void FApplication::CreateContext(FApplication* app)
{
	GEngine = app;
	GEngine->LoadProjectSettings();
	GRhi->InitResources();
}

inline FApplication* FApplication::GetApp()
{
	return GEngine;
}

inline FRenderCommand* FApplication::GetRHI()
{
	return GRhi;
}

void FApplication::MainLoop()
{
	for (FLayer* Layer : LayerStack)
	{
		Layer->OnAwake();
	}

	AwakeScene();

	while (!glfwWindowShouldClose((GLFWwindow*)WindowHandle))
	{
		BeginFrame();

		for (FLayer* Layer : LayerStack)
		{
			Layer->OnUpdate();
		}

		UpdateScene();
		RenderScene();

		for (FLayer* Layer : LayerStack)
		{
			Layer->OnGUI();
		}

		EndFrame();
	}

	ReleaseScene();

	for (FLayer* Layer : LayerStack)
	{
		Layer->OnRelease();
	}
}

inline std::string FApplication::GetAppName()
{
	return AppName;
}

inline double FApplication::GetAppTime()
{
	return ImGui::GetTime();
}

inline double FApplication::GetAppDeltaTime()
{
	return ImGui::GetIO().DeltaTime;
}

inline uint32_t FApplication::GetWindowWidth()
{
	return Width;
}

inline uint32_t FApplication::GetWindowHeight()
{
	return Height;
}

void FApplication::BeginFrame()
{
	glfwMakeContextCurrent((GLFWwindow*)WindowHandle);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void FApplication::EndFrame()
{
	//ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();

	glfwPollEvents();
	glfwSwapBuffers((GLFWwindow*)WindowHandle);
}

void FApplication::RenderScene()
{
	FRenderCommand* Cmd = FApplication::GetRHI();

	FRenderer Renderer(Width,Height);
	{
		Ref<FCanvas> Canvas = GetRHI()->GenerateCanvas(Width, Height);
		
		Canvas->Attach();
		Canvas->BindOutput(Renderer.GetBackBuffer(),0);
		Canvas->ClearAll();

		Ref<FMaterial> TestShader = FResourceManager::Get().FindObject<FMaterial>("TestShader");
		Cmd->BindShader(TestShader->GetHandle());
		{
			Ref<FScene> CurrentScene = FSceneManager::Get().GetCurrentScene();
			if (CurrentScene and CurrentScene->GetDebugCamera())
			{
				Ref<FCameraObject> CameraObject = CurrentScene->GetDebugCamera();
				FTransform& Transform = CameraObject->GetComponent<FTransformComponent>()->GetTransform();
				FCamera* Camera = CameraObject->GetComponent<FCameraComponent>()->GetCamera();

				mat4 model =
				{
					0.01,0,0,0,
					0,0.01,0,0,
					0,0,0.01,0,
					0,0,0,1
				};
				Cmd->SetMat4("model", TestShader->GetHandle(), model);
				Cmd->SetMat4("view", TestShader->GetHandle(), Transform.GetViewMatrix());
				Cmd->SetMat4("projection", TestShader->GetHandle(), Camera->GetProjection());
			}

		}

		Ref<FMesh> Sphere = FResourceManager::Get().FindObject<FMesh>("Sphere");
		if (Sphere)
		{
			Sphere->Draw(EDrawMode_TRIANGLES);
		}

		Canvas->Dettach();
	}
	FGameScene::GetMainScene().SetBackBuffer(Renderer.GetBackBuffer());
}

void FApplication::UpdateScene()
{
	FSceneManager& SceneManager = FSceneManager::Get();
	SceneManager.BeginUpdate();
	SceneManager.Update();
	SceneManager.EndUpdate();
}

void FApplication::AwakeScene()
{
	FSceneManager& SceneManager = FSceneManager::Get();
	SceneManager.Awake();
}

void FApplication::ReleaseScene()
{
	FSceneManager& SceneManager = FSceneManager::Get();
	SceneManager.Release();
}

void FApplication::LoadProjectSettings()
{
	glfwSetWindowTitle((GLFWwindow*)WindowHandle, AppName.c_str());
	glfwSetWindowSize((GLFWwindow*)WindowHandle,Width,Height);
	GRhi = FRenderCommand::Create(GEngine->RHIType);
}

#include <windows.h>
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


