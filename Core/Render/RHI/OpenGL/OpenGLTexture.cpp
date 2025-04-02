#include "OpenGLTexture.h"
#include <cmath>
using namespace std;
XVerse::XOpenGLTexture2D::~XOpenGLTexture2D()
{
    ReleaseRHI();
}

bool XVerse::XOpenGLTexture2D::IsValid()
{
    Bind();
    return glIsTexture(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLTexture2D::InitRHI()
{
    glGenTextures(1, &Handle);
    Bind();

    auto WriteFormat = MatchInternalFormat(InternalFormat);

    for (size_t i = 0; i < MipLevels; i++)
    {
        glTexImage2D(GL_TEXTURE_2D, i, ToGLInternalFormat(InternalFormat), SizeX[0] / powf(2, i), SizeY[0] / powf(2, i), 0, ToGLFormat(WriteFormat.first), ToGLDataType(WriteFormat.second), NULL);

        if (i > 0)
		{
			SizeX[i] = SizeX[0] / powf(2, i);
			SizeY[i] = SizeY[0] / powf(2, i);
        }
    }
    //SetSamplerState(XRHISamplerCreateInfo());

    return glIsTexture(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLTexture2D::ReleaseRHI()
{
    glDeleteTextures(1, &Handle);
    return true;
}

bool XVerse::XOpenGLTexture2D::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool XVerse::XOpenGLTexture2D::UpdateRHI()
{
    return true;
}

void XVerse::XOpenGLTexture2D::AddNewMip(uint32_t InSizeX, uint32_t InSizeY)
{
	Bind();

	auto WriteFormat = MatchInternalFormat(InternalFormat);

	glTexImage2D(GL_TEXTURE_2D, MipLevels, ToGLInternalFormat(InternalFormat), InSizeX, InSizeY, 0, ToGLFormat(WriteFormat.first), ToGLDataType(WriteFormat.second), NULL);

	SizeX[MipLevels] = InSizeX;
	SizeY[MipLevels] = InSizeY;

    ++MipLevels;
}

void XVerse::XOpenGLTexture2D::Bind()
{
    glBindTexture(GL_TEXTURE_2D, Handle);
}

void XVerse::XOpenGLTexture2D::UnBind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}


void XVerse::XOpenGLTexture2D::ReadPixels(int level, EFormat format, EDataType type, void* pixels)
{
    Bind();
    glGetTexImage(GL_TEXTURE_2D, level, ToGLFormat(format), ToGLDataType(type), pixels);
}

void XVerse::XOpenGLTexture2D::SetPixels(uint32_t level, EFormat format, EDataType type, const void* data)
{
    Bind();
    glTexImage2D(GL_TEXTURE_2D, level, ToGLInternalFormat(InternalFormat), SizeX[level], SizeY[level], 0, ToGLFormat(format), ToGLDataType(type), data);
}

//void XVerse::XOpenGLTexture2D::SetMinFilter(EFilterMode Mode) const
//{
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(Mode));
//}
//
//void XVerse::XOpenGLTexture2D::SetMagFilter(EFilterMode Mode) const
//{
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(Mode));
//}
//
//void XVerse::XOpenGLTexture2D::SetWrapR(EWrapMode Mode) const
//{
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, ToGLWrapMode(Mode));
//}
//
//void XVerse::XOpenGLTexture2D::SetWrapS(EWrapMode Mode) const
//{
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGLWrapMode(Mode));
//
//}
//
//void XVerse::XOpenGLTexture2D::SetWrapT(EWrapMode Mode) const
//{
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGLWrapMode(Mode));
//
//}
//
//void XVerse::XOpenGLTexture2D::SetMinLod(uint8_t Lod) const
//{
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, Lod);
//
//}
//
//void XVerse::XOpenGLTexture2D::SetMaxLod(uint8_t Lod) const
//{
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, Lod);
//
//}
//
//void XVerse::XOpenGLTexture2D::SetLodBias(float Bias) const
//{
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, Bias);
//
//}
//
//void XVerse::XOpenGLTexture2D::SetBorderColor(float R, float G, float B, float A) const
//{
//    const GLfloat color[4] = { R,G,B,A };
//    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
//}
//
//void XVerse::XOpenGLTexture2D::SetCompareMode(ETextureCompareMode Mode) const
//{
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, ToGLTextureCompareMode(Mode));
//}
//
//void XVerse::XOpenGLTexture2D::SetCompareFunc(ECompareFunc Func) const
//{
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, ToGLCompareFunc(Func));
//}

void XVerse::XOpenGLTexture2D::SetSamplerState(const XRHISamplerCreateInfo& InSampleInfo)
{
    Bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(InSampleInfo.MinFilterMode));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ToGLFilterMode(InSampleInfo.MagFilterMode));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, ToGLWrapMode(InSampleInfo.WrapR));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGLWrapMode(InSampleInfo.WrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGLWrapMode(InSampleInfo.WrapT));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, InSampleInfo.MinMipLevel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, InSampleInfo.MaxMipLevel);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, InSampleInfo.LodBias);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, ToGLTextureCompareMode(InSampleInfo.CompareMode));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, ToGLCompareFunc(InSampleInfo.CompareFunc));

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (const GLfloat*)&(InSampleInfo.BorderColor[0]));
}



XVerse::XOpenGLVolumeTexture::~XOpenGLVolumeTexture()
{
    ReleaseRHI();
}

bool XVerse::XOpenGLVolumeTexture::IsValid()
{
    Bind();
    return glIsTexture(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLVolumeTexture::InitRHI()
{
    glGenTextures(1, &Handle);
    Bind();

    auto WriteFormat = MatchInternalFormat(InternalFormat);

    for (size_t i = 0; i < MipLevels; i++)
    {
        glTexImage3D(GL_TEXTURE_3D, i, ToGLInternalFormat(InternalFormat), SizeX / powf(2, i), SizeY / powf(2, i), SizeZ / powf(2, i), 0, ToGLFormat(WriteFormat.first), ToGLDataType(WriteFormat.second), NULL);
    }
    //SetSamplerState(XRHISamplerCreateInfo());

    return glIsTexture(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLVolumeTexture::ReleaseRHI()
{
    glDeleteTextures(1, &Handle);
    return true;
}

bool XVerse::XOpenGLVolumeTexture::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool XVerse::XOpenGLVolumeTexture::UpdateRHI()
{
    return true;
}

void XVerse::XOpenGLVolumeTexture::Bind()
{
    glBindTexture(GL_TEXTURE_3D, Handle);

}

void XVerse::XOpenGLVolumeTexture::UnBind()
{
    glBindTexture(GL_TEXTURE_3D, 0);

}

void XVerse::XOpenGLVolumeTexture::ReadPixels(int level, EFormat format, EDataType type, void* pixels)
{
    Bind();
    glGetTexImage(GL_TEXTURE_3D, level, ToGLFormat(format), ToGLDataType(type), pixels);
}

void XVerse::XOpenGLVolumeTexture::SetPixels(uint32_t level, EFormat format, EDataType type, const void* pixels)
{
    Bind();
    glTexImage3D(GL_TEXTURE_3D, level, ToGLInternalFormat(InternalFormat), SizeX / powf(2, level), SizeY / powf(2, level), SizeZ / powf(2, level), 0, ToGLFormat(format), ToGLDataType(type), pixels);
}

//void XVerse::XOpenGLVolumeTexture::SetMinFilter(EFilterMode Mode) const
//{
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(Mode));
//}
//
//void XVerse::XOpenGLVolumeTexture::SetMagFilter(EFilterMode Mode) const
//{
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(Mode));
//}
//
//void XVerse::XOpenGLVolumeTexture::SetWrapR(EWrapMode Mode) const
//{
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, ToGLWrapMode(Mode));
//}
//
//void XVerse::XOpenGLVolumeTexture::SetWrapS(EWrapMode Mode) const
//{
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, ToGLWrapMode(Mode));
//
//}
//
//void XVerse::XOpenGLVolumeTexture::SetWrapT(EWrapMode Mode) const
//{
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, ToGLWrapMode(Mode));
//
//}
//
//void XVerse::XOpenGLVolumeTexture::SetMinLod(uint8_t Lod) const
//{
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_LOD, Lod);
//
//}
//
//void XVerse::XOpenGLVolumeTexture::SetMaxLod(uint8_t Lod) const
//{
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LOD, Lod);
//
//}
//
//void XVerse::XOpenGLVolumeTexture::SetLodBias(float Bias) const
//{
//    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_LOD_BIAS, Bias);
//
//}
//
//void XVerse::XOpenGLVolumeTexture::SetBorderColor(float R, float G, float B, float A) const
//{
//    const GLfloat color[4] = { R,G,B,A };
//    glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, color);
//}
//
//void XVerse::XOpenGLVolumeTexture::SetCompareMode(ETextureCompareMode Mode) const
//{
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_COMPARE_MODE, ToGLTextureCompareMode(Mode));
//}
//
//void XVerse::XOpenGLVolumeTexture::SetCompareFunc(ECompareFunc Func) const
//{
//    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_COMPARE_FUNC, ToGLCompareFunc(Func));
//}

void XVerse::XOpenGLVolumeTexture::SetSamplerState(const XRHISamplerCreateInfo& InSampleInfo)
{
    Bind();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(InSampleInfo.MinFilterMode));

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, ToGLFilterMode(InSampleInfo.MagFilterMode));

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, ToGLWrapMode(InSampleInfo.WrapR));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, ToGLWrapMode(InSampleInfo.WrapS));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, ToGLWrapMode(InSampleInfo.WrapT));

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_LOD, InSampleInfo.MinMipLevel);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LOD, InSampleInfo.MaxMipLevel);

    glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_LOD_BIAS, InSampleInfo.LodBias);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_COMPARE_MODE, ToGLTextureCompareMode(InSampleInfo.CompareMode));
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_COMPARE_FUNC, ToGLCompareFunc(InSampleInfo.CompareFunc));

    glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, (const GLfloat*)&(InSampleInfo.BorderColor[0]));
}

XVerse::XOpenGLTextureCube::~XOpenGLTextureCube()
{
    ReleaseRHI();
}

bool XVerse::XOpenGLTextureCube::IsValid()
{
    Bind();
    return glIsTexture(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLTextureCube::InitRHI()
{
    glGenTextures(1, &Handle);
    Bind();
    for (size_t i = 0; i < MipLevels; i++)
    {
        auto WriteFormat = MatchInternalFormat(InternalFormat);
        SetPixels(i, WriteFormat.first, WriteFormat.second, nullptr);
    }

    //SetSamplerState(XRHISamplerCreateInfo());

    return glIsTexture(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLTextureCube::ReleaseRHI()
{
    glDeleteTextures(1, &Handle);
    return true;
}

bool XVerse::XOpenGLTextureCube::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool XVerse::XOpenGLTextureCube::UpdateRHI()
{
    return true;
}

void XVerse::XOpenGLTextureCube::Bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, Handle);
}

void XVerse::XOpenGLTextureCube::UnBind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void XVerse::XOpenGLTextureCube::ReadPixels(int level, EFormat format, EDataType type, void* pixels)
{
    int pixel_offset = (SizeX / powf(2, level)) * (SizeX / powf(2, level)) * GetRHI()->GetFormatCompNum(format) * GetRHI()->GetDataTypeSize(type);

    std::vector<std::byte> data[6];
    for (size_t i = 0; i < 6; i++)
    {
        data[i].resize(pixel_offset);
        ReadPixels((ECubeFace)i, level, format, type, data[i].data());
    }
    std::byte* p_buffer = (std::byte*)pixels;

    for (size_t i = 0; i < 6; i++)
    {
        memcpy(p_buffer, data[i].data(), data[i].size());
        p_buffer += pixel_offset;
    }
    assert(glGetError() == GL_NO_ERROR);
}

void XVerse::XOpenGLTextureCube::SetPixels(uint32_t level, EFormat format, EDataType type, const void* pixels)
{
    std::byte* p_buffer = (std::byte*)pixels;
    int pixel_offset = SizeX / powf(2, level) * SizeX / powf(2, level) * GetRHI()->GetFormatCompNum(format) * GetRHI()->GetDataTypeSize(type);
    for (size_t i = 0; i < 6; i++)
    {
        SetPixels((ECubeFace)i, level, format, type, pixels ? p_buffer + pixel_offset : nullptr);
    }
}

void XVerse::XOpenGLTextureCube::ReadPixels(ECubeFace face, int level, EFormat format, EDataType type, void* pixels)
{
    //XRHI_ASSERT_ERROR("{0}:{1}",__FILE__,__LINE__)
        Bind();
    glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, ToGLFormat(format), ToGLDataType(type), pixels);
    //XRHI_ASSERT_ERROR("{0}:{1}",__FILE__,__LINE__)
}

void XVerse::XOpenGLTextureCube::SetPixels(ECubeFace face, uint32_t level, EFormat format, EDataType type, const void* pixels)
{
    //XRHI_ASSERT_ERROR("{0}:{1}",__FILE__,__LINE__)
        Bind();
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, ToGLInternalFormat(InternalFormat), SizeX / powf(2, level), SizeX / powf(2, level), 0, ToGLFormat(format), ToGLDataType(type), pixels);
    //XRHI_ASSERT_ERROR("{0}:{1}",__FILE__,__LINE__)
}

void XVerse::XOpenGLTextureCube::SetSamplerState(const XRHISamplerCreateInfo& InSampleInfo)
{
    Bind();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(InSampleInfo.MinFilterMode));

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, ToGLFilterMode(InSampleInfo.MagFilterMode));

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, ToGLWrapMode(InSampleInfo.WrapR));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, ToGLWrapMode(InSampleInfo.WrapS));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, ToGLWrapMode(InSampleInfo.WrapT));

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_LOD, InSampleInfo.MinMipLevel);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LOD, InSampleInfo.MaxMipLevel);

    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_LOD_BIAS, InSampleInfo.LodBias);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, ToGLTextureCompareMode(InSampleInfo.CompareMode));
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, ToGLCompareFunc(InSampleInfo.CompareFunc));

    glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, (const GLfloat*)&(InSampleInfo.BorderColor[0]));
}
