#include "OpenGLBuffer.h"
#include <Render/RHI/OpenGL/OpenGLRHI.h>

flora::XOpenGLVertexBuffer::~XOpenGLVertexBuffer()
{
    ReleaseRHI();
}

bool flora::XOpenGLVertexBuffer::IsValid()
{
    Bind();
    return glIsBuffer(Handle) == GL_TRUE;
}

bool flora::XOpenGLVertexBuffer::InitRHI()
{
    glGenBuffers(1, &Handle);
    return IsValid();
}

bool flora::XOpenGLVertexBuffer::ReleaseRHI()
{
    glDeleteBuffers(1, &Handle);
    return true;
}

bool flora::XOpenGLVertexBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool flora::XOpenGLVertexBuffer::UpdateRHI()
{
    return true;
}

void flora::XOpenGLVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, Handle);
}

void flora::XOpenGLVertexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool flora::XOpenGLVertexBuffer::SetData(size_t size, const void* data, EBufferUsage usage)
{
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, flora::ToGLBufferUsage(usage));
    return IsValid();
}

bool flora::XOpenGLVertexBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    return true;
}

void* flora::XOpenGLVertexBuffer::Map(EAccessPolicy policy)
{
    Bind();
    return glMapBuffer(GL_ARRAY_BUFFER, flora::ToGLAccessPolicy(policy));
}

void* flora::XOpenGLVertexBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_ARRAY_BUFFER, offset, length, flora::ToGLAccessPolicy(policy));
}

bool flora::XOpenGLVertexBuffer::Unmap()
{
    Bind();
    return glUnmapBuffer(GL_ARRAY_BUFFER);
}

void flora::XOpenGLVertexBuffer::BindBufferBase()
{
    Bind();
    glBindBufferBase(GL_ARRAY_BUFFER, BindingSlot, Handle);
}


flora::XOpenGLIndexBuffer::~XOpenGLIndexBuffer()
{
    ReleaseRHI();
}

bool flora::XOpenGLIndexBuffer::IsValid()
{
    return glIsBuffer(Handle) == GL_TRUE;
}

bool flora::XOpenGLIndexBuffer::InitRHI()
{
    glGenBuffers(1, &Handle);
    return IsValid();
}

bool flora::XOpenGLIndexBuffer::ReleaseRHI()
{
    glDeleteBuffers(1, &Handle);
    return true;
}

bool flora::XOpenGLIndexBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool flora::XOpenGLIndexBuffer::UpdateRHI()
{
    return true;
}

void flora::XOpenGLIndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Handle);

}

void flora::XOpenGLIndexBuffer::UnBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

bool flora::XOpenGLIndexBuffer::SetData(size_t size, const void* data, EBufferUsage usage)
{
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, flora::ToGLBufferUsage(usage));
    return IsValid();
}

bool flora::XOpenGLIndexBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	return true;
}

void* flora::XOpenGLIndexBuffer::Map(EAccessPolicy policy)
{
    Bind();
    return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, flora::ToGLAccessPolicy(policy));
}

void* flora::XOpenGLIndexBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, length, flora::ToGLAccessPolicy(policy));
}

bool flora::XOpenGLIndexBuffer::Unmap()
{
    Bind();
    return glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void flora::XOpenGLIndexBuffer::BindBufferBase()
{
    Bind();
    glBindBufferBase(GL_ELEMENT_ARRAY_BUFFER, BindingSlot, Handle);
}

flora::XOpenGLUniformBuffer::~XOpenGLUniformBuffer()
{
    ReleaseRHI();
}

bool flora::XOpenGLUniformBuffer::IsValid()
{
    return glIsBuffer(Handle) == GL_TRUE;
}

bool flora::XOpenGLUniformBuffer::InitRHI()
{
    glGenBuffers(1, &Handle);
    Bind();
    return IsValid();
}

bool flora::XOpenGLUniformBuffer::ReleaseRHI()
{
    glDeleteBuffers(1, &Handle);
    return true;
}

bool flora::XOpenGLUniformBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool flora::XOpenGLUniformBuffer::UpdateRHI()
{
    return true;
}

void flora::XOpenGLUniformBuffer::Bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, Handle);

}

void flora::XOpenGLUniformBuffer::UnBind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

bool flora::XOpenGLUniformBuffer::SetData(size_t size, const void* data, EBufferUsage usage)
{
    glBufferData(GL_UNIFORM_BUFFER, size, data, flora::ToGLBufferUsage(usage));
    return IsValid();
}

bool flora::XOpenGLUniformBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	return true;
}

void* flora::XOpenGLUniformBuffer::Map(EAccessPolicy policy)
{
    Bind();
    return glMapBuffer(GL_UNIFORM_BUFFER, flora::ToGLAccessPolicy(policy));
}

void* flora::XOpenGLUniformBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_UNIFORM_BUFFER, offset, length, flora::ToGLAccessPolicy(policy));
}

bool flora::XOpenGLUniformBuffer::Unmap()
{
    Bind();
    return glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void flora::XOpenGLUniformBuffer::BindBufferBase()
{
    Bind();
    glBindBufferBase(GL_UNIFORM_BUFFER, BindingSlot, Handle);
}

flora::XOpenGLShaderStorageBuffer::~XOpenGLShaderStorageBuffer()
{
    ReleaseRHI();
}

bool flora::XOpenGLShaderStorageBuffer::IsValid()
{
    return glIsBuffer(Handle) == GL_TRUE;
}

bool flora::XOpenGLShaderStorageBuffer::InitRHI()
{
    glGenBuffers(1, &Handle);
    Bind();
    return IsValid();
}

bool flora::XOpenGLShaderStorageBuffer::ReleaseRHI()
{
    glDeleteBuffers(1, &Handle);
    return true;
}

bool flora::XOpenGLShaderStorageBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool flora::XOpenGLShaderStorageBuffer::UpdateRHI()
{
    return true;
}

void flora::XOpenGLShaderStorageBuffer::Bind()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, Handle);

}

void flora::XOpenGLShaderStorageBuffer::UnBind()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

bool flora::XOpenGLShaderStorageBuffer::SetData(size_t size, const void*data, EBufferUsage usage)
{
    Bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, flora::ToGLBufferUsage(usage));
    return IsValid();
}

bool flora::XOpenGLShaderStorageBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
	return true;
}

void* flora::XOpenGLShaderStorageBuffer::Map(EAccessPolicy policy)
{
    Bind();
    return glMapBuffer(GL_SHADER_STORAGE_BUFFER, flora::ToGLAccessPolicy(policy));
}

void* flora::XOpenGLShaderStorageBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_SHADER_STORAGE_BUFFER, offset,length, flora::ToGLAccessPolicy(policy));
}

bool flora::XOpenGLShaderStorageBuffer::Unmap()
{
    Bind();
    return glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void flora::XOpenGLShaderStorageBuffer::BindBufferBase()
{
    Bind();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BindingSlot, Handle);
}

//==========================//

flora::XOpenGLDrawIndirectBuffer::~XOpenGLDrawIndirectBuffer()
{
    ReleaseRHI();
}

bool flora::XOpenGLDrawIndirectBuffer::IsValid()
{
    return glIsBuffer(Handle) == GL_TRUE;
}

bool flora::XOpenGLDrawIndirectBuffer::InitRHI()
{
    glGenBuffers(1, &Handle);
    Bind();
    return IsValid();
}

bool flora::XOpenGLDrawIndirectBuffer::ReleaseRHI()
{
    glDeleteBuffers(1, &Handle);
    return true;
}

bool flora::XOpenGLDrawIndirectBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool flora::XOpenGLDrawIndirectBuffer::UpdateRHI()
{
    return true;
}

void flora::XOpenGLDrawIndirectBuffer::Bind()
{
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, Handle);

}

void flora::XOpenGLDrawIndirectBuffer::UnBind()
{
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);

}

bool flora::XOpenGLDrawIndirectBuffer::SetData(size_t size, const void* data, EBufferUsage usage)
{
    Bind();
    glBufferData(GL_DRAW_INDIRECT_BUFFER, size, data, flora::ToGLBufferUsage(usage));
    return IsValid();
}

bool flora::XOpenGLDrawIndirectBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_DRAW_INDIRECT_BUFFER, offset, size, data);
	return true;
}

void* flora::XOpenGLDrawIndirectBuffer::Map(EAccessPolicy policy)
{
    Bind();
    return glMapBuffer(GL_DRAW_INDIRECT_BUFFER, flora::ToGLAccessPolicy(policy));
}

void* flora::XOpenGLDrawIndirectBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, offset, length, flora::ToGLAccessPolicy(policy));
}

bool flora::XOpenGLDrawIndirectBuffer::Unmap()
{
    Bind();
    return glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
}

void flora::XOpenGLDrawIndirectBuffer::BindBufferBase()
{
    Bind();
    // draw indirect buffer must bound to GL_SHADER_STORAGE_BUFFER  if write in compute shader
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BindingSlot, Handle);
}


//==========================//

flora::XOpenGLAtomicCounterBuffer::~XOpenGLAtomicCounterBuffer()
{
	ReleaseRHI();
}

bool flora::XOpenGLAtomicCounterBuffer::IsValid()
{
	return glIsBuffer(Handle) == GL_TRUE;
}

bool flora::XOpenGLAtomicCounterBuffer::InitRHI()
{
	glGenBuffers(1, &Handle);
	Bind();
	return IsValid();
}

bool flora::XOpenGLAtomicCounterBuffer::ReleaseRHI()
{
	glDeleteBuffers(1, &Handle);
	return true;
}

bool flora::XOpenGLAtomicCounterBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
	return false;
}

bool flora::XOpenGLAtomicCounterBuffer::UpdateRHI()
{
	return true;
}

void flora::XOpenGLAtomicCounterBuffer::Bind()
{
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, Handle);

}

void flora::XOpenGLAtomicCounterBuffer::UnBind()
{
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

}

bool flora::XOpenGLAtomicCounterBuffer::SetData(size_t size, const void* data, EBufferUsage usage)
{
	Bind();
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, size, data, flora::ToGLBufferUsage(usage));
	return IsValid();
}

bool flora::XOpenGLAtomicCounterBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, offset, size, data);
	return true;
}

void* flora::XOpenGLAtomicCounterBuffer::Map(EAccessPolicy policy)
{
	Bind();
	return glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, flora::ToGLAccessPolicy(policy));
}

void* flora::XOpenGLAtomicCounterBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, offset, length, flora::ToGLAccessPolicy(policy));
}

bool flora::XOpenGLAtomicCounterBuffer::Unmap()
{
	Bind();
	return glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
}

void flora::XOpenGLAtomicCounterBuffer::BindBufferBase()
{
	Bind();
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, BindingSlot, Handle);
}



//==========================//

flora::XOpenGLDispatchIndirectBuffer::~XOpenGLDispatchIndirectBuffer()
{
	ReleaseRHI();
}

bool flora::XOpenGLDispatchIndirectBuffer::IsValid()
{
	return glIsBuffer(Handle) == GL_TRUE;
}

bool flora::XOpenGLDispatchIndirectBuffer::InitRHI()
{
	glGenBuffers(1, &Handle);
	Bind();
	return IsValid();
}

bool flora::XOpenGLDispatchIndirectBuffer::ReleaseRHI()
{
	glDeleteBuffers(1, &Handle);
	return true;
}

bool flora::XOpenGLDispatchIndirectBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
	return false;
}

bool flora::XOpenGLDispatchIndirectBuffer::UpdateRHI()
{
	return true;
}

void flora::XOpenGLDispatchIndirectBuffer::Bind()
{
	glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, Handle);

}

void flora::XOpenGLDispatchIndirectBuffer::UnBind()
{
	glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, 0);

}

bool flora::XOpenGLDispatchIndirectBuffer::SetData(size_t size, const void* data, EBufferUsage usage)
{
	Bind();
	glBufferData(GL_DISPATCH_INDIRECT_BUFFER, size, data, flora::ToGLBufferUsage(usage));
	return IsValid();
}

bool flora::XOpenGLDispatchIndirectBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_DISPATCH_INDIRECT_BUFFER, offset, size, data);
	return true;
}

void* flora::XOpenGLDispatchIndirectBuffer::Map(EAccessPolicy policy)
{
	Bind();
	return glMapBuffer(GL_DISPATCH_INDIRECT_BUFFER, flora::ToGLAccessPolicy(policy));
}

void* flora::XOpenGLDispatchIndirectBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_DISPATCH_INDIRECT_BUFFER, offset, length, flora::ToGLAccessPolicy(policy));
}

bool flora::XOpenGLDispatchIndirectBuffer::Unmap()
{
	Bind();
	return glUnmapBuffer(GL_DISPATCH_INDIRECT_BUFFER);
}

void flora::XOpenGLDispatchIndirectBuffer::BindBufferBase()
{
	Bind();
	glBindBufferBase(GL_DISPATCH_INDIRECT_BUFFER, BindingSlot, Handle);
}
