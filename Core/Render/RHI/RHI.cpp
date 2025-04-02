#include <Render/RHI/RHI.h>
#include <Render/RHI/OpenGL/OpenGLRHI.h>
#include <Resource/Struct/Material.h>
#include <Render/Proxy/ProxyBase.h>
#include <Render/Proxy/Continer/MaterialFieldContainer.h>
#include <cassert>
#include <CoreMinimal.h>

flora::XRHI* GRhi = nullptr;

template<>
std::shared_ptr<flora::XRHI> flora::XRHI::InitRHI<flora::ERHIFeatureLevel::FL_OpenGL>(void* Callback)
{
    if (GRhi)return nullptr;

    std::shared_ptr<flora::XOpenGLRHI> Rhi = std::make_shared<flora::XOpenGLRHI>();
    if (Rhi->InitOpenGL(Callback))
    {
        GRhi = Rhi.get();

        return Rhi;
    }
    return nullptr;
}

flora::XRHI* flora::GetRHI()
{
    return GRhi;
}

flora::EInternalFormat flora::MatchFormat(flora::EFormat InFormat, flora::EDataType InDataType)
{
    //png jpg jpeg tga ...
    if (InDataType == flora::EDataType::DT_UNSIGNED_BYTE)
    {
        switch (InFormat)
        {
        case flora::EFormat::F_RED:return EInternalFormat::IF_R8;
        case flora::EFormat::F_RG:return EInternalFormat::IF_RG8;
        case flora::EFormat::F_RGB:return EInternalFormat::IF_RGB8;
        case flora::EFormat::F_BGR:break;
        case flora::EFormat::F_RGBA:return EInternalFormat::IF_RGBA8;
        case flora::EFormat::F_BGRA:break;
        case flora::EFormat::F_RED_INTEGER:return EInternalFormat::IF_R8UI;
        case flora::EFormat::F_RG_INTEGER:return EInternalFormat::IF_RG8UI;
        case flora::EFormat::F_RGB_INTEGER:return EInternalFormat::IF_RGB8UI;
        case flora::EFormat::F_BGR_INTEGER:break;
        case flora::EFormat::F_RGBA_INTEGER:return EInternalFormat::IF_RGBA8UI;
        case flora::EFormat::F_BGRA_INTEGER:break;
        case flora::EFormat::F_STENCIL_INDEX:
        case flora::EFormat::F_DEPTH_COMPONENT:
        case flora::EFormat::F_DEPTH_STENCIL:
        case flora::EFormat::F_MAX_COUNT:
        default:
            break;
        }
        assert(0);
    }
    //hdr exr...
    else if(InDataType == flora::EDataType::DT_FLOAT)
    {
        switch (InFormat)
        {
        case flora::EFormat::F_RED:return EInternalFormat::IF_R32F;
        case flora::EFormat::F_RG:return EInternalFormat::IF_RG32F;
        case flora::EFormat::F_RGB:return EInternalFormat::IF_RGB32F;
        case flora::EFormat::F_BGR:break;
        case flora::EFormat::F_RGBA:return EInternalFormat::IF_RGBA32F;
        case flora::EFormat::F_BGRA:break;
        case flora::EFormat::F_RED_INTEGER:break;
        case flora::EFormat::F_RG_INTEGER:break;
        case flora::EFormat::F_RGB_INTEGER:break;
        case flora::EFormat::F_BGR_INTEGER:break;
        case flora::EFormat::F_RGBA_INTEGER:break;
        case flora::EFormat::F_BGRA_INTEGER:break;
        case flora::EFormat::F_STENCIL_INDEX:
        case flora::EFormat::F_DEPTH_COMPONENT:
        case flora::EFormat::F_DEPTH_STENCIL:
        case flora::EFormat::F_MAX_COUNT:
        default:
            break;
        }
        assert(0);
    }
    //hdr exr...
    else if (InDataType == flora::EDataType::DT_HALF_FLOAT)
    {
        switch (InFormat)
        {
        case flora::EFormat::F_RED:return EInternalFormat::IF_R16F;
        case flora::EFormat::F_RG:return EInternalFormat::IF_RG16F;
        case flora::EFormat::F_RGB:return EInternalFormat::IF_RGB16F;
        case flora::EFormat::F_BGR:break;
        case flora::EFormat::F_RGBA:return EInternalFormat::IF_RGBA16F;
        case flora::EFormat::F_BGRA:break;
        case flora::EFormat::F_RED_INTEGER:break;
        case flora::EFormat::F_RG_INTEGER:break;
        case flora::EFormat::F_RGB_INTEGER:break;
        case flora::EFormat::F_BGR_INTEGER:break;
        case flora::EFormat::F_RGBA_INTEGER:break;
        case flora::EFormat::F_BGRA_INTEGER:break;
        case flora::EFormat::F_STENCIL_INDEX:
        case flora::EFormat::F_DEPTH_COMPONENT:
        case flora::EFormat::F_DEPTH_STENCIL:
        case flora::EFormat::F_MAX_COUNT:
        default:
            break;
        }
        assert(0);
    }
    else assert(0);

}

std::pair<flora::EFormat, flora::EDataType> flora::MatchInternalFormat(flora::EInternalFormat InInternalFormat)
{
    std::pair<flora::EFormat, flora::EDataType> Ret;

    switch (InInternalFormat)
    {
    case flora::EInternalFormat::IF_R8:        Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RED, flora::EDataType::DT_UNSIGNED_BYTE); break;
    case flora::EInternalFormat::IF_R8I:       Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RED_INTEGER, flora::EDataType::DT_BYTE); break;
    case flora::EInternalFormat::IF_R8UI:      Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RED_INTEGER, flora::EDataType::DT_UNSIGNED_BYTE); break;

    case flora::EInternalFormat::IF_R16:       Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RED, flora::EDataType::DT_UNSIGNED_SHORT); break;
    case flora::EInternalFormat::IF_R16I:      Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RED_INTEGER, flora::EDataType::DT_SHORT); break;
    case flora::EInternalFormat::IF_R16UI:     Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RED_INTEGER, flora::EDataType::DT_UNSIGNED_SHORT); break;

    case flora::EInternalFormat::IF_R32I:      Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RED_INTEGER, flora::EDataType::DT_INT); break;
    case flora::EInternalFormat::IF_R32UI:     Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RED_INTEGER, flora::EDataType::DT_UNSIGNED_INT); break;

    case flora::EInternalFormat::IF_RG8:       Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RG, flora::EDataType::DT_UNSIGNED_BYTE); break;
    case flora::EInternalFormat::IF_RG8I:      Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RG_INTEGER, flora::EDataType::DT_BYTE); break;
    case flora::EInternalFormat::IF_RG8UI:     Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RG_INTEGER, flora::EDataType::DT_UNSIGNED_BYTE); break;

    case flora::EInternalFormat::IF_RG16:      Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RG, flora::EDataType::DT_UNSIGNED_SHORT); break;
    case flora::EInternalFormat::IF_RG16I:     Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RG_INTEGER, flora::EDataType::DT_SHORT); break;
    case flora::EInternalFormat::IF_RG16UI:    Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RG_INTEGER, flora::EDataType::DT_UNSIGNED_SHORT); break;

    case flora::EInternalFormat::IF_RG32I:     Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RG_INTEGER, flora::EDataType::DT_INT); break;
    case flora::EInternalFormat::IF_RG32UI:    Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RG_INTEGER, flora::EDataType::DT_UNSIGNED_INT); break;

    case flora::EInternalFormat::IF_RGB8:      Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGB, flora::EDataType::DT_UNSIGNED_BYTE);              break;
    case flora::EInternalFormat::IF_RGB8I:     Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGB_INTEGER, flora::EDataType::DT_BYTE); break;
    case flora::EInternalFormat::IF_RGB8UI:    Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGB_INTEGER, flora::EDataType::DT_UNSIGNED_BYTE); break;

    case flora::EInternalFormat::IF_RGB16:     Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGB, flora::EDataType::DT_UNSIGNED_SHORT); break;
    case flora::EInternalFormat::IF_RGB16I:    Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGB_INTEGER, flora::EDataType::DT_SHORT); break;
    case flora::EInternalFormat::IF_RGB16UI:   Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGB_INTEGER, flora::EDataType::DT_UNSIGNED_SHORT); break;
    
    case flora::EInternalFormat::IF_RGB32I:    Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGB_INTEGER, flora::EDataType::DT_INT); break;
    case flora::EInternalFormat::IF_RGB32UI:   Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGB_INTEGER, flora::EDataType::DT_UNSIGNED_INT); break;
    
    case flora::EInternalFormat::IF_RGBA8:     Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGBA, flora::EDataType::DT_UNSIGNED_BYTE);              break;
    case flora::EInternalFormat::IF_RGBA8I:    Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGBA_INTEGER, flora::EDataType::DT_BYTE); break;
    case flora::EInternalFormat::IF_RGBA8UI:   Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGBA_INTEGER, flora::EDataType::DT_UNSIGNED_BYTE); break;
    
    case flora::EInternalFormat::IF_RGBA16:    Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGBA, flora::EDataType::DT_UNSIGNED_SHORT); break;
    case flora::EInternalFormat::IF_RGBA16I:   Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGBA_INTEGER, flora::EDataType::DT_SHORT); break;
    case flora::EInternalFormat::IF_RGBA16UI:  Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGBA_INTEGER, flora::EDataType::DT_UNSIGNED_SHORT); break;
    
    case flora::EInternalFormat::IF_RGBA32I:   Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGBA_INTEGER, flora::EDataType::DT_INT); break;
    case flora::EInternalFormat::IF_RGBA32UI:  Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGBA_INTEGER, flora::EDataType::DT_UNSIGNED_INT); break;
    
    case flora::EInternalFormat::IF_R16F:      Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RED, flora::EDataType::DT_FLOAT); break;
    case flora::EInternalFormat::IF_R32F:      Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RED, flora::EDataType::DT_FLOAT);              break;
    case flora::EInternalFormat::IF_RG16F:     Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RG, flora::EDataType::DT_FLOAT); break;
    case flora::EInternalFormat::IF_RG32F:     Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RG, flora::EDataType::DT_FLOAT);              break;
    case flora::EInternalFormat::IF_RGB16F:    Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGB, flora::EDataType::DT_FLOAT); break;
    case flora::EInternalFormat::IF_RGB32F:    Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGB, flora::EDataType::DT_FLOAT);              break;
    case flora::EInternalFormat::IF_RGBA16F:   Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGBA, flora::EDataType::DT_FLOAT); break;
    case flora::EInternalFormat::IF_RGBA32F:   Ret = std::pair<flora::EFormat, flora::EDataType>(flora::EFormat::F_RGBA, flora::EDataType::DT_FLOAT);              break;

    case flora::EInternalFormat::IF_DEPTH_COMPONENT16: break;
    case flora::EInternalFormat::IF_DEPTH_COMPONENT24: break;
    case flora::EInternalFormat::IF_DEPTH_COMPONENT32F:break;
    case flora::EInternalFormat::IF_DEPTH24_STENCIL8:break;
    case flora::EInternalFormat::IF_DEPTH32F_STENCIL8:break;
    case flora::EInternalFormat::IF_STENCIL_INDEX:break;
    case flora::EInternalFormat::IF_MAX_COUNT:
    default:
        break;
    }
    return Ret;
}

template<>
bool flora::MatchDataType<signed char>(flora::EDataType DT)
{
    return DT == EDataType::DT_BYTE;
};

template<>
bool flora::MatchDataType<unsigned char>(flora::EDataType DT)
{
    return DT == EDataType::DT_UNSIGNED_BYTE;
};

template<>
bool flora::MatchDataType<signed short>(flora::EDataType DT)
{
    return DT == EDataType::DT_SHORT;
};

template<>
bool flora::MatchDataType<unsigned short>(flora::EDataType DT)
{
    return DT == EDataType::DT_UNSIGNED_SHORT;
};

template<>
bool flora::MatchDataType<int>(flora::EDataType DT)
{
    return DT == EDataType::DT_INT;
};

template<>
bool flora::MatchDataType<unsigned int>(flora::EDataType DT)
{
    return DT == EDataType::DT_UNSIGNED_INT;
};

template<>
bool flora::MatchDataType<float>(flora::EDataType DT)
{
    return DT == EDataType::DT_FLOAT;
};

std::shared_ptr<flora::XRHITextureCube> flora::XRHI::PanoramaToCubemap(uint32_t CubeSize, std::shared_ptr<flora::XRHITexture2D> InPanorama)
{
    if (InPanorama)
    {
        std::shared_ptr<XStaticMesh> SkyboxMesh = std::make_shared<XStaticMesh>();
        vcg::tri::Allocator<XStaticMesh>::AddVertex(*SkyboxMesh, { 1,-1,-1 });
        vcg::tri::Allocator<XStaticMesh>::AddVertex(*SkyboxMesh, { 1,-1,1 });
        vcg::tri::Allocator<XStaticMesh>::AddVertex(*SkyboxMesh, { 1, 1,-1 });
        vcg::tri::Allocator<XStaticMesh>::AddVertex(*SkyboxMesh, { 1, 1,1 });
        vcg::tri::Allocator<XStaticMesh>::AddVertex(*SkyboxMesh, { -1,-1,-1 });
        vcg::tri::Allocator<XStaticMesh>::AddVertex(*SkyboxMesh, { -1,-1,1 });
        vcg::tri::Allocator<XStaticMesh>::AddVertex(*SkyboxMesh, { -1, 1,-1 });
        vcg::tri::Allocator<XStaticMesh>::AddVertex(*SkyboxMesh, { -1, 1,1 });

        vcg::tri::Allocator<XStaticMesh>::AddFace(*SkyboxMesh, 0, 1, 2);
        vcg::tri::Allocator<XStaticMesh>::AddFace(*SkyboxMesh, 3, 1, 2);

        vcg::tri::Allocator<XStaticMesh>::AddFace(*SkyboxMesh, 4, 5, 6);
        vcg::tri::Allocator<XStaticMesh>::AddFace(*SkyboxMesh, 7, 5, 6);

        vcg::tri::Allocator<XStaticMesh>::AddFace(*SkyboxMesh, 1, 3, 7);
        vcg::tri::Allocator<XStaticMesh>::AddFace(*SkyboxMesh, 1, 5, 7);

        vcg::tri::Allocator<XStaticMesh>::AddFace(*SkyboxMesh, 0, 4, 6);
        vcg::tri::Allocator<XStaticMesh>::AddFace(*SkyboxMesh, 2, 0, 6);

        vcg::tri::Allocator<XStaticMesh>::AddFace(*SkyboxMesh, 5, 1, 0);
        vcg::tri::Allocator<XStaticMesh>::AddFace(*SkyboxMesh, 5, 4, 0);

        vcg::tri::Allocator<XStaticMesh>::AddFace(*SkyboxMesh, 3, 7, 2);
        vcg::tri::Allocator<XStaticMesh>::AddFace(*SkyboxMesh, 7, 6, 2);
        XStaticMeshProxy Skybox(SkyboxMesh);

        std::shared_ptr<XMaterial> Material = std::make_shared<XMaterial>();
        Material->GetShaderMap()[EShaderType::ST_VERTEX_SHADER] = R"(
				#version 330 core
                layout (location = 0) in vec3 aPos;

                out vec3 localPos;

                uniform mat4 model;
                uniform mat4 projection;
                uniform mat4 view;

                void main()
                {
                    localPos = aPos;  
                    gl_Position =  projection * view * vec4(localPos, 1.0);
                }
		)";
        Material->GetShaderMap()[EShaderType::ST_FRAGMENT_SHADER] = R"(
				#version 330 core
                out vec4 FragColor;
                in vec3 localPos;

                uniform sampler2D equirectangularMap;

                uniform uint face_id;

                void main()
                {	
                    vec3 t_V1 = vec3(-localPos.x, localPos.y, localPos.z);//mul(coord , float4(V1,1)).rgb;

                    vec2 uv = vec2(atan(t_V1.y, t_V1.x) * 0.1591549, 0.5 + atan(-t_V1.z, sqrt(t_V1.x * t_V1.x + t_V1.y * t_V1.y)) * 0.3183099);

                    vec3 color = texture(equirectangularMap, uv).rgb;
    
                    FragColor = vec4(color, 1.0);
                }
		)";
        Material->GetMaterialState().bEnableDepth = true;
        Material->GetMaterialState().bEnableCull = true;
        Material->GetMaterialState().FrontFace = EFrontFace::FF_CW;
        Material->GetMaterialState().CullFace = ECullFace::CF_BACK;
        XMaterialProxy CubemapMaterial = std::move(Material);

        XRHITextureCreateInfo CubemapInfo;
        CubemapInfo.MipLevels = 1;
        CubemapInfo.SizeX = CubeSize;
        CubemapInfo.TextureType = ETextureTarget::TT_TEXTURE_CUBE_MAP;
        CubemapInfo.InternalFormat = InPanorama->GetInternalFormat();
        CubemapInfo.DefaultSamplerInfo.MinFilterMode = EFilterMode::FM_LINEAR;
        CubemapInfo.DefaultSamplerInfo.MagFilterMode = EFilterMode::FM_LINEAR;
        CubemapInfo.DefaultSamplerInfo.WrapR = EWrapMode::WM_CLAMP_TO_EDGE;
        CubemapInfo.DefaultSamplerInfo.WrapS = EWrapMode::WM_CLAMP_TO_EDGE;
        CubemapInfo.DefaultSamplerInfo.WrapT = EWrapMode::WM_CLAMP_TO_EDGE;

        auto Cubemap = GetRHI()->CreateTexture(CubemapInfo)->DynamicPointerCast<XRHITextureCube>();

        XRHIRenderBufferCreateInfo RenderBufferInfo;
        RenderBufferInfo.Width = CubeSize;
        RenderBufferInfo.Height = CubeSize;
        RenderBufferInfo.InternalFormat = EInternalFormat::IF_DEPTH24_STENCIL8;

        XRHIFrameBufferCreateInfo FramebufferInfo;
        FramebufferInfo.DepthStencilAttachment = GetRHI()->CreateRenderBuffer(RenderBufferInfo);
        FramebufferInfo.ColorAttachments =
        {
            {"FinalColor",Cubemap,0,PX}
        };
        FramebufferInfo.DrawBuffers = { 0 };

        auto BackBuffer = GetRHI()->CreateFrameBuffer(FramebufferInfo);

        glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
        glm::mat4 captureViews[] =
        {
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), -glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), -glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
            glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
        };

        CubemapMaterial.GetContainer()->ShaderProgramResource->Use();
        CubemapMaterial.GetContainer()->ShaderProgramResource->SetMatrixFloat4x4Array("projection", 1, false, glm::value_ptr(captureProjection));
        CubemapMaterial.GetContainer()->ShaderProgramResource->SetTexture("equirectangularMap", 0, InPanorama, nullptr);

        for (ECubeFace i = 0; i < ECubeFace_::MAX_COUNT; i++)
        {
            BackBuffer->SetColorAttachment(
                {
                    {"FinalColor",Cubemap,0,i}
                });
            BackBuffer->EnableOutputAttachments({ 0 });
            BackBuffer->UpdateRHI();

            BackBuffer->Bind();
            {
                GetRHI()->ClearFrameBuffer(CBB_COLOR_BUFFER_BIT | CBB_DEPTH_BUFFER_BIT | CBB_STENCIL_BUFFER_BIT);
                GetRHI()->ResizeViewport(0, 0, CubeSize, CubeSize);

                CubemapMaterial.GetContainer()->ShaderProgramResource->SetMatrixFloat4x4Array("view", 1, false, glm::value_ptr(captureViews[i]));
                Skybox.GetContainer()->DrawElementInstanced(EDrawMode::DM_TRIANGLES);
            }
            BackBuffer->UnBind();
        }

        return Cubemap;
    }
    return nullptr;
}

std::shared_ptr<flora::XRHITexture2D> flora::XRHI::CubemapFaceToTexture2D(ECubeFace Face, std::shared_ptr<XRHITextureCube> Cubemap)
{
    if (Cubemap)
    {
        uint32_t SizeX = Cubemap->GetSizeX();
        EInternalFormat InternalFormat = Cubemap->GetInternalFormat();

        std::pair<flora::EFormat, flora::EDataType> ReadFormat = flora::MatchInternalFormat(InternalFormat);

        std::vector<std::byte> Buffer;
        {
            int Stride = GetRHI()->GetDataTypeSize(ReadFormat.second) * GetRHI()->GetFormatCompNum(ReadFormat.first);
            Buffer.resize(Stride * SizeX * SizeX);
        }

        Cubemap->ReadPixels(Face,0, ReadFormat.first, ReadFormat.second, Buffer.data());

        /*uint8_t* p = (uint8_t*)(Buffer.data());
        while (p)
        {
            std::cout << p << " ";
            ++p;
        }*/

        XRHITextureCreateInfo Texture2DInfo;
        Texture2DInfo.TextureType = ETextureTarget::TT_TEXTURE_2D;
        Texture2DInfo.SizeX = SizeX;
        Texture2DInfo.SizeY = SizeX;
        Texture2DInfo.InternalFormat = InternalFormat;

        std::shared_ptr<flora::XRHITexture2D> Texture2D = GetRHI()->CreateTexture(Texture2DInfo)->DynamicPointerCast<flora::XRHITexture2D>();
        Texture2D->SetPixels(0,ReadFormat.first, ReadFormat.second, Buffer.data());
        
        return Texture2D;
    }

    return nullptr;
}

std::shared_ptr<flora::XTexture> flora::XRHI::CreateStaticTexture(std::shared_ptr<XRHITexture> InTexture)
{
    if (!InTexture)return nullptr;

    std::shared_ptr<XTexture> Ret = nullptr;
    switch (InTexture->GetType())
    {
    case flora::ERHIResourceType::RT_Texture2D:
    {
        auto Texture2D = std::make_shared<XTexture2D>();
        auto Texture2DResource = InTexture->DynamicPointerCast<XRHITexture2D>();

        auto SizeX = Texture2DResource->GetSizeX();
        auto SizeY = Texture2DResource->GetSizeY();
        auto MipLevels = Texture2DResource->GetMipLevels();
        auto InternalFormat = Texture2DResource->GetInternalFormat();
        auto ReadFormat = MatchInternalFormat(InternalFormat);
        int CompNum = GetRHI()->GetFormatCompNum(ReadFormat.first);
        std::byte* Buffer = (std::byte*)malloc(SizeX* SizeY * GetRHI()->GetDataTypeSize(ReadFormat.second) * GetRHI()->GetFormatCompNum(ReadFormat.first));
        {
            Texture2DResource->ReadPixels(0, ReadFormat.first, ReadFormat.second, Buffer);
        }
        
        Texture2D->Width = SizeX;
        Texture2D->Height = SizeY;

        Texture2D->ChannelNum = CompNum;
        Texture2D->Format = ReadFormat.first;
        Texture2D->DataType; ReadFormat.second;
        
        Texture2D->ImageData = Buffer;
        Texture2D->MipLevels = MipLevels;

        //Texture2D->ID = 0;
        //Texture2D->FilePath = "";
        //Texture2D->Reference;

        Ret = Texture2D;

        break;
    }
    case flora::ERHIResourceType::RT_Texture3D:
    {

        break;
    }
    case flora::ERHIResourceType::RT_TextureCube:
    {

        break;
    }
    default:
        break;
    }
    return Ret;
}

std::vector<std::shared_ptr<flora::XTexture>> flora::XRHI::CreateStaticTextureMipChain(std::shared_ptr<XRHITexture>InTexture)
{
	std::vector<std::shared_ptr<flora::XTexture>> Ret;

	if (!InTexture)return Ret;

	switch (InTexture->GetType())
	{
	case flora::ERHIResourceType::RT_Texture2D:
	{		
		auto Texture2DResource = InTexture->DynamicPointerCast<XRHITexture2D>();
		auto MipLevels = Texture2DResource->GetMipLevels();

        for (int i = 0; i < MipLevels; ++i)
        {
			auto Texture2D = std::make_shared<XTexture2D>();

			auto SizeX = Texture2DResource->GetSizeX(i);
			auto SizeY = Texture2DResource->GetSizeY(i);
			auto InternalFormat = Texture2DResource->GetInternalFormat();
			auto ReadFormat = MatchInternalFormat(InternalFormat);
			int CompNum = GetRHI()->GetFormatCompNum(ReadFormat.first);
			std::byte* Buffer = (std::byte*)malloc(SizeX * SizeY * GetRHI()->GetDataTypeSize(ReadFormat.second) * GetRHI()->GetFormatCompNum(ReadFormat.first));
			{
				Texture2DResource->ReadPixels(i, ReadFormat.first, ReadFormat.second, Buffer);
			}

			Texture2D->Width = SizeX;
			Texture2D->Height = SizeY;

			Texture2D->ChannelNum = CompNum;
			Texture2D->Format = ReadFormat.first;
			Texture2D->DataType; ReadFormat.second;

			Texture2D->ImageData = Buffer;
			Texture2D->MipLevels = 1;

			Ret.emplace_back(Texture2D);
        }
		break;
	}
	case flora::ERHIResourceType::RT_Texture3D:
	{

		break;
	}
	case flora::ERHIResourceType::RT_TextureCube:
	{

		break;
	}
	default:
		break;
	}
	return Ret;
}


std::shared_ptr<flora::XRHITextureCube> flora::XRHI::FacesToCubemap(std::vector<std::shared_ptr<XRHITexture2D>> InFaces)
{
    if (InFaces.size() != 6)return nullptr;

    bool bValid = false;
    for (size_t i = 0; i < 6; i++)bValid = InFaces[i] != nullptr;

    if (!bValid)return nullptr;

    {
        int CubeSize = InFaces[0]->GetSizeX();

        XRHITextureCreateInfo CubemapInfo;
        CubemapInfo.MipLevels = 1;
        CubemapInfo.SizeX = CubeSize;
        CubemapInfo.TextureType = ETextureTarget::TT_TEXTURE_CUBE_MAP;
        CubemapInfo.InternalFormat = InFaces[0]->GetInternalFormat();
        CubemapInfo.DefaultSamplerInfo.MinFilterMode = EFilterMode::FM_LINEAR;
        CubemapInfo.DefaultSamplerInfo.MagFilterMode = EFilterMode::FM_LINEAR;
        CubemapInfo.DefaultSamplerInfo.WrapR = EWrapMode::WM_CLAMP_TO_EDGE;
        CubemapInfo.DefaultSamplerInfo.WrapS = EWrapMode::WM_CLAMP_TO_EDGE;
        CubemapInfo.DefaultSamplerInfo.WrapT = EWrapMode::WM_CLAMP_TO_EDGE;

        auto Cubemap = GetRHI()->CreateTexture(CubemapInfo)->DynamicPointerCast<XRHITextureCube>();

        std::pair<flora::EFormat, flora::EDataType> ReadFormat = flora::MatchInternalFormat(CubemapInfo.InternalFormat);
        std::vector<std::byte> Buffer;
        {
            int Stride = GetRHI()->GetDataTypeSize(ReadFormat.second) * GetRHI()->GetFormatCompNum(ReadFormat.first);
            Buffer.resize(Stride * CubeSize * CubeSize);
        }

        for (ECubeFace FaceId = 0; FaceId < ECubeFace_::MAX_COUNT; FaceId++)
        {
            InFaces[FaceId]->ReadPixels(0, ReadFormat.first, ReadFormat.second, Buffer.data());
            Cubemap->SetPixels(FaceId,0, ReadFormat.first, ReadFormat.second, Buffer.data());
        }

        return Cubemap;
    }

    return nullptr;
}



