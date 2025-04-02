#include "OpenGLVertexArray.h"

uint32_t GetBufferElementComponentNum(flora::BufferElementDataType type)
{
	switch (type)
	{

	case flora::BufferElementDataType::Bool1: return 1;
	case flora::BufferElementDataType::Bool2: return 2;
	case flora::BufferElementDataType::Bool3: return 3;
	case flora::BufferElementDataType::Bool4: return 4;
	case flora::BufferElementDataType::Float1:return 1;
	case flora::BufferElementDataType::Float2:return 2;
	case flora::BufferElementDataType::Float3:return 3;
	case flora::BufferElementDataType::Float4:return 4;
	case flora::BufferElementDataType::UInt1: return 1;
	case flora::BufferElementDataType::UInt2: return 2;
	case flora::BufferElementDataType::UInt3: return 3;
	case flora::BufferElementDataType::UInt4: return 4;
	case flora::BufferElementDataType::Int1:  return 1;
	case flora::BufferElementDataType::Int2:  return 2;
	case flora::BufferElementDataType::Int3:  return 3;
	case flora::BufferElementDataType::Int4:  return 4;
	case flora::BufferElementDataType::Mat2:  return 2 * 2;
	case flora::BufferElementDataType::Mat3:  return 3 * 3;
	case flora::BufferElementDataType::Mat4:  return 4 * 4;
	case flora::BufferElementDataType::Max:
	default:
		break;
	}
	return 0;
}

uint32_t GetBufferElementDataTypeSize(flora::BufferElementDataType type)
{
	switch (type)
	{

	case flora::BufferElementDataType::Bool1: return 1 * 1;
	case flora::BufferElementDataType::Bool2: return 1 * 2;
	case flora::BufferElementDataType::Bool3: return 1 * 3;
	case flora::BufferElementDataType::Bool4: return 1 * 4;
	case flora::BufferElementDataType::Float1:return 4 * 1;
	case flora::BufferElementDataType::Float2:return 4 * 2;
	case flora::BufferElementDataType::Float3:return 4 * 3;
	case flora::BufferElementDataType::Float4:return 4 * 4;
	case flora::BufferElementDataType::UInt1: return 4 * 1;
	case flora::BufferElementDataType::UInt2: return 4 * 2;
	case flora::BufferElementDataType::UInt3: return 4 * 3;
	case flora::BufferElementDataType::UInt4: return 4 * 4;
	case flora::BufferElementDataType::Int1:  return 4 * 1;
	case flora::BufferElementDataType::Int2:  return 4 * 2;
	case flora::BufferElementDataType::Int3:  return 4 * 3;
	case flora::BufferElementDataType::Int4:  return 4 * 4;
	case flora::BufferElementDataType::Mat2:  return 4 * 2 * 2;
	case flora::BufferElementDataType::Mat3:  return 4 * 3 * 3;
	case flora::BufferElementDataType::Mat4:  return 4 * 4 * 4;
	case flora::BufferElementDataType::Max:
	default:
		break;
	}
	return 0;
}

uint32_t BufferElementDataTypeToOpenGLBaseType(flora::BufferElementDataType type)
{
	switch (type)
	{

	case flora::BufferElementDataType::Bool1: return GL_BOOL;
	case flora::BufferElementDataType::Bool2: return GL_BOOL;
	case flora::BufferElementDataType::Bool3: return GL_BOOL;
	case flora::BufferElementDataType::Bool4: return GL_BOOL;
	case flora::BufferElementDataType::Float1:return GL_FLOAT;
	case flora::BufferElementDataType::Float2:return GL_FLOAT;
	case flora::BufferElementDataType::Float3:return GL_FLOAT;
	case flora::BufferElementDataType::Float4:return GL_FLOAT;
	case flora::BufferElementDataType::UInt1: return GL_UNSIGNED_INT;
	case flora::BufferElementDataType::UInt2: return GL_UNSIGNED_INT;
	case flora::BufferElementDataType::UInt3: return GL_UNSIGNED_INT;
	case flora::BufferElementDataType::UInt4: return GL_UNSIGNED_INT;
	case flora::BufferElementDataType::Int1:  return GL_INT;
	case flora::BufferElementDataType::Int2:  return GL_INT;
	case flora::BufferElementDataType::Int3:  return GL_INT;
	case flora::BufferElementDataType::Int4:  return GL_INT;
	case flora::BufferElementDataType::Mat2:  return GL_FLOAT;
	case flora::BufferElementDataType::Mat3:  return GL_FLOAT;
	case flora::BufferElementDataType::Mat4:  return GL_FLOAT;
	case flora::BufferElementDataType::Max:
	default:
		break;
	}
	return 0;
}

flora::XOpenGLVertexArray::~XOpenGLVertexArray()
{
    ReleaseRHI();
}

bool flora::XOpenGLVertexArray::IsValid()
{
    Bind();
    return glIsVertexArray(Handle) == GL_TRUE;
}

bool flora::XOpenGLVertexArray::InitRHI()
{
    glGenVertexArrays(1, &Handle);
    return IsValid();
}

bool flora::XOpenGLVertexArray::ReleaseRHI()
{
    glDeleteVertexArrays(1, &Handle);
    return true;
}

bool flora::XOpenGLVertexArray::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool flora::XOpenGLVertexArray::UpdateRHI()
{
    return true;
}

void flora::XOpenGLVertexArray::Bind() const
{
    glBindVertexArray(Handle);
}

void flora::XOpenGLVertexArray::UnBind() const
{
    glBindVertexArray(0);
}

void flora::XOpenGLVertexArray::Setup(std::shared_ptr<XRHIBuffer>IndexBuffer, std::shared_ptr<XRHIBuffer>VertexBuffer,const BufferLayout& VertexLayout)
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
