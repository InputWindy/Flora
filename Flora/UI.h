#pragma once
#include "Core.h"
#include "imgui.h"
#include "imgui_internal.h"
struct FLORA_API FUI
{
	//window menu
	static bool BeginMainMenuBar();
	static bool BeginMenu(const char* label,bool enabled = true);
	static bool MenuItem(const char* label, const char* shortcut, bool* p_selected, bool enabled = true);
	static void EndMenu();
	static void EndMainMenuBar();

	//new window
	static bool	Begin(const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0);
	static void	End();

	//demo
	static void ShowDemoWindow(bool* p_open = nullptr);

	//window item
	static void BeginGroup();
	static void EndGroup();
	static bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);
	static void OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags = 0);
	static void EndPopup();
	static void CloseCurrentPopup();

	static bool SmallButton(const char* label);
	static bool ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 1), const ImVec2& uv1 = ImVec2(1, 0), int frame_padding = -1, const ImVec4& bg_col = ImVec4(0, 0, 0, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1));
	static void Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0 = ImVec2(0, 1), const ImVec2& uv1 = ImVec2(1, 0), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
	static bool Button(const char* label, const ImVec2& size = ImVec2(0, 0));
	static bool ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags = 0);
	static bool BeginTable(const char* str_id, int column, ImGuiTableFlags flags = 0, const ImVec2& outer_size = ImVec2(0.0f, 0.0f), float inner_width = 0.0f);
	static void EndTable();
	static void TextWrapped(const char* str);
	static bool BeginDragDropSource();
	static bool BeginDragDropTarget();
	static bool SetDragDropPayload(const char* type, const void* data, size_t sz, ImGuiCond cond = 0);
	static const ImGuiPayload* AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags = 0);
	static void EndDragDropSource();
	static void EndDragDropTarget();
	static bool IsMouseDoubleClicked(ImGuiMouseButton button);
	static bool IsItemClicked(ImGuiMouseButton button);
	static bool IsItemHovered();
	static bool IsMouseClicked(ImGuiMouseButton button, bool repeat = false);

	static void Columns(int count = 1, const char* id = NULL, bool border = true);
	static void NextColumn();
	static void TableSetupColumn(const char* label, ImGuiTableColumnFlags flags = 0, float init_width_or_weight = 0.0f, ImGuiID user_id = 0);
	static void TableHeadersRow();
	static void TableNextRow();
	static bool TableSetColumnIndex(int column_n);
	static void TextColored(const ImVec4& color,const char* fmt,...);
	static void Text(const char* fmt, ...);
	static bool CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags = 0);
	static bool TreeNode(const char* label);
	static void LabelText(const char* label, const char* fmt, ...);
	static void TreePop();

	//misc
	static bool ArrowButton(const char* str_id, ImGuiDir dir);

	//dock
	static ImGuiID          DockSpace(ImGuiID id, const ImVec2& size = ImVec2(0, 0), ImGuiDockNodeFlags flags = 0, const ImGuiWindowClass* window_class = nullptr);

	//utils
	static ImGuiViewport*	GetMainViewport();
	static ImVec2			GetContentRegionAvail();
	static ImGuiID          GetID(const char* str_id);
	static void				SetNextWindowPos(const ImVec2& pos, ImGuiCond cond = 0, const ImVec2& pivot = ImVec2(0, 0));
	static void				SetNextWindowSize(const ImVec2& size, ImGuiCond cond = 0);
	static void				SetNextWindowViewport(ImGuiID viewport_id);

	static void PushStyleVar(ImGuiStyleVar idx, float val);
	static void PopStyleVar(int cnt = 1);
	static void PushStyleColor(ImGuiCol idx, const ImVec4& col);
	static void PopStyleColor(int cnt = 1);
	
	static void SameLine();
	static void	Separator();
	static void SeparatorEx(ImGuiSeparatorFlags flags);

	//window
	static ImGuiWindow* GetCurrentWindow();

public:
	typedef void(*DragDropTargetCallback)(const ImGuiPayload*,void*);

	//Flora
	static void DragDropTargetSpace(const char* Category, DragDropTargetCallback Callback = nullptr,void* UserData = nullptr);
};

