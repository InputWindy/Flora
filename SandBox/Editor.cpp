#include "Editor.h"
#include "Flora.h"
#include "OpenGLInterface.h"
#include <filesystem>
#include <direct.h>
#include <functional>
#include <stack>

#include "Texture.h"

#define REVERSE_BIT(x,y)  x^=(1<<y)

void EditorLayer::OnAwake()
{
	/*PUSH_ERROR_MSG("Temp", "Test num :%d %d %f",-200,-100,33.8f);
	PUSH_TRACE_MSG("Temp", "d");
	PUSH_INFO_MSG("Temp", "d");
	PUSH_WARN_MSG("Temp", "d");*/
}

void EditorLayer::OnUpdate()
{
}

void EditorLayer::OnGUI()
{
	MainRegion();
}

bool EditorLayer::MainRegion()
{
	if (FUI::BeginMainMenuBar())
	{
		if (FUI::BeginMenu("File"))
		{
			if (FUI::MenuItem("Save", nullptr,nullptr))
			{
				//FSceneManager::Get().SaveCurrentScene();
				//FResourceManager::Get().SaveMaterial();
			};
			FUI::EndMenu();
		};

		if (FUI::BeginMenu("Edit"))
		{
			FUI::MenuItem("Editor Settings", nullptr, &bOpenEditorSettings);
			FUI::EndMenu();
		};

		if (FUI::BeginMenu("Window"))
		{
			FUI::MenuItem("Console", nullptr, &bOpenConsole);
			FUI::MenuItem("Content Browser", nullptr, &bOpenContentBrowser);
			FUI::Separator();
			FUI::MenuItem("Game Object Viewer", nullptr, &bOpenGameObjectViewer);
			FUI::MenuItem("Game Scene", nullptr, &bOpenGameScene);
			FUI::MenuItem("Hierarchy", nullptr, &bOpenHierarchy);
			FUI::MenuItem("Inspector", nullptr, &bOpenInspector);
			FUI::Separator();
			FUI::MenuItem("LevelViewer", nullptr, &bOpenLevelViewer);
			FUI::MenuItem("ResourceViewer", nullptr, &bOpenResourceViewer);
			FUI::EndMenu();
		};

		if (FUI::BeginMenu("Help"))
		{
			FUI::MenuItem("Document",nullptr, nullptr);
			FUI::EndMenu();
		};

		FUI::EndMainMenuBar();
	};

	const ImGuiViewport* viewport = FUI::GetMainViewport();
	FUI::SetNextWindowPos(viewport->WorkPos);
	FUI::SetNextWindowSize(viewport->WorkSize);
	FUI::SetNextWindowViewport(viewport->ID);

	if (FUI::Begin("FloraEditorMainRegion", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoNavFocus))
	{
		FUI::DockSpace(FUI::GetID("FloraEditorMainRegionDockingSpace"));

		Console();
		ContentBrowser();
		EditorSettings();
		GameScene();
		Hierarchy();
		Inspector();
		LevelViewer();
		ResourceViewer();
		GameObjectViewer();

		FUI::End();
	}
	return true;
}

bool EditorLayer::RenderGizmos()
{
	return false;
}

void EditorLayer::Console()
{
	if (bOpenConsole)
	{
		FUI::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		if (FUI::Begin("Console", &bOpenConsole))
		{
			FConsoleContext& ConsoleContext = FConsoleContext::Get();
			ImVec2 Region = FUI::GetContentRegionAvail();

			static ImVec4 ErrorColor = { 1,0,0,1 };
			static ImVec4 WarnColor = { 1,1,0,1 };
			static ImVec4 TraceColor = { 1,1,1,1 };
			static ImVec4 InfoColor = { 0,1,0,1 };

			static uint8_t DebugType = EMT_None;

			FUI::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
			FUI::BeginGroup();
			FUI::ColorEdit4("##Error Color", &ErrorColor.x, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreviewHalf); FUI::SameLine(); if (FUI::Button((std::string("Error (") + std::to_string(ConsoleContext.ErrorNum) + ')').c_str())) REVERSE_BIT(DebugType, 0);
			FUI::EndGroup();
			FUI::SameLine(); FUI::SeparatorEx(ImGuiSeparatorFlags_Vertical); FUI::SameLine();
			FUI::BeginGroup();
			FUI::ColorEdit4("##Warn Color", &WarnColor.x, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreviewHalf); FUI::SameLine(); if (FUI::Button((std::string("Warn (") + std::to_string(ConsoleContext.WarnNum) + ')').c_str())) REVERSE_BIT(DebugType, 1);
			FUI::EndGroup();
			FUI::SameLine(); FUI::SeparatorEx(ImGuiSeparatorFlags_Vertical); FUI::SameLine();
			FUI::BeginGroup();
			FUI::ColorEdit4("##Trace Color", &TraceColor.x, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreviewHalf); FUI::SameLine(); if (FUI::Button((std::string("Trace (") + std::to_string(ConsoleContext.TraceNum) + ')').c_str())) REVERSE_BIT(DebugType, 2);
			FUI::EndGroup();
			FUI::SameLine(); FUI::SeparatorEx(ImGuiSeparatorFlags_Vertical); FUI::SameLine();
			FUI::BeginGroup();
			FUI::ColorEdit4("##Info Color", &InfoColor.x, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaPreviewHalf); FUI::SameLine(); if (FUI::Button((std::string("Info (") + std::to_string(ConsoleContext.InfoNum) + ')').c_str())) REVERSE_BIT(DebugType, 3);
			FUI::EndGroup();
			FUI::SameLine(); FUI::SeparatorEx(ImGuiSeparatorFlags_Vertical); FUI::SameLine();
			FUI::BeginGroup();
			if (FUI::Button("Clear")) ConsoleContext.Clear();
			FUI::EndGroup();
			FUI::PopStyleVar();

			FUI::Separator();

			Region.y -= 55;
			if (FUI::BeginTable("Debug Output", 6, ImGuiTableFlags_NoBordersInBodyUntilResize | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg, Region))
			{
				FUI::TableSetupColumn("Category", ImGuiTableFlags_SizingFixedFit);
				FUI::TableSetupColumn("Comment", ImGuiTableFlags_SizingFixedFit);
				FUI::TableSetupColumn("File", ImGuiTableFlags_SizingFixedFit);
				FUI::TableSetupColumn("Function", ImGuiTableFlags_SizingFixedFit);
				FUI::TableSetupColumn("Line", ImGuiTableFlags_SizingFixedFit);

				FUI::TableHeadersRow();

				for (uint8_t Idx = 0; Idx < ConsoleContext.MsgBuffer.size(); Idx++)
				{
					auto& Msg = ConsoleContext.MsgBuffer[Idx];

					if ((DebugType & 0x01) && (Msg.type & 0x01));
					bool bDisplay = false;
					ImVec4 TextColor = { 0,0,0,1 };
					switch (Msg.type)
					{
					case EMT_Warn: {TextColor = { WarnColor }; bDisplay = (DebugType & 0x02); break; };
					case EMT_Error: {TextColor = { ErrorColor }; bDisplay = (DebugType & 0x01); break; };
					case EMT_Info: {TextColor = { InfoColor }; bDisplay = (DebugType & 0x08); break; };
					case EMT_Trace: {TextColor = { TraceColor }; bDisplay = (DebugType & 0x04); break; };
					default:
						break;
					}
					if (bDisplay)
					{
						FUI::TableNextRow();
						FUI::TableSetColumnIndex(0); FUI::TextColored(TextColor, Msg.category);
						FUI::TableSetColumnIndex(1); FUI::TextColored(TextColor, Msg.comment.c_str());
						FUI::TableSetColumnIndex(2); FUI::TextColored(TextColor, Msg.file);
						FUI::TableSetColumnIndex(3); FUI::TextColored(TextColor, Msg.function);
						FUI::TableSetColumnIndex(4); FUI::TextColored(TextColor, "%d", Msg.line_num);
					}
				}
			}
			FUI::EndTable();
		}
		FUI::End();
	}
}

void EditorLayer::ContentBrowser()
{
	if (bOpenContentBrowser)
	{
		if (FUI::Begin("Content Browser", &bOpenContentBrowser))
		{
			FContentBrowserContext& Content = FContentBrowserContext::Get();
			ImVec2 Region = FUI::GetContentRegionAvail();

			std::filesystem::path Root(Content.RootPath);
			std::filesystem::path Current(Content.CurrentPath);

			{
				static std::stack<uint32_t> FileStack;

				if (FileStack.size() == 0)FileStack.push(0);

				FUI::BeginGroup();

				if (FileStack.size() > 1)
				{
					if (FUI::ArrowButton("##<-", ImGuiDir_Left))
					{
						FileStack.pop();
					}
					FUI::Separator();
				}

				FUI::Columns((int)(Region.x / 100) < 1 ? 1 : (int)(Region.x / 100), 0, false);
				FUI::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				FUI::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1, 0.1, 0.1, 0.2));
				FUI::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
				static std::filesystem::path payload_path;

				FContentBrowserContext::FTreeNode& CurrentNode = FContentBrowserContext::FTreeNode::GetTreeNodes()[FileStack.top()];
				if (CurrentNode.ChildCount > 0)
				{
					for (uint32_t Idx = 0; Idx < CurrentNode.ChildCount; Idx++)
					{
						FContentBrowserContext::FTreeNode& ChildNode = FContentBrowserContext::FTreeNode::GetTreeNodes()[CurrentNode.ChildIdx + Idx];
						
						uint32_t Handle = ChildNode.ChildCount == -1 ? Content.FileIcon : Content.DirectoryIcon;
						
						FUI::ImageButton((ImTextureID)Handle, { 100 ,100 },ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
						if (FUI::IsItemClicked(ImGuiMouseButton_Left))
						{
							if (ChildNode.ChildCount == -1)
							{
								payload_path = ChildNode.Obj;
							}
						}

						if (FUI::IsMouseDoubleClicked(ImGuiMouseButton_Left) && FUI::IsItemHovered())
						{
							if (ChildNode.ChildCount != -1)
							{
								FileStack.push(CurrentNode.ChildIdx + Idx);
							}
						}
						if (FUI::BeginDragDropSource())
						{
							FUI::SetDragDropPayload("CONTENT_BROWSER_ITEM", &payload_path, sizeof(payload_path));
							FUI::EndDragDropSource();
						}
						string NodeName = Utils::unicode2utf8(ChildNode.NodeName);
						FUI::TextWrapped(NodeName.c_str());
						FUI::NextColumn();
					}
				}

				FUI::PopStyleColor();
				FUI::PopStyleColor();
				FUI::PopStyleColor();
				FUI::EndGroup();
			};

			if (FUI::IsItemHovered() && FUI::IsMouseClicked(ImGuiMouseButton_Right))
				FUI::OpenPopup("Update");

			if (FUI::BeginPopup("Update"))
			{
				if (FUI::SmallButton("Refresh"))
				{
					Content.Update();
					FUI::CloseCurrentPopup();
				}
				FUI::EndPopup();
			}
		}
		FUI::End();
	}
}

void EditorLayer::EditorSettings()
{
	if (bOpenEditorSettings)
	{
		FUI::ShowDemoWindow(&bOpenEditorSettings);
	}
}
#include <iostream>
void PayloadFromContentBrowser(const ImGuiPayload* payload, void* userdata)
{
	FContentBrowserContext& Content = FContentBrowserContext::Get();
	FResourceManager& ResourceManager = FResourceManager::Get();

	std::filesystem::path& path = *((std::filesystem::path*)payload->Data);

	//load map

	//load model & skeleton & animation


	//load audio

	//load material
	Ref<FMaterial> Mat = ResourceManager.FindObject<FMaterial>(path.stem().generic_string());
	if (!Mat)
	{
		string filepath = path.generic_string().c_str();
		//filepath = filepath.substr(Content.RootPath.size(), filepath.size() - 1);
		Mat = FApplication::GetRHI()->GenerateMaterial();
		Mat->LoadFromFile(filepath);
		//Tex->SaveToFile(Content.RootPath + "/Cache/Texture/" + Tex->GetName() + ".ftexture");
		Mat->Register();
	}

	////load test texture
	//Ref<FTexture> Tex = ResourceManager.FindObject<FTexture>(path.stem().generic_string());
	//if (!Tex)
	//{
	//	string filepath = path.generic_string().c_str();
	//	//filepath = filepath.substr(Content.RootPath.size(), filepath.size() - 1);
	//	Tex = FApplication::GetRHI()->GenerateTexture();
	//	Tex->LoadFromFile(filepath);
	//	//Tex->SaveToFile(Content.RootPath + "/Cache/Texture/" + Tex->GetName() + ".ftexture");
	//	Tex->Register();
	//}

	//if (Tex)	*(uint32_t*)userdata = Tex->GetHandle();
	
}

void PayloadFromModelResource(const ImGuiPayload* payload, void* userdata)
{
	//add model to map

}

void PayloadFromGameObject(const ImGuiPayload* payload, void* userdata)
{
	//add model to map

}

void EditorLayer::GameScene()
{
	if (bOpenGameScene)
	{
		if (FUI::Begin("Game Scene", &bOpenGameScene))
		{
			static uint32_t Texture = 0;

			FGameScene EditorGameScene(FUI::GetCurrentWindow(), Texture);

			{
				FUI::Image((ImTextureID)EditorGameScene.GetBackBuffer(), FUI::GetContentRegionAvail());
				RenderGizmos();
			}
			
			if (FUI::BeginDragDropTarget())
			{
				FUI::DragDropTargetSpace("CONTENT_BROWSER_ITEM", PayloadFromContentBrowser,&Texture);
				FUI::DragDropTargetSpace("MODEL_ITEM", PayloadFromModelResource);
				FUI::DragDropTargetSpace("GAME_OBJECT_VIEWER_ITEM", PayloadFromGameObject);
				FUI::EndDragDropTarget();
			}
		}
		FUI::End();
	}
}

void EditorLayer::Hierarchy()
{
}

void EditorLayer::Inspector()
{
}

void EditorLayer::LevelViewer()
{
}

void EditorLayer::ResourceViewer()
{
	if (bOpenResourceViewer)
	{
		if (FUI::Begin("Resource Viewer", &bOpenResourceViewer))
		{
			FResourceManager& Manager = FResourceManager::Get();
			const auto& Textures = Manager.GetTextures();
			const auto& Materials = Manager.GetMaterials();
			const auto& Animations = Manager.GetAnimations();
			if (FUI::CollapsingHeader("Texture"))
			{
				FUI::BeginGroup();
				for (auto& Texture : Textures)
				{
					if (FUI::TreeNode(Texture.second->GetName().c_str()))
					{
						FRenderCommand* Rhi = FApplication::GetRHI();
						FUI::LabelText("Texture Type", Rhi->TextureTargetToString(Texture.second->GetTextureType()));
						uint32_t ShortCut = Texture.second->GetShortCut();
						FUI::ImageButton((ImTextureID)ShortCut, { 100 ,100 });
						if (FUI::BeginDragDropSource())
						{
							FUI::SetDragDropPayload("TEXTURE_ITEM",
								Texture.second->GetName().c_str(), Texture.second->GetName().size() + 1);
							FUI::EndDragDropSource();
						}
						FUI::LabelText("Width", std::to_string(Texture.second->GetWidth()).c_str());
						FUI::LabelText("Height", std::to_string(Texture.second->GetHeight()).c_str());
						FUI::LabelText("Depth", std::to_string(Texture.second->GetDepth()).c_str());

						FUI::LabelText("Min Mip Level", std::to_string(Texture.second->GetTextureInfo().MinMipLevel).c_str());
						FUI::LabelText("Max Mip Level", std::to_string(Texture.second->GetTextureInfo().MaxMipLevel).c_str());

						FUI::LabelText("Wrap Mode R", Rhi->WrapModeToString(Texture.second->GetTextureInfo().WrapModeR));
						FUI::LabelText("Wrap Mode S", Rhi->WrapModeToString(Texture.second->GetTextureInfo().WrapModeS));
						FUI::LabelText("Wrap Mode T", Rhi->WrapModeToString(Texture.second->GetTextureInfo().WrapModeT));

						FUI::LabelText("Min Filter Mode", Rhi->FilterModeToString(Texture.second->GetTextureInfo().MinFilterMode));
						FUI::LabelText("Mag Filter Mode", Rhi->FilterModeToString(Texture.second->GetTextureInfo().MagFilterMode));

						FUI::LabelText("Internal Format", Rhi->InternalFormatToString(Texture.second->GetInternalFormat()));

						FUI::LabelText("Load From File", Texture.second->IsLoadedFromFile() ? "True" : "False");

						if (Texture.second->GetRelativePath().size())
						{
							FUI::LabelText("File Path", Texture.second->GetRelativePath().c_str());
							FUI::LabelText("Cache Path", Texture.second->GetCachePath().c_str());
						}

						FUI::TreePop();
					}
				}
				FUI::EndGroup();
			}
		
			if (FUI::CollapsingHeader("Material"))
			{
				FUI::BeginGroup();
				for (auto& Material : Materials)
				{
					if (FUI::TreeNode(Material.second->GetName().c_str()))
					{
						FUI::TreePop();
					}
				}
				FUI::EndGroup();
			}

			if (FUI::CollapsingHeader("Animation"))
			{
				FUI::BeginGroup();
				for (auto& Animation : Animations)
				{
					if (FUI::TreeNode(Animation.second->GetName().c_str()))
					{
						FUI::LabelText("Duration", "%f", Animation.second->GetDuration());
						FUI::LabelText("Ticks Per Second", "%f", Animation.second->GetTicksPerSecond());
						auto& Bones = Animation.second->GetBoneTransforms();
						FUI::Separator();
						for (auto& Bone : Bones)
						{
							FUI::LabelText("Bone", Bone.first.c_str());
						}
						FUI::Separator();
						FUI::TreePop();
					}
				}
				FUI::EndGroup();
			}
		}
		FUI::End();
	}
}

void EditorLayer::GameObjectViewer()
{
}