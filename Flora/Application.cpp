#include "Application.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "Render.h"
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
}

inline std::string FApplication::GetAppName()
{
	return AppName;
}

inline double FApplication::GetAppTime()
{
	return glfwGetTime();
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

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

unsigned int shaderProgram;

float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
};

unsigned int VBO, VAO;

void FApplication::RenderScene()
{
	auto init = [&]()->bool
	{
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		}
		// fragment shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		}

		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//====================Mesh=======================//
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		return true;
	};
	static bool b_init = init();
	
	FRenderer Renderer(Width,Height);
	{
		Ref<FCanvas> Canvas = GetRHI()->GenerateCanvas(Width, Height);
		
		Canvas->Attach();
		Canvas->BindOutput(Renderer.GetBackBuffer(),0);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		Canvas->ClearAll();

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); 
		glDrawArrays(GL_TRIANGLES, 0, 3);

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


