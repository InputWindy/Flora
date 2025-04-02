#include "OpenGLShader.h"

flora::XOpenGLShader::~XOpenGLShader()
{
    ReleaseRHI();
}

bool flora::XOpenGLShader::IsValid()
{
    return glIsShader(Handle) == GL_TRUE;
}

bool flora::XOpenGLShader::InitRHI()
{
    switch (ShaderType)
    {
    case flora::EShaderType::ST_VERTEX_SHADER:Handle = glCreateShader(GL_VERTEX_SHADER); break;
    case flora::EShaderType::ST_TESS_CONTROL_SHADER:Handle = glCreateShader(GL_TESS_CONTROL_SHADER); break;
    case flora::EShaderType::ST_TESS_EVALUATION_SHADER:Handle = glCreateShader(GL_TESS_EVALUATION_SHADER); break;
    case flora::EShaderType::ST_GEOMETRY_SHADER:Handle = glCreateShader(GL_GEOMETRY_SHADER); break;
    case flora::EShaderType::ST_FRAGMENT_SHADER:Handle = glCreateShader(GL_FRAGMENT_SHADER); break;
    case flora::EShaderType::ST_COMPUTE_SHADER:Handle = glCreateShader(GL_COMPUTE_SHADER); break;
    case flora::EShaderType::ST_MAX_COUNT:
    default:
        break;
    }
    return IsValid();
}

bool flora::XOpenGLShader::ReleaseRHI()
{
    glDeleteShader(Handle);
    return true;
}

bool flora::XOpenGLShader::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool flora::XOpenGLShader::UpdateRHI()
{
    return Compile();
}

void flora::XOpenGLShader::SetSourceCode(const std::string& InSourceCode)
{
    const char* ShaderSource = InSourceCode.c_str();
    glShaderSource(Handle, 1, &ShaderSource, nullptr);
}

void flora::XOpenGLShader::GetShaderInfoLog(std::string& outInfo)
{
    GLint logLength;
    glGetShaderiv(Handle, GL_INFO_LOG_LENGTH, &logLength);

    GLchar* log = new GLchar[logLength];
    glGetShaderInfoLog(Handle, logLength, nullptr, log);

    outInfo = std::string(log, logLength);

    delete[] log;
}

bool flora::XOpenGLShader::Compile()
{
    glCompileShader(Handle);
    return IsCompiled();
}

bool flora::XOpenGLShader::IsCompiled()
{
    GLint compileStatus;
    glGetShaderiv(Handle, GL_COMPILE_STATUS, &compileStatus);
    return compileStatus == GL_TRUE;
}
