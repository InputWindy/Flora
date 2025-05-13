#include <CoreMinimal.h>

#include "actorstat.h"
#include "roaming.h"
#include "exportrts.h"

using namespace flora;

template<>
std::shared_ptr<flora::IEditor> flora::IEditor::InitEditor<EEditorType::ET_ImGui>(void* InWindowHandle)
{
	return nullptr;
};


#define STR_CAT(Str1,Str2) Str1##Str2

std::string flora::ExcutePath;

using namespace flora;

void UpdateNCCLoss(std::shared_ptr<XRHIFrameBuffer> FrameBuffer, XPerspectiveCamera& Camera, const std::string& roamingfile, const std::string& actorstatfile, const std::string& exportrtsfile, const std::string& InputFolder, const std::string& outputFolder, bool bSaveDebugImg, float NccLoss);


struct SimEvaluationApp : public flora::IApp
{
	SimEvaluationApp(int argc, char* argv[])
	{
		bUseGPU = true;

		WindowDesc.Name = "SimEvaluation";
		WindowDesc.Width = 1024;
		WindowDesc.Height = 1024;
		WindowDesc.bHideWindow = true;

		ExcutePath = std::filesystem::canonical(std::filesystem::current_path() / std::filesystem::path(argv[0])).parent_path().generic_string();
		LogPath = "log.txt";

		ParseCmdline(argc, argv);
	}

	virtual ~SimEvaluationApp()override
	{
		
	}

	virtual void StartUp()override
	{
		IApp::StartUp();
		//XMaterial::ImportShaderHeaderFiles(SHADER_ROOT_DIR);
		//XMaterial::ImportShaderHeaderFiles(STR_CAT(SHADER_ROOT_DIR, "Core/"));

		XMaterial::ImportShaderHeaderFiles(ExcutePath + "/Shaders/");
		XMaterial::ImportShaderHeaderFiles(ExcutePath + "/Shaders/Core/");

		{
			XRHIRenderBufferCreateInfo RenderBufferInfo;
			RenderBufferInfo.Height = resy;
			RenderBufferInfo.Width = resx;
			RenderBufferInfo.InternalFormat = EInternalFormat::IF_DEPTH32F_STENCIL8;

			XRHIFrameBufferCreateInfo FrameBufferInfo;
			FrameBufferInfo.DepthStencilAttachment = GetRHI()->CreateRenderBuffer(RenderBufferInfo);

			FrameBuffer = GetRHI()->CreateFrameBuffer(FrameBufferInfo);
		}
	}

	//Main Loop
	virtual void Run()override
	{
		UpdateNCCLoss(FrameBuffer, Camera, roamingfile, actorstatfile, exportrtsfile, inputfolder, outputfolder, false, NccLoss);
	}

private:
	virtual void ParseCmdline(int argc, char* argv[])
	{
		argparse::ArgumentParser program("mesh profiling", "2.0.0");

		//optional args: 
		program.add_argument("-roamingfile")
			.required();

		/*program.add_argument("-actorstatfile")
			.required();*/

		program.add_argument("-exportrtsfile")
			.required();

		program.add_argument("-inputfolder")
			.required();

		program.add_argument("-outputfolder")
			.required();

		program.add_argument("-resx")
			.required();

		program.add_argument("-resy")
			.required();

		program.add_argument("-nccloss")
			.required();

		/*program.add_argument("-debug")
			.default_value(true)
			.implicit_value(false)
			.help("export lod .")
			.nargs(0);*/

		program.parse_args(argc, argv);

		roamingfile = program.get<std::string>("-roamingfile");
		//actorstatfile = program.get<std::string>("-actorstatfile");
		exportrtsfile = program.get<std::string>("-exportrtsfile");
		inputfolder = program.get<std::string>("-inputfolder");
		outputfolder = program.get<std::string>("-outputfolder");

		resx = std::stoi(program.get<std::string>("-resx"));
		resy = std::stoi(program.get<std::string>("-resy"));
		NccLoss = std::stof(program.get<std::string>("-nccloss"));

		//bSaveDebugImg = program.get<bool>("-debug");
	};

private:
	int resx = 1920;
	int resy = 1080;

	std::string roamingfile;
	std::string actorstatfile;
	std::string exportrtsfile;
	std::string inputfolder;
	std::string outputfolder;

	float NccLoss = 0.95;

	//bool bSaveDebugImg = true;
private:
	XPerspectiveCamera Camera;

	std::shared_ptr<XRHIFrameBuffer> FrameBuffer;
};

#ifdef PY_MODULE_NAME
#include "PyRegister.h"
PY_REGISTER(SimEvaluation,

	.def(py::init<>())

	.def("start_up", &SimEvaluationApp::StartUp)
	.def("run", &SimEvaluationApp::Run)
	.def("shutdown", &SimEvaluationApp::Shutdown)
	, SimEvaluationApp
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
IMPL_APP(SimEvaluationApp)
#endif

using namespace flora;
using namespace glm;

#include "Shaders/ShaderDefines.h"
#include "Shaders/ShaderStructs.h"

DECLARE_COMPUTE_SHADER(FZeroAverageShader, "zero_average.comp")

void ZeroAverage(std::shared_ptr<XRHITexture2D> Tex)
{
	if (!Tex)return;

	FScopeShader<FZeroAverageShader> ZeroAverageShader;

	if (!ZeroAverageShader.IsValid())
	{
		flora_CORE_ERROR("Invalid ZeroAverage Shader .")
			throw std::runtime_error("Invalid ZeroAverage Shaders .");
	}

	int NumBlockX = Tex->GetSizeX() / BLOCK_SIZE_X + 1;
	int NumBlockY = Tex->GetSizeY() / BLOCK_SIZE_Y + 1;

	auto Program = ZeroAverageShader.GetProgram();
	Program->Use();
	Program->SetTextureImage(Tex, 0, 0, EAccessPolicy::AP_READ_WRITE);

	GetRHI()->DispatchCompute(NumBlockX, NumBlockY, 1);
	GetRHI()->FinishRenderCommand();

	//flora_CORE_ERROR("ZeroAverage")
}

DECLARE_COMPUTE_SHADER(FEdgeDetectionShader, "edge_detection.comp")

void EdgeDetection(std::shared_ptr<XRHITexture2D> Tex)
{
	if (!Tex)return;

	FScopeShader<FEdgeDetectionShader> EdgeDetectionShader;

	if (!EdgeDetectionShader.IsValid())
	{
		flora_CORE_ERROR("Invalid EdgeDetectionShader Shader .")
			throw std::runtime_error("Invalid EdgeDetectionShader Shaders .");
	}

	int NumBlockX = Tex->GetSizeX() / BLOCK_SIZE_X + 1;
	int NumBlockY = Tex->GetSizeY() / BLOCK_SIZE_Y + 1;

	auto Program = EdgeDetectionShader.GetProgram();
	Program->Use();
	Program->SetTextureImage(Tex, 0, 0, EAccessPolicy::AP_READ_WRITE);

	GetRHI()->DispatchCompute(NumBlockX, NumBlockY, 1);
	GetRHI()->FinishRenderCommand();

	//flora_CORE_ERROR("EdgeDetectionShader")
}

//DECLARE_COMPUTE_SHADER(FBlockNCCShader, "block_ncc.comp")

bool IsZeroVector(const glm::vec4& vec, float epsilon = 1e-6f)
{
	return (glm::abs(vec.x) < epsilon) && (glm::abs(vec.y) < epsilon) &&
		(glm::abs(vec.z) < epsilon) && (glm::abs(vec.w) < epsilon);
}

std::pair<bool, glm::vec4> BlockNCC(std::shared_ptr<XRHITexture2D> Template, std::shared_ptr<XRHITexture2D> Target)
{
	if (!Template || !Target ||
		Template->GetSizeX() != Target->GetSizeX() ||
		Template->GetSizeY() != Target->GetSizeY())
		return std::pair<bool, glm::vec4>(false, vec4(0, 0, 0, 0));

	std::vector<float> TemplatePixels;
	Template->ReadPixels(TemplatePixels);

	std::vector<float> TargetPixels;
	Target->ReadPixels(TargetPixels);

	auto [Format, Type] = MatchInternalFormat(Template->GetInternalFormat());

	int CompNum = GetRHI()->GetFormatCompNum(Format);

	int NumPixels = TemplatePixels.size() / CompNum;

	vec4 T_Mul_I_Sum = vec4(0, 0, 0, 0);
	vec4 T_2_Sum = vec4(0, 0, 0, 0);
	vec4 I_2_Sum = vec4(0, 0, 0, 0);

	for (int i = 0; i < NumPixels; ++i)
	{
		vec4 T_0 =
		{
			TemplatePixels[i * CompNum] + 0,
			TemplatePixels[i * CompNum] + 1,
			TemplatePixels[i * CompNum] + 2,
			TemplatePixels[i * CompNum] + 3
		};

		vec4 I_0 =
		{
			TargetPixels[i * CompNum] + 0,
			TargetPixels[i * CompNum] + 1,
			TargetPixels[i * CompNum] + 2,
			TargetPixels[i * CompNum] + 3
		};

		T_Mul_I_Sum += I_0 * T_0;
		I_2_Sum += I_0 * I_0;
		T_2_Sum += T_0 * T_0;
	}

	// error situation
	if (IsZeroVector(I_2_Sum * T_2_Sum))return std::pair<bool, glm::vec4>(false, vec4(0, 0, 0, 0));

	return std::pair<bool, glm::vec4>(true, T_Mul_I_Sum / (sqrt(I_2_Sum * T_2_Sum) + vec4(0.0001)));
}


namespace fs = std::filesystem;



#include <regex>

struct FModel
{
	FModel() {}

	FModel(const std::string& path)
	{
		Open(path);
	}

	void Open(const std::string& path)
	{
		ModelRef = nullptr;
		DrawLists.clear();

		if (auto RH = IResource<>::Open<XImporter>(path, AssimpImportSettings()))
		{
			ModelRef = RH->DynamicPointerCast<XModelMesh>();
			ModelRef->Register();

			int NumMeshes = 0;

			NumMeshes = ModelRef->GetMeshes().size();

			for (int i = 0; i < NumMeshes; ++i)
			{
				auto Mesh = ModelRef->GetMeshes()[i];
				Mesh->UpdateBouding();
				Box.Add(Mesh->bbox);

				DrawLists.emplace_back(std::move(Mesh));
			}

			for (auto Node : ModelRef->GetMeshNodes())
			{
				std::string Name = Node.lock()->Name;

				std::regex pattern("_LOD(\\d)");

				std::smatch match;
				std::string lastLOD;

				auto begin = std::sregex_iterator(Name.begin(), Name.end(), pattern);
				auto end = std::sregex_iterator();

				for (auto it = begin; it != end; ++it) { lastLOD = it->str(); }

				if (!lastLOD.empty())
				{
					LODChain.insert({ lastLOD.at(lastLOD.size() - 1) - '0',Node.lock()->NodeMeshes[0] });
					LODBouding.insert({ lastLOD.at(lastLOD.size() - 1) - '0',Node.lock()->NodeMeshes[0]->bbox });
				}
			}

		}
	}

	// only one lod
	glm::vec3 GetCenterWorldSpace(glm::mat4 ModelMatrix)
	{
		auto P = Box.Center();

		mat4 mat = glm::identity<glm::mat4>();

		if (DrawLists.size() > 0 && DrawLists[0].GetObject())
		{
			mat = ModelMatrix * DrawLists[0].GetObject()->GetCoordMatrix() * DrawLists[0].GetObject()->GetTransformToRootMatrix();
		}

		return glm::vec3(mat * glm::vec4(P.X(), P.Y(), P.Z(), 1));
	}

	glm::vec3 GetCenterWorldSpace(int LOD, glm::mat4 ModelMatrix)
	{
		auto P = LODBouding[LOD].Center();

		mat4 mat = glm::identity<glm::mat4>();

		if (DrawLists.size() > 0 && DrawLists[0].GetObject())
		{
			mat = ModelMatrix * DrawLists[0].GetObject()->GetCoordMatrix() * DrawLists[0].GetObject()->GetTransformToRootMatrix();
		}

		return glm::vec3(mat * glm::vec4(P.X(), P.Y(), P.Z(), 1));
	}

public:
	std::shared_ptr<XModelMesh> ModelRef;
	std::vector<XStaticMeshProxy> DrawLists;

	std::map<int, XStaticMeshProxy> LODChain;
	std::map<int, vcg::Box3f> LODBouding;

	vcg::Box3f Box;

};

DECLARE_SURFACE_SHADER(FSurfaceShader, "basepass.vert", "basepass.frag")

void UpdateNCCLoss(std::shared_ptr<XRHIFrameBuffer> FrameBuffer,XPerspectiveCamera& Camera, const std::string& roamingfile, const std::string& actorstatfile, const std::string& exportrtsfile, const std::string& InputFolder, const std::string& outputFolder,bool bSaveDebugImg,float NccLoss)
{
	flora_CORE_ERROR("Output Folder:{0}", outputFolder)

	auto collectFBXFiles = [](const std::string& InputFolder)->std::vector<std::string>
		{
			std::vector<std::string> fbxFiles;

			try {
				// Check if the input folder exists
				if (fs::exists(InputFolder) && fs::is_directory(InputFolder)) {
					// Iterate through the folder
					for (const auto& entry : fs::recursive_directory_iterator(InputFolder)) {
						// Check if the current entry is a regular file and has an ".fbx" extension
						if (fs::is_regular_file(entry) && entry.path().extension() == ".fbx") {
							// Get the relative path (relative to the InputFolder)
							std::string relativePath = fs::relative(entry.path(), InputFolder).string();
							fbxFiles.push_back(relativePath);  // Add the relative file path to the list
						}
					}
				}
				else {
					std::cerr << "The folder does not exist or is not a directory!" << std::endl;
				}
			}
			catch (const fs::filesystem_error& e) {
				std::cerr << "Error accessing the folder: " << e.what() << std::endl;
			}

			return fbxFiles;
		};

	struct NCCCtx
	{
		NCCCtx(int NumLOD, XRHITextureCreateInfo& Info)
		{
			for (int i = 0; i < NumLOD; ++i)
			{
				TemplateNormal.emplace_back(GetRHI()->CreateTexture(Info)->DynamicPointerCast<XRHITexture2D>());
				TargetNormal.emplace_back(GetRHI()->CreateTexture(Info)->DynamicPointerCast<XRHITexture2D>());
			}
		}

		std::vector<std::shared_ptr<XRHITexture2D>> TemplateNormal;
		std::vector<std::shared_ptr<XRHITexture2D>> TargetNormal;
	};

	FScopeShader<FSurfaceShader> SurfaceMaterial;

	if (!SurfaceMaterial.IsValid())
	{
		flora_CORE_ERROR("Invalid Shader .")
			throw std::runtime_error("Invalid Shaders .");
	}

	// ���ػ����ļ�
	XRoamingPath RoamingPath; RoamingPath.LoadFromFile(roamingfile);
	//XActorStats ActorStats; ActorStats.LoadFromFile(actorstatfile);
	XExportRTS ExportRTS; ExportRTS.LoadFromFile(exportrtsfile);

	// ����ColorBuffer
	XRHITextureCreateInfo ColorBufferInfo;
	ColorBufferInfo.TextureType = ETextureTarget::TT_TEXTURE_2D;
	ColorBufferInfo.InternalFormat = EInternalFormat::IF_RGBA32F;
	ColorBufferInfo.DefaultSamplerInfo.MagFilterMode = EFilterMode::FM_NEAREST;
	ColorBufferInfo.DefaultSamplerInfo.MinFilterMode = EFilterMode::FM_NEAREST;
	ColorBufferInfo.DefaultSamplerInfo.BorderColor = { 0,0,0,0 };
	ColorBufferInfo.DefaultSamplerInfo.WrapR = EWrapMode::WM_CLAMP_TO_BORDER;
	ColorBufferInfo.DefaultSamplerInfo.WrapS = EWrapMode::WM_CLAMP_TO_BORDER;
	ColorBufferInfo.DefaultSamplerInfo.WrapT = EWrapMode::WM_CLAMP_TO_BORDER;
	ColorBufferInfo.SizeX = FrameBuffer->GetDepthStencilAttachment()->GetSizeX();
	ColorBufferInfo.SizeY = FrameBuffer->GetDepthStencilAttachment()->GetSizeY();

	// ��Normalͼ
	auto DrawLOD = [](
		int LOD, int FrameId, std::shared_ptr<XRHITexture2D> Tex,
		std::shared_ptr<XRHIFrameBuffer >FrameBuffer,
		FScopeShader<FSurfaceShader>& SurfaceMaterial,
		FModel& LodChain,
		std::vector<XRoamingPath::CameraState> CameraStates,
		XPerspectiveCamera& Camera,
		XTransform ActorTrans
		)
		{
			if (!Tex)
			{
				flora_CORE_ERROR("ok1")
			}

			// Capture LOD 0
			XStaticMeshProxy& DrawItem = LodChain.LODChain[LOD];
			{
				mat4 Model = ActorTrans.ModelMatrix() * DrawItem.GetObject()->GetCoordMatrix() * DrawItem.GetObject()->GetTransformToRootMatrix();
				mat4 View = glm::identity<glm::mat4>();
				mat4 Proj = Camera.ProjectionMatrix();

				{
					auto& Stat = CameraStates[FrameId];

					XTransform Trans;
					Trans.SetPosition({ Stat.x, Stat.y, Stat.z });
					View = Trans.LookAtMatrix(LodChain.GetCenterWorldSpace(LOD, ActorTrans.ModelMatrix()));

					FrameBuffer->SetColorAttachment(
						{
							{"TemplateNormal",Tex,0}
						});
					FrameBuffer->EnableOutputAttachments({ 0 });
					FrameBuffer->UpdateRHI();
					FrameBuffer->Bind();
					{
						FrameBuffer->ClearBuffer(CBB_DEPTH_BUFFER_BIT | CBB_STENCIL_BUFFER_BIT);
						FrameBuffer->ClearColor(0, 0, 0, 0, 1);

						SurfaceMaterial.GetRenderState().bEnableDepth = true;
						SurfaceMaterial.GetRenderState().bEnableCull = true;
						SurfaceMaterial.GetRenderState().CullFace = ECullFace::CF_BACK;
						SurfaceMaterial.SetupRenderState();

						auto Program = SurfaceMaterial.GetProgram();
						Program->Use();
						Program->SetMatrixFloat4x4Array("u_View", 1, false, glm::value_ptr(View));
						Program->SetMatrixFloat4x4Array("u_Proj", 1, false, glm::value_ptr(Proj));
						Program->SetMatrixFloat4x4Array("u_Model", 1, false, glm::value_ptr(Model));

						DrawItem.GetContainer()->DrawElementInstanced(EDrawMode::DM_TRIANGLES, 1);

						GetRHI()->FinishRenderCommand();

					}
					FrameBuffer->UnBind();

					ZeroAverage(Tex);

				}

			}
		};

	auto Models = collectFBXFiles(InputFolder);
	for (auto ModelName : Models)
	{
		int ExportLOD = 7;

		std::string filename;
		{
			// ɾ����׺
			size_t pos = ModelName.rfind(".fbx");
			if (pos != std::string::npos)
			{
				filename = ModelName.substr(0, pos);
			}
		}

		// �õ����StaticMesh��Ӧ��Info
		XExportRTS::MeshInfo& Infos =  ExportRTS.StaticMeshInfos[filename];

		// �����Ѿ�����loss��StaticMesh
		if (Infos.bUpdate)continue;

		// ����LODģ��
		FModel LodChain(InputFolder + "/" + ModelName);

		Infos.bUpdate = true;
		Infos.Loss.resize(LodChain.LODChain.size());

		int MaxCameraStat = 0;

		XTransform Trans = {};//StaticMeshҪ�ŵ�λ��
		std::vector<XRoamingPath::CameraState> CameraStates = {};//���Ҫ�ĵ��ӽ�

		// �ҳ���������ӽ������Ǹ�Actorλ��
		for (auto& Info : Infos.ActorInfos)
		{
			XTransform TempTrans = {};
			TempTrans.SetPosition({ Info.X, Info.Y, Info.Z })
				.SetRotation({ Info.Roll,Info.Pitch,Info.Yaw })
				.SetScale({ Info.ScaleX, Info.ScaleY, Info.ScaleZ });

			std::vector<XRoamingPath::CameraState> TempCameraStates = {};
			TempCameraStates = RoamingPath.ChooseCameraStates(8, 4, LodChain.GetCenterWorldSpace(TempTrans.ModelMatrix()));

			if (TempCameraStates.size() >= MaxCameraStat)
			{
				MaxCameraStat = TempCameraStates.size();
				Trans = TempTrans;
				CameraStates = TempCameraStates;
			}
		}

		//for (auto& Info : Infos.ActorInfos)
		{
			//XTransform Trans;
			//Trans.SetPosition({ Info.X, Info.Y, Info.Z })
			//	.SetRotation({ Info.Roll,Info.Pitch,Info.Yaw })
			//	.SetScale({ Info.ScaleX, Info.ScaleY, Info.ScaleZ });

			////ͨ��Actorλ�ã�ѡȡ���յ�λ
			//std::vector<XRoamingPath::CameraState> CameraStates = RoamingPath.ChooseCameraStates(8, 4, LodChain.GetCenterWorldSpace(Trans.ModelMatrix()));

			// �м�����
			NCCCtx Ctx(CameraStates.size(), ColorBufferInfo);

			// ������������ӽ���LOD 0��Normal��Ϊ������
			for (int FrameId = 0; FrameId < CameraStates.size(); ++FrameId)
			{
				DrawLOD(
					0, FrameId, Ctx.TemplateNormal[FrameId],
					FrameBuffer,
					SurfaceMaterial,
					LodChain,
					CameraStates,
					Camera,
					Trans
				);
			}

			// ����LOD n�ӽǵ�Normal����ΪTarget
			for (int i = LodChain.LODChain.size() - 1; i >=0 ; --i)
			{
				float MinLoss = 1.0f;

				// ���������ӽ���loss��ȡ�������һ����Ϊ��һ��LOD��loss
				for (int FrameId = 0; FrameId < CameraStates.size(); ++FrameId)
				{
					// ��Target Normal
					DrawLOD(
						i, FrameId, Ctx.TargetNormal[FrameId],
						FrameBuffer,
						SurfaceMaterial,
						LodChain,
						CameraStates,
						Camera,
						Trans
					);

					// ����Template��Target��Ncc Loss
					auto [bSuccess, NCCLoss] = BlockNCC(Ctx.TemplateNormal[FrameId], Ctx.TargetNormal[FrameId]);

					// ����ncc loss
					if (bSuccess)
					{
						MinLoss = glm::min(glm::sqrt(glm::dot(NCCLoss, NCCLoss) / 4), MinLoss);
					}
				}

				// ��¼��һ��lod��lossֵ
				Infos.Loss[i] = MinLoss;
			}
		}
	}
	ExportRTS.SaveToFile(exportrtsfile);

}