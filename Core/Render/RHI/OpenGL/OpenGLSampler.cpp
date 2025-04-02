#include "OpenGLSampler.h"
#include <Render/RHI/OpenGL/OpenGLRHI.h>
flora::XOpenGLSampler::~XOpenGLSampler()
{
    ReleaseRHI();
}

bool flora::XOpenGLSampler::IsValid()
{
    return glIsSampler(Handle) == GL_TRUE;
}

bool flora::XOpenGLSampler::InitRHI()
{
    glGenSamplers(1, &Handle);
    return IsValid();
}

bool flora::XOpenGLSampler::ReleaseRHI()
{
    glDeleteSamplers(1, &Handle);
    return true;
}

bool flora::XOpenGLSampler::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool flora::XOpenGLSampler::UpdateRHI()
{
    return true;
}

void flora::XOpenGLSampler::Bind(uint32_t unit)
{
    glBindSampler(unit, Handle);

}

void flora::XOpenGLSampler::UnBind(uint32_t unit)
{
    glBindSampler(unit, 0);

}

void flora::XOpenGLSampler::SetSamplerState(const XRHISamplerCreateInfo& InSampleInfo)
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
//void flora::XOpenGLSampler::SetMinFilter(EFilterMode Mode) const
//{
//    
//}
//
//void flora::XOpenGLSampler::SetMagFilter(EFilterMode Mode) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_MIN_FILTER, ToGLFilterMode(Mode));
//}
//
//void flora::XOpenGLSampler::SetWrapR(EWrapMode Mode) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_WRAP_R, ToGLWrapMode(Mode));
//}
//
//void flora::XOpenGLSampler::SetWrapS(EWrapMode Mode) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_WRAP_S, ToGLWrapMode(Mode));
//
//}
//
//void flora::XOpenGLSampler::SetWrapT(EWrapMode Mode) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_WRAP_T, ToGLWrapMode(Mode));
//
//}
//
//void flora::XOpenGLSampler::SetMinLod(uint8_t Lod) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_MIN_LOD, Lod);
//
//}
//
//void flora::XOpenGLSampler::SetMaxLod(uint8_t Lod) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_MAX_LOD, Lod);
//
//}
//
//void flora::XOpenGLSampler::SetLodBias(float Bias) const
//{
//    glSamplerParameterf(Handle, GL_TEXTURE_LOD_BIAS, Bias);
//
//}
//
//void flora::XOpenGLSampler::SetBorderColor(float R, float G, float B, float A) const
//{
//    const GLfloat color[4] = { R,G,B,A };
//    glSamplerParameterfv(Handle, GL_TEXTURE_BORDER_COLOR, color);
//
//}
//
//void flora::XOpenGLSampler::SetCompareMode(ETextureCompareMode Mode) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_COMPARE_MODE, ToGLTextureCompareMode(Mode));
//}
//
//void flora::XOpenGLSampler::SetCompareFunc(ECompareFunc Func) const
//{
//    glSamplerParameteri(Handle, GL_TEXTURE_COMPARE_FUNC, ToGLCompareFunc(Func));
//}
