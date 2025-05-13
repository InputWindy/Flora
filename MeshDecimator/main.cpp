#include <CoreMinimal.h>

#include "WhiteList.h"
#include "RoamingPath.h"

#define STR_CAT(Str1,Str2) Str1##Str2

std::string flora::ExcutePath;
using namespace flora;

template<>
std::shared_ptr<flora::IEditor> flora::IEditor::InitEditor<EEditorType::ET_ImGui>(void*)
{
	return nullptr;
};


using namespace glm;

#include "Shaders/ShaderDefines.h"
#include "Shaders/ShaderStructs.h"

struct MeshDecimatorApp : public flora::IApp
{
	DECLARE_SURFACE_SHADER(FSurfaceShader, "Surface.vert", "Surface.frag")
	DECLARE_COMPUTE_SHADER(FComputeShader, "MarkTriangle.comp")

	typedef FStructuredBuffer<TRIANGLE_VISIBILITY_BUFFER_SLOT, FVisibility, EBufferUsage::BU_DYNAMIC_COPY> FTriangleVisBuffer;
	typedef FStructuredBuffer<MESH_BUFFER_SLOT, FMesh, EBufferUsage::BU_STATIC_READ> FMeshBuffer;


	MeshDecimatorApp(int argc, char* argv[])
	{
		WindowDesc.Name = "MeshDecimator";
		WindowDesc.Width = 1024;
		WindowDesc.Height = 1024;
		WindowDesc.bHideWindow = true;

		ExcutePath = std::filesystem::canonical(std::filesystem::current_path() / std::filesystem::path(argv[0])).parent_path().generic_string();
		LogPath = "log.txt";

		ParseCmdline(argc, argv);
	}

	virtual void StartUp()override
	{
		IApp::StartUp();

// //#ifdef DEBUG

// 		XMaterial::ImportShaderHeaderFiles(SHADER_ROOT_DIR);
// 		XMaterial::ImportShaderHeaderFiles(STR_CAT(SHADER_ROOT_DIR, "Core/"));
// //#endif // DEBUG

		XMaterial::ImportShaderHeaderFiles(ExcutePath + "/Shaders/");
		XMaterial::ImportShaderHeaderFiles(ExcutePath + "/Shaders/Core/");
	}

	virtual void Run()override
	{
		// load config
		XRoamingPath RoamingPath; XWhiteList WhiteLists;
		{
			RoamingPath.LoadFromFile(CameraStateFilepath.c_str());
			WhiteLists.LoadFromFile(WhiteListsFilepath.c_str());
		}

		// load model
		std::shared_ptr<flora::XModelMesh> HighModel = nullptr;
		{
			if (auto RH = IResource<>::Open<XImporter>(FbxFilepath, AssimpImportSettings()))
			{
				HighModel = RH->DynamicPointerCast<XModelMesh>();
				HighModel->Register();
			}
		}

		if (!HighModel)
		{
			flora_CORE_ERROR("Failed to load model from:{0}", FbxFilepath.c_str())
			return;
		}

		// decimate
		FScopeShader<FSurfaceShader> SurfaceShader;
		FScopeShader<FComputeShader> ComputeShader;
		if (SurfaceShader.IsValid() && ComputeShader.IsValid())
		{
			// setup camera
			XPerspectiveCamera Camera;
			Camera.SetNearClipPlane(0.1 * 100.f);				//m to cm
			Camera.SetFarClipPlane(DistanceCullThres * 100.f);	//m to cm
			Camera.SetFOV(90);
			Camera.SetAspectRatio(1);

			// collect camera pos
			std::vector<XRoamingPath::CameraState> States = {};
			{
				const std::vector<XRoamingPath::Path>& Paths = bCloud ? RoamingPath.GetAllPath() : RoamingPath.GetRoamingPath();

				flora_CORE_TRACE("Roaming path number:{0}", Paths.size());

				for (size_t ii = 0; ii < Paths.size(); ii++)
				{
					auto& CameraStates = Paths[ii].CameraStates;

					States.insert(States.end(), CameraStates.begin(), CameraStates.end());
				}

				flora_CORE_TRACE("Camera state number:{0}", States.size());
			}
			
			// init mesh resources
			FMeshBuffer						MeshBuffer;	// ssbo
			FTriangleVisBuffer				VisBuffer;	// ssbo
			std::vector<XStaticMeshProxy>	DrawLists;	// vao
			{
				FMesh M;
				M.GlobalTriOffset = 0;
				M.NumTriangles = 0;

				MeshBuffer.Resize(HighModel->GetMeshes().size(), M);

				int NumTriangles = 0;
				for (int i = 0; i < MeshBuffer.GetNum(); ++i)
				{
					auto Mesh = HighModel->GetMeshes()[i];

					MeshBuffer[i].GlobalTriOffset = NumTriangles;
					MeshBuffer[i].NumTriangles = Mesh->FN();

					NumTriangles += Mesh->FN();

					DrawLists.push_back(Mesh);
				}

				FVisibility Vis;
				Vis.Visibility = INVISIBLE;

				VisBuffer.Resize(NumTriangles, Vis);

				MeshBuffer.InitRHI();
				VisBuffer.InitRHI();
			}

			// init render resource
			std::shared_ptr<XRHIFrameBuffer> Backbuffer;
			std::shared_ptr<XRHITexture2D> RenderTarget;
			{
				XRHITextureCreateInfo RenderTargetInfo;
				RenderTargetInfo.TextureType = ETextureTarget::TT_TEXTURE_2D;
				RenderTargetInfo.SizeX = Width;
				RenderTargetInfo.SizeY = Height;
				RenderTargetInfo.InternalFormat = EInternalFormat::IF_RG32I; // ( mesh_id + 1 , primitive_id + 1 )
				RenderTargetInfo.DefaultSamplerInfo.WrapR = EWrapMode::WM_CLAMP_TO_EDGE;
				RenderTargetInfo.DefaultSamplerInfo.WrapS = EWrapMode::WM_CLAMP_TO_EDGE;
				RenderTargetInfo.DefaultSamplerInfo.WrapT = EWrapMode::WM_CLAMP_TO_EDGE;
				RenderTargetInfo.DefaultSamplerInfo.MagFilterMode = EFilterMode::FM_NEAREST;
				RenderTargetInfo.DefaultSamplerInfo.MinFilterMode = EFilterMode::FM_NEAREST;

				RenderTarget = GetRHI()->CreateTexture(RenderTargetInfo)->DynamicPointerCast<XRHITexture2D>();

				XRHIRenderBufferCreateInfo RenderBufferInfo;
				RenderBufferInfo.Width = Width;
				RenderBufferInfo.Height = Height;
				RenderBufferInfo.InternalFormat = EInternalFormat::IF_DEPTH32F_STENCIL8;

				XRHIFrameBufferCreateInfo BackbufferInfo;
				BackbufferInfo.DepthStencilAttachment = GetRHI()->CreateRenderBuffer(RenderBufferInfo);

				Backbuffer = GetRHI()->CreateFrameBuffer(BackbufferInfo);
			}
			
			// init uniform data
			glm::mat4 Model = glm::identity<glm::mat4>();
			glm::mat4 View = glm::identity<glm::mat4>();
			glm::mat4 Proj = glm::identity<glm::mat4>();
			{
				Proj = Camera.ProjectionMatrix();
			}

			// foreach every frame
			for (size_t i = 0; i < States.size(); i++)
			{
				// pass0 : render a frame
				Backbuffer->SetColorAttachment(
					{
						{"RenderTarget",RenderTarget ,0},/*debug name(not use) / texture resource / binding point / cubemap face*/
					});
				Backbuffer->EnableOutputAttachments({ 0 });
				Backbuffer->UpdateRHI();
				Backbuffer->Bind();
				{
					// update MVP
					{
						const XRoamingPath::CameraState& Item = States[i];

						flora::XTransform CameraTransform;
						CameraTransform.SetPosition({ Item.x,Item.y,Item.z })
							.SetRotation({ 0,Item.pitch,Item.yaw });

						View = CameraTransform.ViewMatrix();
					}

					// update material states
					{
						if (bCloud)
							SurfaceShader.GetRenderState().bEnableCull = false;
						else
						{
							SurfaceShader.GetRenderState().bEnableCull = true;
							SurfaceShader.GetRenderState().CullFace = ECullFace::CF_BACK;
						}

						SurfaceShader.SetupRenderState();
						SurfaceShader.GetProgram()->Use();
						SurfaceShader.GetProgram()->SetMatrixFloat4x4Array("u_View", 1, false, glm::value_ptr(View));
						SurfaceShader.GetProgram()->SetMatrixFloat4x4Array("u_Proj", 1, false, glm::value_ptr(Proj));
					}

					GetRHI()->ResizeViewport(0, 0, Width, Height);
					GetRHI()->ClearFrameBuffer(CBB_COLOR_BUFFER_BIT | CBB_DEPTH_BUFFER_BIT | CBB_STENCIL_BUFFER_BIT);

					// draw meshes
					for (size_t Idx = 0; Idx < DrawLists.size(); Idx++)
					{
						XStaticMeshProxy& DrawItem = DrawLists[Idx];

						Model = glm::identity<glm::mat4>() * DrawItem.GetObject()->GetCoordMatrix() * DrawItem.GetObject()->GetTransformToRootMatrix();

						SurfaceShader.GetProgram()->SetMatrixFloat4x4Array("u_Model", 1, false, glm::value_ptr(Model));
						SurfaceShader.GetProgram()->SetInt1("u_MeshID", Idx);

						DrawItem.GetContainer()->DrawElementInstanced(EDrawMode::DM_TRIANGLES, 1);
					}

					GetRHI()->FinishRenderCommand();
				}
				Backbuffer->UnBind();

				//std::vector<int> Pixels;
				//RenderTarget->ReadPixels(Pixels);

				// pass 1 : mark triangle as visible
				ComputeShader.GetProgram()->Use();
				ComputeShader.GetProgram()->SetTexture("u_TriangleID", U_TRI_ID_BINDING, RenderTarget, nullptr);
				ComputeShader.GetProgram()->SetBuffer(MeshBuffer.GetRef());
				ComputeShader.GetProgram()->SetBuffer(VisBuffer.GetRef());

				GetRHI()->DispatchCompute(Width/NUM_LOCAL_THREAD_X, Height / NUM_LOCAL_THREAD_Y, 1);
				GetRHI()->MemoryBarrier(MBB_SHADER_STORAGE_BARRIER_BIT);
				GetRHI()->FinishRenderCommand();

				//VisBuffer.ReadBack();

				PrintProgressBar(i, States.size());
			}

			// collect visible triangles
			std::vector<std::set<uint32_t>> VisibleTriangles(MeshBuffer.GetNum());
			{
				MeshBuffer.ReadBack();
				VisBuffer.ReadBack();

				for (int i = 0; i < MeshBuffer.GetNum(); ++i)
				{
					for (int j = 0; j < MeshBuffer[i].NumTriangles; ++j)
					{
						if (VisBuffer[MeshBuffer[i].GlobalTriOffset + j].Visibility == VISIBLE)
						{
							VisibleTriangles[i].insert(j);
						}
					}
				}
			}

			// regenerate mesh
			for (int i = 0; i < MeshBuffer.GetNum(); ++i)
			{
				std::string Name = DrawLists[i].GetObject()->GetNodeName();
				if (std::find(WhiteLists.WhiteLists.begin(), WhiteLists.WhiteLists.end(), Name) == WhiteLists.WhiteLists.end())
				{
					DrawLists[i].GetContainer()->RegenerateMesh(VisibleTriangles[i]);
					DrawLists[i].UpdateObject();//update static mesh resource
				}
				else
				{
					// skip actor
					flora_CORE_TRACE("White List : {0}", Name);
				}
			}
		}

		flora_CORE_TRACE("Save model :{0}", OutputDir);
		IResource<>::Save<XExporter>(*HighModel, OutputDir.c_str(), AssimpExportSettings());
		flora_CORE_TRACE("Done .");
	}

	private:
	virtual void ParseCmdline(int argc, char* argv[])
	{
		//version : release2.0.2 ( back face culling & distance culling )
		argparse::ArgumentParser program("mesh_decimator", "2.0.2");

		//optional args: 
		program.add_argument("-i", "--input")
			.required()
			.help("specify the input mesh filepath.");

		program.add_argument("-c", "--camera")
			.required()
			.help("specify the input camera_state.json filepath.");

		program.add_argument("-o", "--output")
			.help("specify the output submesh save filepath.")
			.required();

		program.add_argument("-rx", "--resolusion_x")
			.help("specify resolution x.");

		program.add_argument("-ry", "--resolusion_y")
			.help("specify resolution y.");

		program.add_argument("-dt", "--distance_threshold")
			.help("specify distance culling threshold.");

		program.add_argument("-wl", "--whitelist")
			.help("whitelist")
			.default_value(std::string(""))
			.nargs(1);

		program.add_argument("-cloud", "--cloud")
			.help("cloud")
			.default_value(false)
			.implicit_value(true)
			.nargs(0);

		try
		{
			program.parse_args(argc, argv);
		}
		catch (const std::exception& err)
		{
			std::cerr << err.what() << std::endl;
			std::cerr << program;
			std::exit(1);
		}

		if (auto input_mesh_filepath = program.present("-i"))
		{
			FbxFilepath = *input_mesh_filepath;
			std::filesystem::path p(FbxFilepath);
			FbxName = p.stem().generic_string().c_str();
		};

		if (auto camera_state_filepath = program.present("-c"))
		{
			CameraStateFilepath = *camera_state_filepath;
		};

		if (auto output_dir = program.present("-o"))
		{
			OutputDir = *output_dir;
		};

		if (auto res_x = program.present("-rx"))
		{
			Width = stoi(*res_x);
		};

		if (auto res_y = program.present("-ry"))
		{
			Height = stoi(*res_y);
		};

		if (auto distance_threshold = program.present("-dt"))
		{
			DistanceCullThres = stof(*distance_threshold);
		};

		std::string whitelists = program.get<std::string>("-wl");
		bCloud = program.get<bool>("-cloud");

		if (!whitelists.empty())
		{
			WhiteListsFilepath = whitelists;
		}
	};

	std::string FbxFilepath;
	std::string FbxName;
	std::string CameraStateFilepath;
	std::string OutputDir;
	std::string WhiteListsFilepath;

	float DistanceCullThres;
	int Width, Height;

	bool bCloud;
};

#ifdef PY_MODULE_NAME
#include "PyRegister.h"
PY_REGISTER(MeshDecimator,

	.def(py::init<>())

	.def("start_up", &MeshDecimatorApp::StartUp)
	.def("run", &MeshDecimatorApp::Run)
	.def("shutdown", &MeshDecimatorApp::Shutdown)
	, MeshDecimatorApp
)
PYBIND11_MODULE(PY_MODULE_NAME, m) 
{
    m.doc() =
        R"(
            xpydec - flora model decimator python library .    
        )";

    PyRegister::Get().Exec(m);
}
#else
#include <EntryPoint.h>
IMPL_APP(MeshDecimatorApp)
#endif

