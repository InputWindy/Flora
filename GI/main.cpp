#include <CoreMinimal.h>
#include "Editor.h"

#define STR_CAT(Str1,Str2) Str1##Str2

std::string flora::ExcutePath;

#define WIDTH 800
#define HEIGHT 600

using namespace flora;



struct GIApp : public flora::IApp
{
	GIApp(int argc, char* argv[])
	{
		WindowDesc.Name = "GI";
		WindowDesc.Width = WIDTH;
		WindowDesc.Height = HEIGHT;
		WindowDesc.bHideWindow = false;

		ExcutePath = std::filesystem::canonical(std::filesystem::current_path() / std::filesystem::path(argv[0])).parent_path().generic_string();
		LogPath = "log.txt";

		ParseCmdline(argc, argv);
	}

	virtual void StartUp()
	{
		IApp::StartUp();
		XMaterial::ImportShaderHeaderFiles(SHADER_ROOT_DIR);
		XMaterial::ImportShaderHeaderFiles(STR_CAT(SHADER_ROOT_DIR, "Core/"));

		XMaterial::ImportShaderHeaderFiles(ExcutePath + "/Shaders/");
		XMaterial::ImportShaderHeaderFiles(ExcutePath + "/Shaders/Core/");

		// Create Backbuffer
		{
			XRHIRenderBufferCreateInfo DepthStencilInfo;
			DepthStencilInfo.Width = WIDTH;
			DepthStencilInfo.Height = HEIGHT;
			DepthStencilInfo.InternalFormat = EInternalFormat::IF_DEPTH32F_STENCIL8;

			XRHIFrameBufferCreateInfo BackbufferInfo;
			BackbufferInfo.DepthStencilAttachment = GetRHI()->CreateRenderBuffer(DepthStencilInfo);

			Backbuffer = GetRHI()->CreateFrameBuffer(BackbufferInfo);
		}

		// Create SceneTextures
		{
			XRHITextureCreateInfo SceneTextureInfo;
			SceneTextureInfo.TextureType = ETextureTarget::TT_TEXTURE_2D;
			SceneTextureInfo.SizeX = WIDTH;
			SceneTextureInfo.SizeY = HEIGHT;
			SceneTextureInfo.SizeZ = 1;
			SceneTextureInfo.MipLevels = 1;
			SceneTextureInfo.InternalFormat = EInternalFormat::IF_RGB8UI;
			SceneTextureInfo.DefaultSamplerInfo.WrapR = EWrapMode::WM_CLAMP_TO_EDGE;
			SceneTextureInfo.DefaultSamplerInfo.WrapS = EWrapMode::WM_CLAMP_TO_EDGE;
			SceneTextureInfo.DefaultSamplerInfo.WrapT = EWrapMode::WM_CLAMP_TO_EDGE;
			SceneTextureInfo.DefaultSamplerInfo.MinFilterMode = EFilterMode::FM_LINEAR;
			SceneTextureInfo.DefaultSamplerInfo.MagFilterMode = EFilterMode::FM_LINEAR;
			SceneTextureInfo.DefaultSamplerInfo.MaxMipLevel = 1;
			SceneTextureInfo.DefaultSamplerInfo.MinMipLevel = 1;
			SceneTextureInfo.DefaultSamplerInfo.BorderColor = { 0,0,0,0 };

			SceneTextures["SceneColor"] = GetRHI()->CreateTexture(SceneTextureInfo);
		}
	};

	//Main Loop
	virtual void Run()override
	{
		//TODO:Do Something Before Run

		


		while (BeginFrame())
		{
			ProcessInput();

			//UpdateCamera();
			//UpdateGameScene();
			//RenderScene();

		#ifdef WITH_GUI
			Editor->BeginRender();
			Editor->Render();
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

private:

	std::map<std::string, std::shared_ptr<XRHITexture>> SceneTextures;

	std::shared_ptr<XRHIFrameBuffer> Backbuffer;

private:
	flora::XTransform CameraTrans;

	void ProcessInput()
	{
		flora::XTransform& CameraTransform = CameraTrans;
		CameraTransform.SetMovementSpeed(300);
		{
			// camera
			static float lastX = WIDTH / 2.0;
			static float lastY = HEIGHT / 2.0;
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

};

#ifdef PY_MODULE_NAME
#include "PyRegister.h"
PY_REGISTER(GI,

	.def(py::init<>())

	.def("start_up", &GIApp::StartUp)
	.def("run", &GIApp::Run)
	.def("shutdown", &GIApp::Shutdown)
	, GIApp
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
IMPL_APP(GIApp)
#endif

