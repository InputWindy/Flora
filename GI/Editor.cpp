#include "Editor.h"
#ifdef WITH_GUI

template<>
std::shared_ptr<XVerse::IEditor> XVerse::IEditor::InitEditor<EEditorType::ET_ImGui>(void* InWindowHandle)
{
	return std::make_shared<XEditor>(InWindowHandle);
};

XEditor::XEditor(void* InWindowHandle)
{
	IMGUI_CHECKVERSION();
	if (!ImGui::CreateContext())assert(0);
	if (!ImPlot::CreateContext())assert(0);

	ImGuiIO& io = ImGui::GetIO();
	io.BackendFlags = ImGuiBackendFlags_HasSetMousePos | ImGuiBackendFlags_HasMouseCursors;
	io.ConfigFlags = ImGuiConfigFlags_DockingEnable;

	//ImGui::StyleColorsLight();

	ImFont* Chinese = io.Fonts->AddFontFromFileTTF(
		"C:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)InWindowHandle, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");
}

void XEditor::Terminate()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImPlot::DestroyContext();
	ImGui::DestroyContext();
}

void XEditor::BeginRender()
{
	GetRHI()->ClearFrameBuffer(CBB_COLOR_BUFFER_BIT | CBB_DEPTH_BUFFER_BIT | CBB_STENCIL_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void XEditor::Render()
{
	MainMenu();
}

void XEditor::EndRender()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}
void XEditor::MainMenu()
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			//ImGui::Separator();
			//ImGui::MenuItem("Content Browser", nullptr);
			//ImGui::MenuItem("World Outliner", nullptr);
			//ImGui::MenuItem("Console", nullptr);
			//ImGui::MenuItem("Profiling", nullptr);

			//ImGui::Separator();
			//ImGui::MenuItem("Style Editor", nullptr);
			///*ImGui::MenuItem("Game Scene", nullptr, &bOpenGameScene);
			//ImGui::MenuItem("Hierarchy", nullptr, &bOpenHierarchy);
			//ImGui::MenuItem("Inspector", nullptr, &bOpenInspector);*/
			//ImGui::Separator();
			//ImGui::MenuItem("ResourceViewer", nullptr);
			ImGui::EndMenu();
		};

		ImGui::EndMainMenuBar();
	};
}
#endif
