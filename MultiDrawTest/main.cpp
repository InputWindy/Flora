#include <CoreMinimal.h>
#include <EntryPoint.h>
#include "Editor.h"

using namespace glm;
using namespace flora;

#include "Shaders/ShaderDefines.h"
#include "Shaders/ShaderStructs.h"

#include <glad/glad.h>

struct ProfilingManager :public flora::XSingleton<ProfilingManager>
{
	/*std::vector<std::pair<std::string, double>> TimerMap;
	std::vector<std::pair<std::string, unsigned int>> PrimitiveMap;

	int StaticDrawCallNum = 0;
	int DynamicDrawCallNum = 0;*/

	int VisibleInstanceNum = 0;

	void ShowWindow() 
	{
		if (ImGui::Begin("GPU Timers"))
		{
			for (auto Result : FQueryManager::Get()[EQC_TIME])
			{
				FScopeTimer::TimeResult* ScopeTimeResult = static_cast<FScopeTimer::TimeResult*>(Result.get());
				ImGui::Text("%s: %.3f ms", ScopeTimeResult->Name.c_str(), ScopeTimeResult->ElapsedTime);
			}

			ImGui::Separator();

			/*for (const auto Result : Categories[EQC_PRIMITIVE])
			{
				auto ScopeTimeResult = std::dynamic_pointer_cast<FScopeTimer::TimeResult>(Result);
				ImGui::Text("%s: %.3f ms", ScopeTimeResult->Name.c_str(), ScopeTimeResult->ElapsedTime);
			}

			for (const auto& [name, time] : PrimitiveMap)
			{
				ImGui::Text("%s: %3d ", name.c_str(), time);
			}*/

			ImGui::Separator();
			/*ImGui::Text("%s: %3d ", "Static Draw Call Num", StaticDrawCallNum);
			ImGui::Text("%s: %3d ", "Dynamic Draw Call Num", DynamicDrawCallNum);*/
			ImGui::Text("%s: %3d ", "Visible Instance Num", VisibleInstanceNum);
		};
		ImGui::End();
	}
};

#define SCOPE_TIMER(Name) flora::FScopeTimer Timer(#Name);
//#define SCOPE_PRIMITIVE_COUNTER(Name) flora::ScopeTriangleCount Counter(#Name);
//#define STATIC_DRAW_CALL_NUM(num) ProfilingManager::Get().StaticDrawCallNum = num;
//#define DYNAMIC_DRAW_CALL_NUM(num) ProfilingManager::Get().DynamicDrawCallNum = num;

struct MultiDrawTestApp : public flora::IApp
{
	MultiDrawTestApp(int argc, char* argv[])
	{
		WindowDesc.Name = "MultiDrawTest";
		WindowDesc.Width = 1024;
		WindowDesc.Height = 1024;
		WindowDesc.bHideWindow = false;

		LogPath = "log.txt";

		ParseCmdline(argc, argv);
	}

	virtual ~MultiDrawTestApp()override
	{
		
	}

	virtual void StartUp()override
	{
		IApp::StartUp();
		XMaterial::ImportShaderHeaderFiles(SHADER_ROOT_DIR);
		XMaterial::ImportShaderHeaderFiles(std::string(SHADER_ROOT_DIR) + "Core/");

		InitCanvas();
		ImportMaterial();
		ImportModel();
		InitResource();
	}

	virtual void Shutdown()override
	{
		IApp::Shutdown();
	}

	//Main Loop
	virtual void Run()override
	{
		//TODO:Do Something Before Run

		while (BeginFrame())
		{
			ProcessInput();
			UpdateCamera();

			BeginRender();
			{
				PreRendering();
				InstanceCulling();
				GenDynamicIndexBuffer();
				GenIndirectDrawCmd();
				IndirectDraw();
				FinishRendering();
			}
			EndRender();


		#ifdef WITH_GUI
			Editor->BeginRender();
			Editor->Render();
			if (ImGui::Begin("Preview"))
			{
				ImGui::Image(ImTextureID(RenderTarget->GetHandle()), ImGui::GetContentRegionAvail(), ImVec2{ 0,1 }, ImVec2{ 1,0 });

			}ImGui::End();

			ProfilingManager::Get().ShowWindow();

			Editor->EndRender();
		#endif
			EndFrame();
		}

		//TODO:Do Something After Run
	}

	private:
	virtual void ParseCmdline(int argc, char* argv[])
	{
		
	};

	public:
	//canvas
	std::shared_ptr<XRHIFrameBuffer> Backbuffer;
	std::shared_ptr<XRHITexture2D> RenderTarget;

	void InitCanvas()
	{
		//Backbuffer
		{
			XRHITextureCreateInfo RenderTarget2DInfo;
			RenderTarget2DInfo.SizeX = 1024;
			RenderTarget2DInfo.SizeY = 1024;
			RenderTarget2DInfo.InternalFormat = EInternalFormat::IF_RGBA32F;
			RenderTarget = GetRHI()->CreateTexture(RenderTarget2DInfo)->DynamicPointerCast<XRHITexture2D>();

			XRHIRenderBufferCreateInfo DSBufferInfo;
			DSBufferInfo.Width = 1024;
			DSBufferInfo.Height = 1024;
			DSBufferInfo.InternalFormat = EInternalFormat::IF_DEPTH24_STENCIL8;

			XRHIFrameBufferCreateInfo FrameBufferInfo;
			FrameBufferInfo.DepthStencilAttachment = GetRHI()->CreateRenderBuffer(DSBufferInfo);

			Backbuffer = GetRHI()->CreateFrameBuffer(FrameBufferInfo);
		}
	}

	public:

		void ImportModel()
		{
			std::string FbxFilepath = std::string(SHADER_ROOT_DIR) + "../test.fbx";
			
			//Load high model if exists 
			if (auto RH = IResource<>::Open<XImporter>(FbxFilepath.c_str(), AssimpImportSettings()))
			{
				HighModel = RH->DynamicPointerCast<XModelMesh>();
				HighModel->Register();
			}
		};

		std::shared_ptr<flora::XModelMesh> HighModel;
	public:
		flora::XPerspectiveCamera Camera;
		XTransform CameraTrans;

		void UpdateCamera()
		{
			Camera.SetFarClipPlane(2000 * 100);

			CullingParamList[0].Camera.project_matrix = Camera.ProjectionMatrix();
			memcpy(CullingParamList[0].Camera.view_frustum, Camera.GetFrustum().Planes, sizeof(glm::vec4) * 6);
			CullingParamList[0].camera_pos = vec4(CameraTrans.GetLocalPosition(), 1);
			CullingParamList[0].view_matix = CameraTrans.ViewMatrix();

			CullingParamList.UpdateSubData(0);
		}

		void ProcessInput()
		{
			flora::XTransform& CameraTransform = CameraTrans;
			CameraTransform.SetMovementSpeed(300);
			{
				// camera
				static float lastX = 4096.f / 2.0;
				static float lastY = 4096.f / 2.0;
				static bool firstMouse = true;

				// timing
				static float deltaTime = 0.0f;
				static float lastFrame = 0.0f;

				//game play
				{
					float currentFrame = MainWindow->GetTime();
					deltaTime = currentFrame - lastFrame;
					lastFrame = currentFrame;

					//move forward:W
					//move right:D
					//move back:S
					//move left:A
					//move Up:Q
					//move Down:E
					//press blank and move mouse:rotate
					if (MainWindow->GetKey(X_KEY_W) == X_PRESS)
						CameraTransform.MoveForward(deltaTime);
					if (MainWindow->GetKey(X_KEY_S) == X_PRESS)
						CameraTransform.MoveBackward(deltaTime);
					if (MainWindow->GetKey(X_KEY_A) == X_PRESS)
						CameraTransform.MoveLeft(deltaTime);
					if (MainWindow->GetKey(X_KEY_D) == X_PRESS)
						CameraTransform.MoveRight(deltaTime);
					if (MainWindow->GetKey(X_KEY_Q) == X_PRESS)
						CameraTransform.MoveUp(deltaTime);
					if (MainWindow->GetKey(X_KEY_E) == X_PRESS)
						CameraTransform.MoveDown(deltaTime);

					//rotate
					{
						double xposIn, yposIn;
						MainWindow->GetCursorPos(&xposIn, &yposIn);

						float xpos = static_cast<float>(xposIn);
						float ypos = static_cast<float>(yposIn);
						if (firstMouse)
						{
							lastX = xpos;
							lastY = ypos;
							firstMouse = false;
						}

						float xoffset = xpos - lastX;
						float yoffset = lastY - ypos;

						lastX = xpos;
						lastY = ypos;

						if (MainWindow->GetMouseButton(X_MOUSE_BUTTON_RIGHT) == X_PRESS)
						{
							CameraTransform.Rotate(0, -yoffset, xoffset);
						}
					}
				}
			}
		}
	public:
		void BeginRender()
		{
			Backbuffer->SetColorAttachment(
				{
					{"Color0",RenderTarget ,0},
				});
			Backbuffer->EnableOutputAttachments({ 0 });
			Backbuffer->UpdateRHI();

			Backbuffer->Bind();
			GetRHI()->ResizeViewport(0, 0, Backbuffer->GetDepthStencilAttachment()->GetSizeX(), Backbuffer->GetDepthStencilAttachment()->GetSizeY());
			//GetRHI()->ClearColor(1, 1, 0, 1);
			GetRHI()->ClearFrameBuffer(CBB_COLOR_BUFFER_BIT | CBB_DEPTH_BUFFER_BIT | CBB_STENCIL_BUFFER_BIT);

		};

		void EndRender()
		{
			Backbuffer->UnBind();
		}

		/*
			gpu pipeline
		*/

		//float DistanceFromPoint(const glm::vec4& plane, const glm::vec3& in_point)
		//{
		//	glm::vec3 normalizedNormal = glm::normalize(glm::vec3(plane));

		//	float distance = glm::dot(in_point, normalizedNormal) + plane.w;

		//	return distance;
		//}

		//// view space interaction test
		//bool IsBoxInsideFrustum(vec4 frustum[6], vec3 center, float radius)
		//{
		//	bool in_frustum = true;
		//	for (int i = 0; i < 6; i++)
		//	{
		//		float Dist = DistanceFromPoint(frustum[i], center) + radius;

		//		in_frustum = in_frustum && (Dist > 0.0f);
		//	}

		//	return in_frustum;
		//}

		void InstanceCulling()
		{
			SCOPE_TIMER(InstanceCulling)

			auto Program = InstanceCullingMaterial.GetContainer()->ShaderProgramResource;
			Program->Use();
			Program->SetBuffer(PosList.GetRef());
			Program->SetBuffer(IndexList.GetRef());
			Program->SetBuffer(StaticMeshList.GetRef());
			Program->SetBuffer(SceneInstanceList.GetRef());
			Program->SetBuffer(CullingParamList.GetRef());
			Program->SetBuffer(VisibleInstanceList.GetRef());
			Program->SetBuffer(DynamicIndexList.GetRef());
			Program->SetBuffer(CmdBuffer.GetRef());
			Program->SetBuffer(StatList.GetRef());
			Program->SetBuffer(DebugList0.GetRef());
			Program->SetBuffer(DebugList1.GetRef());

			int GroupSize = CalculateGroupSize(MAX_INSTANCE_NUM);

			GetRHI()->DispatchCompute(GroupSize, GroupSize, 1);
			GetRHI()->MemoryBarrier(MBB_SHADER_STORAGE_BARRIER_BIT);
			GetRHI()->FinishRenderCommand();

			//VisibleInstanceList.ReadBack();

			StatList.ReadBack();

			ProfilingManager::Get().VisibleInstanceNum = StatList[0].VisibleInstanceNum;
			/*DebugList1.ReadBack();

			vec4 frustum[6];
			frustum[0] = DebugList1[0][0];
			frustum[1] = DebugList1[1][0];
			frustum[2] = DebugList1[2][0];
			frustum[3] = DebugList1[3][0];
			frustum[4] = DebugList1[4][0];
			frustum[5] = DebugList1[5][0];

			mat4 vm;
			vm = DebugList1[6];

			vec4 center = DebugList1[7][0];

			bool bIsBoxInsideFrustum = IsBoxInsideFrustum(frustum, vec3(center), center.w * 100);*/

		}

		void GenDynamicIndexBuffer()
		{
			SCOPE_TIMER(GenDynamicIndexBuffer)

			auto Program = GenDynamicIndexBufferMaterial.GetContainer()->ShaderProgramResource;
			Program->Use();
			Program->SetBuffer(PosList.GetRef());
			Program->SetBuffer(IndexList.GetRef());
			Program->SetBuffer(StaticMeshList.GetRef());
			Program->SetBuffer(SceneInstanceList.GetRef());
			Program->SetBuffer(CullingParamList.GetRef());
			Program->SetBuffer(VisibleInstanceList.GetRef());
			Program->SetBuffer(DynamicIndexList.GetRef());
			Program->SetBuffer(CmdBuffer.GetRef());
			Program->SetBuffer(StatList.GetRef());
			Program->SetBuffer(DebugList0.GetRef());
			Program->SetBuffer(DebugList1.GetRef());

			int GroupSize = CalculateGroupSize(MAX_INSTANCE_NUM);

			GetRHI()->DispatchCompute(GroupSize, GroupSize, 1);
			GetRHI()->MemoryBarrier(MBB_SHADER_STORAGE_BARRIER_BIT);
			GetRHI()->FinishRenderCommand();

			/*std::vector<glm::ivec3> DynamicIndexData;
			DynamicIndexList.ReadBack(DynamicIndexData);

			std::vector<glm::ivec3> StaticIndexData;
			IndexList.ReadBack(StaticIndexData);

			VisibleInstanceList.ReadBack();*/
			//DebugList0.ReadBack();
		}

		void GenIndirectDrawCmd()
		{
			SCOPE_TIMER(GenIndirectDrawCmd)

			auto Program = GenIndirectDrawCmdMaterial.GetContainer()->ShaderProgramResource;
			Program->Use();
			Program->SetBuffer(StatList.GetRef());
			Program->SetBuffer(VisibleInstanceList.GetRef());
			Program->SetBuffer(StaticMeshList.GetRef());
			Program->SetBuffer(CmdBuffer.GetRef());
			Program->SetBuffer(CmdBuffer1.GetRef());

			int GroupSize = CalculateGroupSize((MAX_INSTANCE_NUM / (32 * 32)) + 1);

			GetRHI()->DispatchCompute(GroupSize, GroupSize, 1);
			GetRHI()->MemoryBarrier(MBB_SHADER_STORAGE_BARRIER_BIT);
			GetRHI()->FinishRenderCommand();

			//std::vector<XDrawElementsIndirectCommand> Cmd;
			//CmdBuffer.ReadBack(Cmd);

			//std::vector<XDrawElementsIndirectCommand> Cmd1;
			//CmdBuffer1.ReadBack(Cmd1);

		}

		void IndirectDraw()
		{
			IndirectDrawMaterial.GetContainer()->MaterialState.bEnableDepth = true;
			IndirectDrawMaterial.GetContainer()->MaterialState.bEnableCull = false;
			IndirectDrawMaterial.GetContainer()->MaterialState.bEnableBlend = false;

			IndirectDrawMaterial.GetContainer()->SetupMaterialState();

			auto Program = IndirectDrawMaterial.GetContainer()->ShaderProgramResource;
			Program->Use();
			Program->SetBuffer(PosList.GetRef());
			Program->SetBuffer(IndexList.GetRef());
			Program->SetBuffer(StaticMeshList.GetRef());
			Program->SetBuffer(SceneInstanceList.GetRef());
			Program->SetBuffer(CullingParamList.GetRef());
			Program->SetBuffer(VisibleInstanceList.GetRef());
			Program->SetBuffer(DynamicIndexList.GetRef());
			Program->SetBuffer(CmdBuffer.GetRef());
			Program->SetBuffer(CmdBuffer1.GetRef());
			Program->SetBuffer(StatList.GetRef());
			Program->SetBuffer(DebugList0.GetRef());
			Program->SetBuffer(DebugList1.GetRef());

			{
				//SCOPE_PRIMITIVE_COUNTER(Static Primitive Num)
				{
					SCOPE_TIMER(DrawStaticIBO)
					//STATIC_DRAW_CALL_NUM(CmdBuffer.GetCmdNum())
					GetRHI()->DrawElementIndirect(EDrawMode::DM_TRIANGLES, CmdBuffer.GetRef()->DynamicPointerCast<XRHIDrawIndirectBuffer>(), IndexList.GetRef());
				}
			}

			{				
				//SCOPE_PRIMITIVE_COUNTER(Dyanmic Primitive Num)
				{
					SCOPE_TIMER(DrawDynamicIBO)
					//DYNAMIC_DRAW_CALL_NUM(CmdBuffer1.GetCmdNum())
					GetRHI()->DrawElementIndirect(EDrawMode::DM_TRIANGLES, CmdBuffer1.GetRef()->DynamicPointerCast<XRHIDrawIndirectBuffer>(), DynamicIndexList.GetRef());
				}
			}
			

			GetRHI()->MemoryBarrier(MBB_SHADER_STORAGE_BARRIER_BIT);
			GetRHI()->FinishRenderCommand();

			//DebugList0.ReadBack();
			/*DebugList1.ReadBack();
			StatList.ReadBack();
			StatList[0].VertexCnt;*/
		}

		int CalculateGroupSize(int NeededGroupNum)
		{
			auto NextPowerOfTwo = [](uint32_t x)->uint32_t
				{
					if (x == 0) return 1;
					x--;
					x |= x >> 1;
					x |= x >> 2;
					x |= x >> 4;
					x |= x >> 8;
					x |= x >> 16;
					x++;
					return x;
				};

			int INeededGroupNum = NextPowerOfTwo(NeededGroupNum);
			int k = ceil(log2(float(INeededGroupNum))) / 2 + 1;
			return pow(2, k);
		}

		void PreRendering()
		{
			SCOPE_TIMER(PreRendering)

			auto Program = FinishRenderingMaterial.GetContainer()->ShaderProgramResource;
			Program->Use();
			Program->SetBuffer(StatList.GetRef());
			Program->SetBuffer(VisibleInstanceList.GetRef());
			Program->SetBuffer(CmdBuffer.GetRef());
			Program->SetBuffer(DebugList0.GetRef());
			Program->SetBuffer(DebugList1.GetRef());

			int GroupSize = CalculateGroupSize((MAX_INSTANCE_NUM / (32 * 32)) + 1);

			GetRHI()->DispatchCompute(GroupSize, GroupSize, 1);
			GetRHI()->MemoryBarrier(MBB_SHADER_STORAGE_BARRIER_BIT);
			GetRHI()->FinishRenderCommand();

			//DebugList1.ReadBack();
		}

		void FinishRendering()
		{
			SCOPE_TIMER(FinishRendering)

				auto Program = FinishRenderingMaterial.GetContainer()->ShaderProgramResource;
			Program->Use();
			Program->SetBuffer(StatList.GetRef());
			Program->SetBuffer(VisibleInstanceList.GetRef());
			Program->SetBuffer(CmdBuffer.GetRef());
			Program->SetBuffer(DebugList0.GetRef());
			Program->SetBuffer(DebugList1.GetRef());

			int GroupSize = CalculateGroupSize((MAX_INSTANCE_NUM / (32 * 32)) + 1);

			GetRHI()->DispatchCompute(GroupSize, GroupSize, 1);
			GetRHI()->MemoryBarrier(MBB_SHADER_STORAGE_BARRIER_BIT);
			GetRHI()->FinishRenderCommand();

			//DebugList1.ReadBack();
		}

	public:

		void ImportMaterial()
		{
			{
				MaterialImportSettings Settings;
				Settings.Files[flora::EShaderType::ST_COMPUTE_SHADER] = "FinishRendering.comp";
				if (auto RM = IResource<>::Open<XImporter>(SHADER_ROOT_DIR, Settings))
				{
					auto Material = RM->DynamicPointerCast<XMaterial>();
					FinishRenderingMaterial = std::move(Material);
				}
			}

			{
				MaterialImportSettings Settings;
				Settings.Files[flora::EShaderType::ST_COMPUTE_SHADER] = "GenIndirectDrawCmd.comp";
				if (auto RM = IResource<>::Open<XImporter>(SHADER_ROOT_DIR, Settings))
				{
					auto Material = RM->DynamicPointerCast<XMaterial>();
					GenIndirectDrawCmdMaterial = std::move(Material);
				}
			}

			{
				MaterialImportSettings Settings;
				Settings.Files[flora::EShaderType::ST_COMPUTE_SHADER] = "GenDynamicIndexBuffer.comp";
				if (auto RM = IResource<>::Open<XImporter>(SHADER_ROOT_DIR, Settings))
				{
					auto Material = RM->DynamicPointerCast<XMaterial>();
					GenDynamicIndexBufferMaterial = std::move(Material);
				}
			}
			
			{
				MaterialImportSettings Settings;
				Settings.Files[flora::EShaderType::ST_COMPUTE_SHADER] = "InstanceCulling.comp";
				if (auto RM = IResource<>::Open<XImporter>(SHADER_ROOT_DIR, Settings))
				{
					auto Material = RM->DynamicPointerCast<XMaterial>();
					InstanceCullingMaterial = std::move(Material);
				}
			}

			{
				MaterialImportSettings Settings;
				Settings.Files[flora::EShaderType::ST_VERTEX_SHADER] = "IndirectDraw.vert";
				Settings.Files[flora::EShaderType::ST_FRAGMENT_SHADER] = "DrawPrimitiveId.frag";
				if (auto RM = IResource<>::Open<XImporter>(SHADER_ROOT_DIR, Settings))
				{
					auto Material = RM->DynamicPointerCast<XMaterial>();
					IndirectDrawMaterial = std::move(Material);
				}
			}
		}

		flora::XMaterialProxy IndirectDrawMaterial;
		flora::XMaterialProxy InstanceCullingMaterial;
		flora::XMaterialProxy GenIndirectDrawCmdMaterial;
		flora::XMaterialProxy GenDynamicIndexBufferMaterial;
		flora::XMaterialProxy FinishRenderingMaterial;

	public:
		FByteAddressBuffer<POS_SLOT, EBufferUsage::BU_STATIC_DRAW> PosList;
		FByteAddressBuffer<INDEX_SLOT, EBufferUsage::BU_STATIC_DRAW> IndexList;
		FStructuredBuffer<STATIC_MESH_SLOT, CStaticMesh, MAX_STATIC_MESH_NUM, EBufferUsage::BU_STATIC_DRAW> StaticMeshList;
		FStructuredBuffer<SCENE_INSTANCE_SLOT, CSceneInstance, MAX_INSTANCE_NUM, EBufferUsage::BU_STATIC_DRAW> SceneInstanceList;
		FStructuredBuffer<CULL_PARAM_SLOT, CSceneCullingParam, 1, EBufferUsage::BU_STATIC_DRAW> CullingParamList;
		FStructuredBuffer<VISIBLE_INSTANCE_SLOT, CVisibleInfo, MAX_INSTANCE_NUM, EBufferUsage::BU_STATIC_DRAW> VisibleInstanceList;
		FByteAddressBuffer<DYNAMIC_INDEX_SLOT, EBufferUsage::BU_DYNAMIC_COPY> DynamicIndexList;
		FCommandBuffer<DRAW_COMMAND_SLOT> CmdBuffer;
		FCommandBuffer<DRAW_DYNAMIC_IBO_COMMAND_SLOT> CmdBuffer1;
		FStructuredBuffer<STAT_SLOT, CStat, 1, EBufferUsage::BU_STATIC_DRAW> StatList;
		FStructuredBuffer<DEBUG_0_SLOT, unsigned int, MAX_DEBUG_NUM, EBufferUsage::BU_DYNAMIC_COPY> DebugList0;
		FStructuredBuffer<DEBUG_1_SLOT, mat4, MAX_DEBUG_NUM, EBufferUsage::BU_DYNAMIC_COPY> DebugList1;

		void InitResource()
		{
			auto Meshes = HighModel->GetMeshes();
			int MeshNum = Meshes.size();

			std::vector<glm::vec3> pos_list;
			std::vector<ivec3> triangle_list;

			// init gpu mesh resource
			int GlobalPosOffset = 0;
			int GlobalTriangleOffset = 0;
			int InstanceNum = 0;
			for (int MeshId = 0; MeshId < MeshNum; ++MeshId)
			{
				auto Mesh = Meshes[MeshId];

				// add pos
				for (auto& v : Mesh->vert)
				{
					pos_list.push_back({ v.cP().X(),v.cP().Y(),v.cP().Z() });
				};

				// add triangle
				for (auto& f : Mesh->face)
				{
					triangle_list.push_back({
							f.cV(0)->Index(),
							f.cV(1)->Index(),
							f.cV(2)->Index()
						});
				};

				// add static mesh
				{
					CStaticMesh StaticMesh;
					StaticMesh.pos_offset = GlobalPosOffset;
					StaticMesh.tri_offset = GlobalTriangleOffset;
					StaticMesh.pos_num = Mesh->vert.size();
					StaticMesh.tri_num = Mesh->face.size();

					vcg::Box3f& Box = Mesh->bbox;
					StaticMesh.sphere_box.x = Box.Center().X();
					StaticMesh.sphere_box.y = Box.Center().Y();
					StaticMesh.sphere_box.z = Box.Center().Z();
					StaticMesh.sphere_box.w = Box.Diag() / 2;

					StaticMeshList[MeshId] = StaticMesh;
				}

				// add instance
				{
					CSceneInstance instance;
					instance.static_mesh_resource_id = MeshId;
					instance.model_matrix = Mesh->GetCoordMatrix() * Mesh->GetTransformToRootMatrix();

					SceneInstanceList[MeshId] = instance;
					++InstanceNum;
				}

				GlobalPosOffset += Mesh->vert.size();
				GlobalTriangleOffset += Mesh->face.size();
			};

			StatList[0].StaticMeshNum = MeshNum;
			StatList[0].SceneInstanceNum = InstanceNum;

			PosList.InitRHI(pos_list.data(), sizeof(glm::vec3) * pos_list.size());
			IndexList.InitRHI(triangle_list.data(), sizeof(glm::ivec3) * triangle_list.size());
			StaticMeshList.InitRHI();
			SceneInstanceList.InitRHI();
			CullingParamList.InitRHI();
			VisibleInstanceList.InitRHI();
			DynamicIndexList.InitRHI(nullptr, sizeof(glm::ivec3) * triangle_list.size());
			CmdBuffer.InitRHI(InstanceNum);
			CmdBuffer1.InitRHI(1);
			StatList.InitRHI();
			DebugList0.InitRHI();
			DebugList1.InitRHI();
		}
};

IMPL_APP(MultiDrawTestApp)
