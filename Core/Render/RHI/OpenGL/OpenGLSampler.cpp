#include "OpenGLSampler.h"
#include <Render/RHI/OpenGL/OpenGLRHI.h>
XVerse::XOpenGLSampler::~XOpenGLSampler()
{
    ReleaseRHI();
}

bool XVerse::XOpenGLSampler::IsValid()
{
    return glIsSampler(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLSampler::InitRHI()
{
    glGenSamplers(1, &Handle);
    return IsValid();
}

bool XVerse::XOpenGLSampler::ReleaseRHI()
{
    glDeleteSamplers(1, &Handle);
    return true;
}

bool XVerse::XOpenGLSampler::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool XVerse::XOpenGLSampler::UpdateRHI()
{
    return true;
}

void XVerse::XOpenGLSampler::Bind(uint32_t unit)
{
    glBindSampler(unit, Handle);

}

void XVerse::XOpenGLSampler::UnBind(uint32_t unit)
{
    glBindSampler(unit, 0);

}

void XVerse::XOpenGLSampler::SetSamplerState(const XRHISamplerCreateInfo& InSampleInfo)
{
    glSamplerParameteri(Handle, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(InSampleInfo.MinFilterMode));
    glSamplerParameteri(Handle, GL_TEXTURE_MAG_FILTER, ToGLFilterMode(InSampleInfo.MagFilterMode));

    glSamplerParameteri(Handle, GL_TEXTURE_WRAP_R, ToGLWrapMode(InSampleInfo.WrapR));
    glSamplerParameteri(Handle, GL_TEXTURE_WRAP_S, ToGLWrapMode(InSampleInfo.WrapS));
    glSamplerParameteri(Handle, GL_TEXTURE_WRAP_T, ToGLWrapMode(InSampleInfo.WrapT));

    glSamplerParameteri(Handle, GL_TEXTURE_MIN_LOD, InSampleInfo.MinMipLevel);
    glSamplerParameteri(Handle, GL_TEXTURE_MAX_LOD, InSampleInfo.MaxMipLevel);

    glSamplerParameterf(Handle, GL_TEXTURE_LOD_BIAS, InSampleInfo.LodBias);

    glSamplerParameteri(Handle, GL_TEXTURE_COMPARE_MODE, ToGLTextureCompareMode(InSampleInfo.CompareMode));
    glSamplerParameteri(Handle, GL_TEXTURE_COMPARE_FUNC, ToGLCompareFunc(InSampleInfo.CompareFunc));

    glSamplerParameterfv(Handle, GL_TEXTURE_BORDER_COLOR, (const GLfloat*)&(InSampleInfo.BorderColor[0]));
}
//
//
//void XVerse::XOpenGLSampler::SetMinFilter(EFilterMode Mode) const
//{
//    
//}
//
//void XVerse::XOpenGLSampler::SetMagFilter(EFilterMode Mode) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(Mode));
//}
//
//void XVerse::XOpenGLSampler::SetWrapR(EWrapMode Mode) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_WRAP_R, ToGLWrapMode(Mode));
//}
//
//void XVerse::XOpenGLSampler::SetWrapS(EWrapMode Mode) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_WRAP_S, ToGLWrapMode(Mode));
//
//}
//
//void XVerse::XOpenGLSampler::SetWrapT(EWrapMode Mode) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_WRAP_T, ToGLWrapMode(Mode));
//
//}
//
//void XVerse::XOpenGLSampler::SetMinLod(uint8_t Lod) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_MIN_LOD, Lod);
//
//}
//
//void XVerse::XOpenGLSampler::SetMaxLod(uint8_t Lod) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_MAX_LOD, Lod);
//
//}
//
//void XVerse::XOpenGLSampler::SetLodBias(float Bias) const
//{
//    glSamplerParameterf(Handle, GL_TEXTURE_LOD_BIAS, Bias);
//
//}
//
//void XVerse::XOpenGLSampler::SetBorderColor(float R, float G, float B, float A) const
//{
//    const GLfloat color[4] = { R,G,B,A };
//    glSamplerParameterfv(Handle, GL_TEXTURE_BORDER_COLOR, color);
//
//}
//
//void XVerse::XOpenGLSampler::SetCompareMode(ETextureCompareMode Mode) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_COMPARE_MODE, ToGLTextureCompareMode(Mode));
//}
//
//void XVerse::XOpenGLSampler::SetCompareFunc(ECompareFunc Func) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_COMPARE_FUNC, ToGLCompareFunc(Func));
//}
