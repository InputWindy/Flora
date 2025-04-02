#include "OpenGLShaderProgram.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"

XVerse::XOpenGLShaderProgram::~XOpenGLShaderProgram()
{
	ReleaseRHI();
}

bool XVerse::XOpenGLShaderProgram::IsValid()
{
    return glIsProgram(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLShaderProgram::InitRHI()
{
    Handle = glCreateProgram();
    return IsValid();
}

bool XVerse::XOpenGLShaderProgram::ReleaseRHI()
{
    glDeleteProgram(Handle);
    return true;
}

bool XVerse::XOpenGLShaderProgram::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool XVerse::XOpenGLShaderProgram::UpdateRHI()
{
    return Link();
}

void XVerse::XOpenGLShaderProgram::AttachShader(std::shared_ptr<XRHIShader> Shader) const
{
	if (!Shader)return;
    glAttachShader(Handle, std::dynamic_pointer_cast<XOpenGLShader>(Shader)->GetHandle());
}

bool XVerse::XOpenGLShaderProgram::Link() const
{
    glLinkProgram(Handle);
    return IsLinked();
}

bool XVerse::XOpenGLShaderProgram::IsLinked() const
{
    GLint linkStatus;
    glGetProgramiv(Handle, GL_LINK_STATUS, &linkStatus);
    return linkStatus == GL_TRUE;
}

void XVerse::XOpenGLShaderProgram::Use() const
{
    glUseProgram(Handle);
}

void XVerse::XOpenGLShaderProgram::GetShaderProgramInfoLog(std::string& out) const
{
    if (IsLinked() == false)
    {
        GLint logLength;
        glGetProgramiv(Handle, GL_INFO_LOG_LENGTH, &logLength);

        if (logLength > 0) 
        {
            std::vector<char> log(logLength + 1);
            glGetProgramInfoLog(Handle, logLength, NULL, log.data());
            out = log.data();
        }
    }
}

void XVerse::XOpenGLShaderProgram::SetBuffer(std::shared_ptr<XVerse::XRHIBuffer>buffer) const
{
	if (buffer)buffer->BindBufferBase();
}

void XVerse::XOpenGLShaderProgram::SetTexture(const char* name, uint32_t tex_unit, std::shared_ptr<class XRHITexture>texture, std::shared_ptr<class XRHISampler>sampler) const
{
	glActiveTexture(GL_TEXTURE0 + tex_unit);
	assert(glGetError() == GL_NO_ERROR);
	if (sampler)sampler->Bind(tex_unit);
	assert(glGetError() == GL_NO_ERROR);
	texture->Bind();
	SetInt1(name, tex_unit);
}

void XVerse::XOpenGLShaderProgram::SetTextureImage(std::shared_ptr<class XRHITexture>texture, uint32_t image_unit, uint32_t mip, uint32_t access_policy) const
{
	if (texture)
	{
		texture->Bind();
		if( texture->GetType() == ERHIResourceType::RT_Texture2D){
		
			glBindImageTexture(image_unit, texture->DynamicCast<XVerse::XOpenGLTexture2D>()->GetHandle(), mip, GL_FALSE, 0, access_policy, ToGLInternalFormat(texture->DynamicCast<XVerse::XOpenGLTexture2D>()->GetInternalFormat()));	
		}
		if( texture->GetType() == ERHIResourceType::RT_Texture3D){
		
			glBindImageTexture(image_unit, texture->DynamicCast<XVerse::XOpenGLVolumeTexture>()->GetHandle(), mip, GL_FALSE, 0, access_policy, ToGLInternalFormat(texture->DynamicCast<XVerse::XOpenGLVolumeTexture>()->GetInternalFormat()));	
		}
	}
}

void XVerse::XOpenGLShaderProgram::SetTextureImage(std::shared_ptr<class XRHITexture>texture, uint32_t image_unit, uint32_t mip, EAccessPolicy access_policy) const
{
	SetTextureImage(texture, image_unit, mip, ToGLAccessPolicy(access_policy));
}

void XVerse::XOpenGLShaderProgram::SetBool1(const char* name, bool data) const
{
	glUniform1i(glGetUniformLocation(Handle, name), data ? 1 : 0);
}

void XVerse::XOpenGLShaderProgram::SetBool2(const char* name, bool data1, bool data2) const
{
	glUniform2i(glGetUniformLocation(Handle, name), data1 ? 1 : 0, data2 ? 1 : 0);
}

void XVerse::XOpenGLShaderProgram::SetBool3(const char* name, bool data1, bool data2, bool data3) const
{
	glUniform3i(glGetUniformLocation(Handle, name), data1 ? 1 : 0, data2 ? 1 : 0, data3 ? 1 : 0);
}

void XVerse::XOpenGLShaderProgram::SetBool4(const char* name, bool data1, bool data2, bool data3, bool data4) const
{
	glUniform4i(glGetUniformLocation(Handle, name), data1 ? 1 : 0, data2 ? 1 : 0, data3 ? 1 : 0, data4 ? 1 : 0);
}

void XVerse::XOpenGLShaderProgram::SetFloat1(const char* name, float data) const
{
	glUniform1f(glGetUniformLocation(Handle, name), data);
}

void XVerse::XOpenGLShaderProgram::SetFloat2(const char* name, float data1, float data2) const
{
	glUniform2f(glGetUniformLocation(Handle, name), data1, data2);
}

void XVerse::XOpenGLShaderProgram::SetFloat3(const char* name, float data1, float data2, float data3) const
{
	glUniform3f(glGetUniformLocation(Handle, name), data1, data2, data3);
}

void XVerse::XOpenGLShaderProgram::SetFloat4(const char* name, float data1, float data2, float data3, float data4) const
{
	glUniform4f(glGetUniformLocation(Handle, name), data1, data2, data3, data4);
}

void XVerse::XOpenGLShaderProgram::SetFloat1Array(const char* name, size_t cnt, float* data) const
{
	glUniform1fv(glGetUniformLocation(Handle, name), cnt, data);
}

void XVerse::XOpenGLShaderProgram::SetFloat2Array(const char* name, size_t cnt, float* data) const
{
	glUniform2fv(glGetUniformLocation(Handle, name), cnt, data);
}

void XVerse::XOpenGLShaderProgram::SetFloat3Array(const char* name, size_t cnt, float* data) const
{
	glUniform3fv(glGetUniformLocation(Handle, name), cnt, data);
}

void XVerse::XOpenGLShaderProgram::SetFloat4Array(const char* name, size_t cnt, float* data) const
{
	glUniform4fv(glGetUniformLocation(Handle, name), cnt, data);
}

void XVerse::XOpenGLShaderProgram::SetInt1Array(const char* name, size_t cnt, int* data) const
{
	glUniform1iv(glGetUniformLocation(Handle, name), cnt, data);
}

void XVerse::XOpenGLShaderProgram::SetInt2Array(const char* name, size_t cnt, int* data) const
{
	glUniform2iv(glGetUniformLocation(Handle, name), cnt, data);
}

void XVerse::XOpenGLShaderProgram::SetInt3Array(const char* name, size_t cnt, int* data) const
{
	glUniform3iv(glGetUniformLocation(Handle, name), cnt, data);
}

void XVerse::XOpenGLShaderProgram::SetInt4Array(const char* name, size_t cnt, int* data) const
{
	glUniform4iv(glGetUniformLocation(Handle, name), cnt, data);
}

void XVerse::XOpenGLShaderProgram::SetUInt1Array(const char* name, size_t cnt, uint32_t* data) const
{
	glUniform1uiv(glGetUniformLocation(Handle, name), cnt, data);
}

void XVerse::XOpenGLShaderProgram::SetUInt2Array(const char* name, size_t cnt, uint32_t* data) const
{
	glUniform2uiv(glGetUniformLocation(Handle, name), cnt, data);
}

void XVerse::XOpenGLShaderProgram::SetUInt3Array(const char* name, size_t cnt, uint32_t* data) const
{
	glUniform3uiv(glGetUniformLocation(Handle, name), cnt, data);
}

void XVerse::XOpenGLShaderProgram::SetUInt4Array(const char* name, size_t cnt, uint32_t* data) const
{
	glUniform4uiv(glGetUniformLocation(Handle, name), cnt, data);
}

void XVerse::XOpenGLShaderProgram::SetMatrixFloat2x2Array(const char* name, size_t cnt, bool transpose, float* data) const
{
	glUniformMatrix2fv(glGetUniformLocation(Handle, name), cnt, transpose ? GL_TRUE : GL_FALSE, data);
}

void XVerse::XOpenGLShaderProgram::SetMatrixFloat3x3Array(const char* name, size_t cnt, bool transpose, float* data) const
{
	glUniformMatrix3fv(glGetUniformLocation(Handle, name), cnt, transpose ? GL_TRUE : GL_FALSE, data);
}

void XVerse::XOpenGLShaderProgram::SetMatrixFloat4x4Array(const char* name, size_t cnt, bool transpose, float* data) const
{
	glUniformMatrix4fv(glGetUniformLocation(Handle, name), cnt, transpose ? GL_TRUE : GL_FALSE, data);
}

void XVerse::XOpenGLShaderProgram::SetMatrixFloat2x3Array(const char* name, size_t cnt, bool transpose, float* data) const
{
	glUniformMatrix2x3fv(glGetUniformLocation(Handle, name), cnt, transpose ? GL_TRUE : GL_FALSE, data);
}

void XVerse::XOpenGLShaderProgram::SetMatrixFloat3x2Array(const char* name, size_t cnt, bool transpose, float* data) const
{
	glUniformMatrix3x2fv(glGetUniformLocation(Handle, name), cnt, transpose ? GL_TRUE : GL_FALSE, data);
}

void XVerse::XOpenGLShaderProgram::SetMatrixFloat2x4Array(const char* name, size_t cnt, bool transpose, float* data) const
{
	glUniformMatrix2x4fv(glGetUniformLocation(Handle, name), cnt, transpose ? GL_TRUE : GL_FALSE, data);
}

void XVerse::XOpenGLShaderProgram::SetMatrixFloat4x2Array(const char* name, size_t cnt, bool transpose, float* data) const
{
	glUniformMatrix4x2fv(glGetUniformLocation(Handle, name), cnt, transpose ? GL_TRUE : GL_FALSE, data);
}

void XVerse::XOpenGLShaderProgram::SetMatrixFloat3x4Array(const char* name, size_t cnt, bool transpose, float* data) const
{
	glUniformMatrix3x4fv(glGetUniformLocation(Handle, name), cnt, transpose ? GL_TRUE : GL_FALSE, data);
}

void XVerse::XOpenGLShaderProgram::SetMatrixFloat4x3Array(const char* name, size_t cnt, bool transpose, float* data) const
{
	glUniformMatrix4x3fv(glGetUniformLocation(Handle, name), cnt, transpose ? GL_TRUE : GL_FALSE, data);
}

void XVerse::XOpenGLShaderProgram::SetInt1(const char* name, int data1) const
{
	glUniform1i(glGetUniformLocation(Handle, name), data1);
}

void XVerse::XOpenGLShaderProgram::SetInt2(const char* name, int data1, int data2) const
{
	glUniform2i(glGetUniformLocation(Handle, name), data1, data2);
}

void XVerse::XOpenGLShaderProgram::SetInt3(const char* name, int data1, int data2, int data3) const
{
	glUniform3i(glGetUniformLocation(Handle, name), data1, data2, data3);
}

void XVerse::XOpenGLShaderProgram::SetInt4(const char* name, int data1, int data2, int data3, int data4) const
{
	glUniform4i(glGetUniformLocation(Handle, name), data1, data2, data3, data4);
}

void XVerse::XOpenGLShaderProgram::SetUInt1(const char* name, uint32_t data1) const
{
	glUniform1ui(glGetUniformLocation(Handle, name), data1);
}

void XVerse::XOpenGLShaderProgram::SetUInt2(const char* name, uint32_t data1, uint32_t data2) const
{
	glUniform2ui(glGetUniformLocation(Handle, name), data1, data2);
}

void XVerse::XOpenGLShaderProgram::SetUInt3(const char* name, uint32_t data1, uint32_t data2, uint32_t data3) const
{
	glUniform3ui(glGetUniformLocation(Handle, name), data1, data2, data3);
}

void XVerse::XOpenGLShaderProgram::SetUInt4(const char* name, uint32_t data1, uint32_t data2, uint32_t data3, uint32_t data4) const
{
	glUniform4ui(glGetUniformLocation(Handle, name), data1, data2, data3, data4);
}
