#include "OpenGLVertexArray.h"

uint32_t GetBufferElementComponentNum(XVerse::BufferElementDataType type)
{
	switch (type)
	{

	case XVerse::BufferElementDataType::Bool1: return 1;
	case XVerse::BufferElementDataType::Bool2: return 2;
	case XVerse::BufferElementDataType::Bool3: return 3;
	case XVerse::BufferElementDataType::Bool4: return 4;
	case XVerse::BufferElementDataType::Float1:return 1;
	case XVerse::BufferElementDataType::Float2:return 2;
	case XVerse::BufferElementDataType::Float3:return 3;
	case XVerse::BufferElementDataType::Float4:return 4;
	case XVerse::BufferElementDataType::UInt1: return 1;
	case XVerse::BufferElementDataType::UInt2: return 2;
	case XVerse::BufferElementDataType::UInt3: return 3;
	case XVerse::BufferElementDataType::UInt4: return 4;
	case XVerse::BufferElementDataType::Int1:  return 1;
	case XVerse::BufferElementDataType::Int2:  return 2;
	case XVerse::BufferElementDataType::Int3:  return 3;
	case XVerse::BufferElementDataType::Int4:  return 4;
	case XVerse::BufferElementDataType::Mat2:  return 2 * 2;
	case XVerse::BufferElementDataType::Mat3:  return 3 * 3;
	case XVerse::BufferElementDataType::Mat4:  return 4 * 4;
	case XVerse::BufferElementDataType::Max:
	default:
		break;
	}
	return 0;
}

uint32_t GetBufferElementDataTypeSize(XVerse::BufferElementDataType type)
{
	switch (type)
	{

	case XVerse::BufferElementDataType::Bool1: return 1 * 1;
	case XVerse::BufferElementDataType::Bool2: return 1 * 2;
	case XVerse::BufferElementDataType::Bool3: return 1 * 3;
	case XVerse::BufferElementDataType::Bool4: return 1 * 4;
	case XVerse::BufferElementDataType::Float1:return 4 * 1;
	case XVerse::BufferElementDataType::Float2:return 4 * 2;
	case XVerse::BufferElementDataType::Float3:return 4 * 3;
	case XVerse::BufferElementDataType::Float4:return 4 * 4;
	case XVerse::BufferElementDataType::UInt1: return 4 * 1;
	case XVerse::BufferElementDataType::UInt2: return 4 * 2;
	case XVerse::BufferElementDataType::UInt3: return 4 * 3;
	case XVerse::BufferElementDataType::UInt4: return 4 * 4;
	case XVerse::BufferElementDataType::Int1:  return 4 * 1;
	case XVerse::BufferElementDataType::Int2:  return 4 * 2;
	case XVerse::BufferElementDataType::Int3:  return 4 * 3;
	case XVerse::BufferElementDataType::Int4:  return 4 * 4;
	case XVerse::BufferElementDataType::Mat2:  return 4 * 2 * 2;
	case XVerse::BufferElementDataType::Mat3:  return 4 * 3 * 3;
	case XVerse::BufferElementDataType::Mat4:  return 4 * 4 * 4;
	case XVerse::BufferElementDataType::Max:
	default:
		break;
	}
	return 0;
}

uint32_t BufferElementDataTypeToOpenGLBaseType(XVerse::BufferElementDataType type)
{
	switch (type)
	{

	case XVerse::BufferElementDataType::Bool1: return GL_BOOL;
	case XVerse::BufferElementDataType::Bool2: return GL_BOOL;
	case XVerse::BufferElementDataType::Bool3: return GL_BOOL;
	case XVerse::BufferElementDataType::Bool4: return GL_BOOL;
	case XVerse::BufferElementDataType::Float1:return GL_FLOAT;
	case XVerse::BufferElementDataType::Float2:return GL_FLOAT;
	case XVerse::BufferElementDataType::Float3:return GL_FLOAT;
	case XVerse::BufferElementDataType::Float4:return GL_FLOAT;
	case XVerse::BufferElementDataType::UInt1: return GL_UNSIGNED_INT;
	case XVerse::BufferElementDataType::UInt2: return GL_UNSIGNED_INT;
	case XVerse::BufferElementDataType::UInt3: return GL_UNSIGNED_INT;
	case XVerse::BufferElementDataType::UInt4: return GL_UNSIGNED_INT;
	case XVerse::BufferElementDataType::Int1:  return GL_INT;
	case XVerse::BufferElementDataType::Int2:  return GL_INT;
	case XVerse::BufferElementDataType::Int3:  return GL_INT;
	case XVerse::BufferElementDataType::Int4:  return GL_INT;
	case XVerse::BufferElementDataType::Mat2:  return GL_FLOAT;
	case XVerse::BufferElementDataType::Mat3:  return GL_FLOAT;
	case XVerse::BufferElementDataType::Mat4:  return GL_FLOAT;
	case XVerse::BufferElementDataType::Max:
	default:
		break;
	}
	return 0;
}

XVerse::XOpenGLVertexArray::~XOpenGLVertexArray()
{
    ReleaseRHI();
}

bool XVerse::XOpenGLVertexArray::IsValid()
{
    Bind();
    return glIsVertexArray(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLVertexArray::InitRHI()
{
    glGenVertexArrays(1, &Handle);
    return IsValid();
}

bool XVerse::XOpenGLVertexArray::ReleaseRHI()
{
    glDeleteVertexArrays(1, &Handle);
    return true;
}

bool XVerse::XOpenGLVertexArray::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool XVerse::XOpenGLVertexArray::UpdateRHI()
{
    return true;
}

void XVerse::XOpenGLVertexArray::Bind() const
{
    glBindVertexArray(Handle);
}

void XVerse::XOpenGLVertexArray::UnBind() const
{
    glBindVertexArray(0);
}

void XVerse::XOpenGLVertexArray::Setup(std::shared_ptr<XRHIBuffer>IndexBuffer, std::shared_ptr<XRHIBuffer>VertexBuffer,const BufferLayout& VertexLayout)
{
    if (IndexBuffer && VertexBuffer && IndexBuffer->IsValid() && VertexBuffer->IsValid())
    {
        Bind();
        IndexBuffer->Bind();
        VertexBuffer->Bind();

        uint32_t index = 0;
        const auto& layout = VertexLayout;
        for (const auto& ele : layout)
        {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                GetBufferElementComponentNum(ele.Type),
                BufferElementDataTypeToOpenGLBaseType(ele.Type),
                ele.bIsNormalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)ele.Offset);
            ++index;
        }

        UnBind();
    }
}
