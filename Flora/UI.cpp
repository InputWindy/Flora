#include "UI.h"

void FUI::ShowDemoWindow(bool* p_open)
{
	ImGui::ShowDemoWindow(p_open);
}

void FUI::BeginGroup()
{
	ImGui::BeginGroup();
}

void FUI::EndGroup()
{
	ImGui::EndGroup();
}

bool FUI::BeginPopup(const char* str_id, ImGuiWindowFlags flags)
{
	return ImGui::BeginPopup(str_id,flags);
}

void FUI::OpenPopup(const char* str_id, ImGuiPopupFlags popup_flags)
{
	ImGui::OpenPopup(str_id,popup_flags);
}

void FUI::EndPopup()
{
	ImGui::EndPopup();
}

void FUI::CloseCurrentPopup()
{
	ImGui::CloseCurrentPopup();
}

bool FUI::SmallButton(const char* label)
{
	return ImGui::SmallButton(label);
};

bool FUI::ImageButton(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, int frame_padding, const ImVec4& bg_col, const ImVec4& tint_col)
{
	return ImGui::ImageButton(user_texture_id,size,uv0,uv1,frame_padding,bg_col, tint_col);
}

void FUI::Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col)
{
	ImGui::Image(user_texture_id,size,uv0,uv1,tint_col,border_col);
}

bool FUI::Button(const char* label, const ImVec2& size)
{
	return ImGui::Button(label,size);
}

bool FUI::ColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags)
{
	return ImGui::ColorEdit4(label,col,flags);
}

bool FUI::BeginTable(const char* str_id, int column, ImGuiTableFlags flags, const ImVec2& outer_size, float inner_width)
{
	return ImGui::BeginTable(str_id,column,flags,outer_size,inner_width);
}

void FUI::EndTable()
{
	ImGui::EndTable();
}

void FUI::TextWrapped(const char* str)
{
	ImGui::TextWrapped(str);
}

void FUI::NextColumn()
{
	ImGui::NextColumn();
}

bool FUI::BeginDragDropSource()
{
	return ImGui::BeginDragDropSource();
}

bool FUI::BeginDragDropTarget()
{
	return ImGui::BeginDragDropTarget();
}

void FUI::TableSetupColumn(const char* label, ImGuiTableColumnFlags flags, float init_width_or_weight, ImGuiID user_id)
{
	ImGui::TableSetupColumn(label, flags, init_width_or_weight, user_id);
}

bool FUI::SetDragDropPayload(const char* type, const void* data, size_t sz, ImGuiCond cond)
{
	return ImGui::SetDragDropPayload(type,data,sz,cond);
}

void FUI::TableHeadersRow()
{
	ImGui::TableHeadersRow();
}

void FUI::EndDragDropSource()
{
	ImGui::EndDragDropSource();
}

void FUI::EndDragDropTarget()
{
	ImGui::EndDragDropTarget();
}

void FUI::TableNextRow()
{
	ImGui::TableNextRow();
}

bool FUI::IsMouseDoubleClicked(ImGuiMouseButton button)
{
	return ImGui::IsMouseDoubleClicked(button);
}

bool FUI::TableSetColumnIndex(int column_n)
{
	return ImGui::TableSetColumnIndex(column_n);
}

bool FUI::IsItemClicked(ImGuiMouseButton button)
{
	return ImGui::IsItemClicked(button);
}

void FUI::TextColored(const ImVec4& color, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	ImGui::TextColoredV(color, fmt, args);
	va_end(args);
}

bool FUI::ArrowButton(const char* str_id, ImGuiDir dir)
{
	return ImGui::ArrowButton(str_id,dir);
};

bool FUI::IsItemHovered()
{
	return ImGui::IsItemHovered();
}

bool FUI::IsMouseClicked(ImGuiMouseButton button, bool repeat)
{
	return ImGui::IsMouseClicked(button, repeat);
};

void FUI::Columns(int count, const char* id, bool border)
{
	ImGui::Columns(count, id, border);
}

ImGuiID FUI::DockSpace(ImGuiID id, const ImVec2& size, ImGuiDockNodeFlags flags, const ImGuiWindowClass* window_class)
{
	return ImGui::DockSpace(id,size,flags,window_class);
}

const ImGuiPayload* FUI::AcceptDragDropPayload(const char* type, ImGuiDragDropFlags flags)
{
	return ImGui::AcceptDragDropPayload(type,flags);
}

ImGuiViewport* FUI::GetMainViewport()
{
	return ImGui::GetMainViewport();
}

ImVec2 FUI::GetContentRegionAvail()
{
	return ImGui::GetContentRegionAvail();
}

ImGuiID FUI::GetID(const char* str_id)
{
	return ImGui::GetID(str_id);
}

void FUI::SetNextWindowPos(const ImVec2& pos, ImGuiCond cond, const ImVec2& pivot)
{
	ImGui::SetNextWindowPos(pos, cond, pivot);
}

void FUI::SetNextWindowSize(const ImVec2& size, ImGuiCond cond)
{
	ImGui::SetNextWindowSize(size, cond);
}

void FUI::SetNextWindowViewport(ImGuiID viewport_id)
{
	ImGui::SetNextWindowViewport(viewport_id);
}

void FUI::PushStyleColor(ImGuiCol idx, const ImVec4& col)
{
	ImGui::PushStyleColor(idx, col);
};
void FUI::PopStyleColor(int cnt)
{
	ImGui::PopStyleColor(cnt);
};

void FUI::PushStyleVar(ImGuiStyleVar idx, float val)
{
	ImGui::PushStyleVar(idx, val);
}

void FUI::PopStyleVar(int cnt)
{
	ImGui::PopStyleVar(cnt);
}

void FUI::SameLine()
{
	ImGui::SameLine();
}

bool FUI::CollapsingHeader(const char* label, ImGuiTreeNodeFlags flags)
{
	return ImGui::CollapsingHeader(label,flags);
}

void FUI::Separator()
{
	ImGui::Separator();
}

bool FUI::TreeNode(const char* label)
{
	return ImGui::TreeNode(label);
}

void FUI::SeparatorEx(ImGuiSeparatorFlags flags)
{
	ImGui::SeparatorEx(flags);
}

void FUI::LabelText(const char* label, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	ImGui::LabelTextV(label, fmt, args);
	va_end(args);
}

void FUI::TreePop()
{
	ImGui::TreePop();
}

ImGuiWindow* FUI::GetCurrentWindow()
{
	return ImGui::GetCurrentWindow();
};

void FUI::DragDropTargetSpace(const char* Category, DragDropTargetCallback Callback, void* UserData)
{
	const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload(Category);
	if (Payload and Callback)
	{
		Callback(Payload, UserData);
	}
};

bool FUI::BeginMainMenuBar()
{
	return ImGui::BeginMainMenuBar();
}

bool FUI::BeginMenu(const char* label, bool enabled)
{
	return ImGui::BeginMenu(label,enabled);
}

bool FUI::MenuItem(const char* label, const char* shortcut, bool* selected, bool enabled)
{
	return ImGui::MenuItem(label,shortcut,selected,enabled);
}

void FUI::EndMenu()
{
	ImGui::EndMenu();
}

void FUI::EndMainMenuBar()
{
	ImGui::EndMainMenuBar();
}

bool FUI::Begin(const char* name, bool* p_open, ImGuiWindowFlags flags)
{
	return ImGui::Begin(name,p_open,flags);
}

void FUI::End()
{
	ImGui::End();
}
