#include <CoreMinimal.h>
//#include "Editor.h"

#define STR_CAT(Str1,Str2) Str1##Str2

std::string flora::ExcutePath;

using namespace flora;
template<>
std::shared_ptr<flora::IEditor> flora::IEditor::InitEditor<EEditorType::ET_ImGui>(void* InWindowHandle)
{
	return nullptr;
};
#include <glad/glad.h>

DECLARE_SURFACE_SHADER(FSurfaceShader, "surface.vert", "surface.frag")
DECLARE_COMPUTE_SHADER(FRegionShader, "region.comp")

struct InputInfo :public ISerialization
{
	virtual bool Parse(const json11::Json& In)
	{
		LengthPerPixel = In["LengthPerPixel"].number_value();
		FbxFilepath = In["FbxFilepath"].string_value();
		SavePath = In["SavePath"].string_value();
		MaskUnreachableArea = In["MaskUnreachableArea"].int_value();
		MaskWallArea = In["MaskWallArea"].int_value();
		MaskReachableArea = In["MaskReachableArea"].int_value();
		return true;
	};

	virtual bool Serialize(json11::Json& Out)
	{
		return true;
	};

	void DebugLog()
	{
			FLORA_CORE_INFO("LengthPerPixel:{0}", LengthPerPixel)
			FLORA_CORE_INFO("FbxFilepath:{0}", FbxFilepath)
			FLORA_CORE_INFO("SavePath:{0}", SavePath)
			FLORA_CORE_INFO("MaskUnreachableArea:{0}", MaskUnreachableArea)
			FLORA_CORE_INFO("MaskWallArea:{0}", MaskWallArea)
			FLORA_CORE_INFO("MaskReachableArea:{0}", MaskReachableArea)
	}

	float LengthPerPixel = 0.0f;
	std::string FbxFilepath;
	std::string SavePath;

	int MaskUnreachableArea = 255u;
	int MaskWallArea = 255u;
	int MaskReachableArea = 0u;
};

struct ResultInfo:public ISerialization
{
public:
	virtual bool Parse(const json11::Json&)
	{
		return true;
	};

	virtual bool Serialize(json11::Json& Out)
	{
		Out = json11::Json::object
		{
			{"bottom_left_x_offset",bottom_left_x_offset},
			{"bottom_left_y_offset",bottom_left_y_offset},

		};

		return true;
	};

	float bottom_left_x_offset = 0.0f;
	float bottom_left_y_offset = 0.0f;

};

struct OrthoCamApp : public flora::IApp
{
	OrthoCamApp()
	{
		WindowDesc.Name = "OrthoCam";
		WindowDesc.Width = 1024;
		WindowDesc.Height = 1024;
		WindowDesc.bHideWindow = true;

		ExcutePath = std::filesystem::canonical(std::filesystem::current_path()).generic_string();
		LogPath = "log.txt";

	}

	OrthoCamApp(int argc, char* argv[])
	{
		WindowDesc.Name = "OrthoCam";
		WindowDesc.Width = 1024;
		WindowDesc.Height = 1024;
		WindowDesc.bHideWindow = true;

		ExcutePath = std::filesystem::canonical(std::filesystem::current_path() / std::filesystem::path(argv[0])).parent_path().generic_string();
		LogPath = "log.txt";

		ParseCmdline(argc, argv);
	}

	virtual ~OrthoCamApp()override
	{
		
	}

	virtual void StartUp()override
	{
		IApp::StartUp();
		//XMaterial::ImportShaderHeaderFiles(SHADER_ROOT_DIR);
		//XMaterial::ImportShaderHeaderFiles(STR_CAT(SHADER_ROOT_DIR, "Core/"));

		XMaterial::ImportShaderHeaderFiles(ExcutePath + "/Shaders/");
		XMaterial::ImportShaderHeaderFiles(ExcutePath + "/Shaders/Core/");

		//TODO: Init Your Application (Import Demo Scene or Other Resources)
		{

		}
	}

	virtual void Shutdown()override
	{
		//TODO: Clear Your Resources
		{

		}
		IApp::Shutdown();
	}

	//Main Loop
	virtual void Run()override
	{
		InputInfo Info; Info.LoadFromFile(ConfigPath);

		//TODO:Do Something Before Run
		AssimpImportSettings Settings;
		Settings.bOptimizeMeshes;
		Settings.bPreTransformVertices;

		if (auto RH = IResource<>::Open<XImporter>(Info.FbxFilepath, Settings))
		{
			auto Model = RH->DynamicPointerCast<XModelMesh>();
			Model->Register();

			for (auto M : Model->GetMeshes())
			{
				M->UpdateBouding();

				Bounding.Add(M->bbox);
				DrawLists.emplace_back(std::move(M));
			}
		}

		// init render resource
		{
			auto bbox = Bounding;

			int SizeX = glm::ceil(bbox.DimX() / Info.LengthPerPixel);
			int SizeY = glm::ceil(bbox.DimY() / Info.LengthPerPixel);

			XRHIRenderBufferCreateInfo RBInfo;
			RBInfo.Width = SizeX;
			RBInfo.Height = SizeY;
			RBInfo.InternalFormat = EInternalFormat::IF_DEPTH32F_STENCIL8;

			std::shared_ptr<XRHIRenderBuffer> RB = GetRHI()->CreateRenderBuffer(RBInfo);

			XRHITextureCreateInfo Color0Info;
			Color0Info.SizeX = SizeX;
			Color0Info.SizeY = SizeY;
			Color0Info.TextureType = ETextureTarget::TT_TEXTURE_2D;
			Color0Info.InternalFormat = EInternalFormat::IF_RGBA8UI;
			Color0Info.DefaultSamplerInfo.WrapR = EWrapMode::WM_CLAMP_TO_BORDER;
			Color0Info.DefaultSamplerInfo.WrapS = EWrapMode::WM_CLAMP_TO_BORDER;
			Color0Info.DefaultSamplerInfo.WrapT = EWrapMode::WM_CLAMP_TO_BORDER;
			Color0Info.DefaultSamplerInfo.BorderColor = { 0,0,0,0 };
			Color0Info.DefaultSamplerInfo.MagFilterMode = EFilterMode::FM_NEAREST;
			Color0Info.DefaultSamplerInfo.MinFilterMode = EFilterMode::FM_NEAREST;

			Color0 = GetRHI()->CreateTexture(Color0Info);

			XRHIFrameBufferCreateInfo FBInfo;
			FBInfo.DepthStencilAttachment = RB;

			FrameBuffer = GetRHI()->CreateFrameBuffer(FBInfo);
		}

		FScopeShader<FSurfaceShader> SurfaceShader;

		// render
		if (SurfaceShader.IsValid())
		{
			Info.DebugLog();


			auto bbox = Bounding;

			XOrthographicCamera Camera;
			Camera.SetNearClipPlane(0.1f);
			Camera.SetFarClipPlane(bbox.DimZ());
			Camera.SetLeftClipPlane(-bbox.DimX() / 2 - 1);
			Camera.SetRightClipPlane(bbox.DimX() / 2 + 1);
			Camera.SetTopClipPlane(bbox.DimY() / 2 + 1);
			Camera.SetBottonClipPlane(-bbox.DimY() / 2 - 1);
			Camera.Update();

			

			//FLORA_CORE_INFO("per pixel length: ({0},{1})", bbox.DimX() / ResX, bbox.DimY() / ResY)

			ResultInfo Result;
			Result.bottom_left_x_offset = bbox.min.X();
			Result.bottom_left_y_offset = bbox.min.Y();
			Result.SaveToFile(Info.SavePath + "/Result.json");
			
			XTransform CameraTrans;
			CameraTrans.SetPosition({ bbox.min.X() + bbox.DimX() / 2, bbox.min.Y() + bbox.DimY() / 2, bbox.max.Z() })
				.SetRotation({ 0,90,-90 });

			glm::mat4 Model = glm::identity<glm::mat4>();
			glm::mat4 View = CameraTrans.ViewMatrix();
			glm::mat4 Proj = Camera.ProjectionMatrix();

			FrameBuffer->SetColorAttachment({
				{"Color0",Color0,0}
				});
			FrameBuffer->EnableOutputAttachments({ 0 });
			FrameBuffer->UpdateRHI();
			FrameBuffer->Bind();
			{
				
				GetRHI()->ClearFrameBuffer(CBB_COLOR_BUFFER_BIT | CBB_DEPTH_BUFFER_BIT | CBB_STENCIL_BUFFER_BIT);
				GetRHI()->ResizeViewport(0, 0, FrameBuffer->GetDepthStencilAttachment()->GetSizeX(), FrameBuffer->GetDepthStencilAttachment()->GetSizeY());

				SurfaceShader.GetRenderState().bEnableCull = true;
				SurfaceShader.GetRenderState().bEnableDepth = true;
				SurfaceShader.GetRenderState().CullFace = ECullFace::CF_BACK;
				
				auto Program = SurfaceShader.GetProgram();
				Program->Use();
				Program->SetMatrixFloat4x4Array("Model", 1, false, glm::value_ptr(Model));
				Program->SetMatrixFloat4x4Array("View", 1, false, glm::value_ptr(View));
				Program->SetMatrixFloat4x4Array("Proj", 1, false, glm::value_ptr(Proj));
				
				GetRHI()->SetPolygonMode(EPolygonMode::PM_LINE);
				glLineWidth(1.0f);

				for (int i = 0; i < DrawLists.size(); ++i)
				{
					DrawLists[i].GetContainer()->DrawElementInstanced(EDrawMode::DM_TRIANGLES, 1);
				}
				
				GetRHI()->SetPolygonMode(EPolygonMode::PM_FILL);
				//glLineWidth(5.0f);

				for (int i = 0; i < DrawLists.size(); ++i)
				{
					DrawLists[i].GetContainer()->DrawElementInstanced(EDrawMode::DM_TRIANGLES, 1);
				}
				
				GetRHI()->FinishRenderCommand();

			}
			FrameBuffer->UnBind();
			

			// 读回像素
			{
				std::vector<uint8_t> Pixels; int X, Y, CompN;
				if (Color0->DynamicPointerCast<XRHITexture2D>()->ReadPixels(Pixels, X, Y, CompN))
				{
					FLORA_CORE_TRACE("ReadPixels");

					auto GetPixelData = [&Pixels, X, Y, CompN](int x, int y)->uint8_t&
						{
							return Pixels[(y * X + x) * CompN];
						};

					std::queue<std::pair<int, int>> Q,QOW,QI,QIW;

					for (int i = 0; i < X; ++i) { Q.push({ i,0 }); Q.push({ i,Y - 1 }); }
					for (int i = 1; i < Y - 1; ++i) { Q.push({ 0,i }); Q.push({ X - 1,i }); }

					// 外部区域填(0,1,0)，外环像素标(1,0,1)
					while (!Q.empty())
					{
						auto Front = Q.front(); Q.pop();
						uint8_t* pPixel = &GetPixelData(Front.first, Front.second);

						if (pPixel[1] == 255u)continue;
						if (pPixel[0] == 255u)
						{
							QOW.push(Front);
							//if (QOW.size() == 0)
								
							continue;
						}// wall

						pPixel[0] = 0u;
						pPixel[1] = 255u;
						pPixel[2] = 0u;
						pPixel[3] = 255u;

						if (Front.first > 0)Q.push({ Front.first - 1,Front.second });
						if (Front.first < X - 1)Q.push({ Front.first + 1,Front.second });
						if (Front.second > 0)Q.push({ Front.first,Front.second - 1 });
						if (Front.second < Y - 1)Q.push({ Front.first,Front.second + 1 });
					}

					// 外环像素标(1,0,1)
					while (!QOW.empty())
					{
						auto Front = QOW.front(); QOW.pop();
						uint8_t* pPixel = &GetPixelData(Front.first, Front.second);

						// 从上下左右4个像素找黑色和红色，红色是外环墙，黑色是可行进区域.把外环墙填(1,0,1)
						if (Front.first > 0)
						{
							uint8_t* pPixelW = &GetPixelData(Front.first - 1, Front.second);
							if (pPixelW[0] == 0u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								QI.push({ Front.first - 1,Front.second });
							}
							else if (pPixelW[0] == 255u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								pPixelW[2] = 255u;
								QOW.push({ Front.first - 1,Front.second });
							}
								
						}
						if (Front.first < X - 1)
						{
							uint8_t* pPixelW = &GetPixelData(Front.first + 1, Front.second);
							if (pPixelW[0] == 0u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								QI.push({ Front.first + 1,Front.second });
							}
							else if (pPixelW[0] == 255u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								pPixelW[2] = 255u;
								QOW.push({ Front.first + 1,Front.second });
							}
						}
						if (Front.second > 0)
						{
							uint8_t* pPixelW = &GetPixelData(Front.first, Front.second - 1);
							if (pPixelW[0] == 0u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								QI.push({ Front.first, Front.second - 1 });
							}
							else if (pPixelW[0] == 255u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								pPixelW[2] = 255u;
								QOW.push({ Front.first, Front.second - 1 });
							}
						}
						if (Front.second < Y - 1)
						{
							uint8_t* pPixelW = &GetPixelData(Front.first, Front.second + 1);
							if (pPixelW[0] == 0u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								QI.push({ Front.first, Front.second + 1 });
							}
							else if (pPixelW[0] == 255u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								pPixelW[2] = 255u;
								QOW.push({ Front.first, Front.second + 1 });
							}
						}
					}

					// 可行进像素标(0,1,0)
					while (!QI.empty())
					{
						auto Front = QI.front(); QI.pop();
						uint8_t* pPixel = &GetPixelData(Front.first, Front.second);

						// 从上下左右4个像素找黑色和红色，红色是内环墙，黑色是可行进区域.把内环墙填(1,0,1)
						if (Front.first > 0)
						{
							uint8_t* pPixelW = &GetPixelData(Front.first - 1, Front.second);
							if (pPixelW[0] == 0u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								pPixelW[2] = 255u;
								QI.push({ Front.first - 1,Front.second });
							}
							else if (pPixelW[0] == 255u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								QIW.push({ Front.first - 1,Front.second });
							}

						}
						if (Front.first < X - 1)
						{
							uint8_t* pPixelW = &GetPixelData(Front.first + 1, Front.second);
							if (pPixelW[0] == 0u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								pPixelW[2] = 255u;
								QI.push({ Front.first + 1,Front.second });
							}
							else if (pPixelW[0] == 255u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								QIW.push({ Front.first + 1,Front.second });
							}
						}
						if (Front.second > 0)
						{
							uint8_t* pPixelW = &GetPixelData(Front.first, Front.second - 1);
							if (pPixelW[0] == 0u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								pPixelW[2] = 255u;
								QI.push({ Front.first, Front.second - 1 });
							}
							else if (pPixelW[0] == 255u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								QIW.push({ Front.first, Front.second - 1 });
							}
						}
						if (Front.second < Y - 1)
						{
							uint8_t* pPixelW = &GetPixelData(Front.first, Front.second + 1);
							if (pPixelW[0] == 0u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								pPixelW[2] = 255u;
								QI.push({ Front.first, Front.second + 1 });
							}
							else if (pPixelW[0] == 255u && pPixelW[1] == 0u && pPixelW[2] == 0u)
							{
								QIW.push({ Front.first, Front.second + 1 });
							}
						}
					}
					for (int i = 0; i < X; ++i)
					{
						for (int j = 0; j < Y; ++j)
						{
							uint8_t* pPixel = &GetPixelData(i, j);

							/*
								绿色、黑色不可行进区域
							*/
							if ((pPixel[0] == 0u && pPixel[1] == 255u && pPixel[2] == 0u) || (pPixel[0] == 0u && pPixel[1] == 0u && pPixel[2] == 0u))
							{
								pPixel[0] = Info.MaskUnreachableArea;
								pPixel[1] = Info.MaskUnreachableArea;
								pPixel[2] = Info.MaskUnreachableArea;
								pPixel[3] = 255u;
							}
							/*
								蓝色可行进区域
							*/
							else if (pPixel[0] == 0u && pPixel[1] == 0u && pPixel[2] == 255u)
							{
								pPixel[0] = Info.MaskReachableArea;
								pPixel[1] = Info.MaskReachableArea;
								pPixel[2] = Info.MaskReachableArea;
								pPixel[3] = 255u;
							}
							/*
								红色粉色墙
							*/
							else if ((pPixel[0] == 255u && pPixel[1] == 0u && pPixel[2] == 0u) || (pPixel[0] == 255u && pPixel[1] == 0u && pPixel[2] == 255u))
							{
								pPixel[0] = Info.MaskWallArea;
								pPixel[1] = Info.MaskWallArea;
								pPixel[2] = Info.MaskWallArea;
								pPixel[3] = 255u;
							}
							else
							{
								throw std::runtime_error("Unknown Pixel Color .");
							}
						}
					}
					

					if (Color0->DynamicPointerCast<XRHITexture2D>()->SetPixels(Pixels))
					{
						FLORA_CORE_TRACE("SetPixels");
					};

				}
			}
			



//			while (BeginFrame())
//			{
//				//UpdateCamera();
//				//UpdateGameScene();
//				//RenderScene();
//
//#ifdef WITH_GUI
//				Editor->BeginRender();
//				Editor->Render();
//				if (ImGui::Begin("Color0"))
//				{
//					ImGui::Image((ImTextureID)(Color0->GetHandle()), ImGui::GetContentRegionAvail());
//
//					ImGui::End();
//				}
//				Editor->EndRender();
//#endif
//				EndFrame();
//			}

			Texture2DExportSettings ExportSettings;
			ExportSettings.bIsFlipVerticallyOnLoad = false;

			IResource<>::Save<XExporter>(*(GetRHI()->CreateStaticTexture(Color0)->DynamicPointerCast<XTexture2D>()), Info.SavePath + "/output.jpg", ExportSettings);
		}
		else
		{
			FLORA_CORE_ERROR("Invalid Surface Shader . ")
		}

		FLORA_CORE_INFO("Done .")

		//TODO:Do Something After Run
	}

	private:
	virtual void ParseCmdline(int argc, char* argv[])
	{
		argparse::ArgumentParser program("ortho_cam", "1.0.0");

		//optional args: 
		program.add_argument("-config_path")
			.required()
			.help("input config filepath.support - .json");

		program.parse_args(argc, argv);

		ConfigPath = program.get<std::string>("-config_path");
	};

	private:
	std::string ConfigPath;

	private:
	std::vector<XStaticMeshProxy> DrawLists;
	vcg::Box3f Bounding;

	std::shared_ptr<XRHIFrameBuffer> FrameBuffer;
	std::shared_ptr<XRHITexture>     Color0;
};

#ifdef PY_MODULE_NAME
#include "PyRegister.h"
PY_REGISTER(OrthoCam,

	.def(py::init<>())

	.def("start_up", &OrthoCamApp::StartUp)
	.def("run", &OrthoCamApp::Run)
	.def("shutdown", &OrthoCamApp::Shutdown)
	, OrthoCamApp
)
PYBIND11_MODULE(PY_MODULE_NAME, m) 
{
    m.doc() =
        R"(
            xpydec - xverse model decimator python library .    
        )";

    PyRegister::Get().Exec(m);
}
#else
#include <EntryPoint.h>
IMPL_APP(OrthoCamApp)
#endif

