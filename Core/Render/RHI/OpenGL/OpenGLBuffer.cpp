#include "OpenGLBuffer.h"
#include <Render/RHI/OpenGL/OpenGLRHI.h>

XVerse::XOpenGLVertexBuffer::~XOpenGLVertexBuffer()
{
    ReleaseRHI();
}

bool XVerse::XOpenGLVertexBuffer::IsValid()
{
    Bind();
    return glIsBuffer(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLVertexBuffer::InitRHI()
{
    glGenBuffers(1, &Handle);
    return IsValid();
}

bool XVerse::XOpenGLVertexBuffer::ReleaseRHI()
{
    glDeleteBuffers(1, &Handle);
    return true;
}

bool XVerse::XOpenGLVertexBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool XVerse::XOpenGLVertexBuffer::UpdateRHI()
{
    return true;
}

void XVerse::XOpenGLVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, Handle);
}

void XVerse::XOpenGLVertexBuffer::UnBind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool XVerse::XOpenGLVertexBuffer::SetData(size_t size, const void* data, EBufferUsage usage)
{
    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, XVerse::ToGLBufferUsage(usage));
    return IsValid();
}

bool XVerse::XOpenGLVertexBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
    glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    return true;
}

void* XVerse::XOpenGLVertexBuffer::Map(EAccessPolicy policy)
{
    Bind();
    return glMapBuffer(GL_ARRAY_BUFFER, XVerse::ToGLAccessPolicy(policy));
}

void* XVerse::XOpenGLVertexBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_ARRAY_BUFFER, offset, length, XVerse::ToGLAccessPolicy(policy));
}

bool XVerse::XOpenGLVertexBuffer::Unmap()
{
    Bind();
    return glUnmapBuffer(GL_ARRAY_BUFFER);
}

void XVerse::XOpenGLVertexBuffer::BindBufferBase()
{
    Bind();
    glBindBufferBase(GL_ARRAY_BUFFER, BindingSlot, Handle);
}


XVerse::XOpenGLIndexBuffer::~XOpenGLIndexBuffer()
{
    ReleaseRHI();
}

bool XVerse::XOpenGLIndexBuffer::IsValid()
{
    return glIsBuffer(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLIndexBuffer::InitRHI()
{
    glGenBuffers(1, &Handle);
    return IsValid();
}

bool XVerse::XOpenGLIndexBuffer::ReleaseRHI()
{
    glDeleteBuffers(1, &Handle);
    return true;
}

bool XVerse::XOpenGLIndexBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool XVerse::XOpenGLIndexBuffer::UpdateRHI()
{
    return true;
}

void XVerse::XOpenGLIndexBuffer::Bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Handle);

}

void XVerse::XOpenGLIndexBuffer::UnBind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

bool XVerse::XOpenGLIndexBuffer::SetData(size_t size, const void* data, EBufferUsage usage)
{
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, XVerse::ToGLBufferUsage(usage));
    return IsValid();
}

bool XVerse::XOpenGLIndexBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	return true;
}

void* XVerse::XOpenGLIndexBuffer::Map(EAccessPolicy policy)
{
    Bind();
    return glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, XVerse::ToGLAccessPolicy(policy));
}

void* XVerse::XOpenGLIndexBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, offset, length, XVerse::ToGLAccessPolicy(policy));
}

bool XVerse::XOpenGLIndexBuffer::Unmap()
{
    Bind();
    return glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void XVerse::XOpenGLIndexBuffer::BindBufferBase()
{
    Bind();
    glBindBufferBase(GL_ELEMENT_ARRAY_BUFFER, BindingSlot, Handle);
}

XVerse::XOpenGLUniformBuffer::~XOpenGLUniformBuffer()
{
    ReleaseRHI();
}

bool XVerse::XOpenGLUniformBuffer::IsValid()
{
    return glIsBuffer(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLUniformBuffer::InitRHI()
{
    glGenBuffers(1, &Handle);
    Bind();
    return IsValid();
}

bool XVerse::XOpenGLUniformBuffer::ReleaseRHI()
{
    glDeleteBuffers(1, &Handle);
    return true;
}

bool XVerse::XOpenGLUniformBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool XVerse::XOpenGLUniformBuffer::UpdateRHI()
{
    return true;
}

void XVerse::XOpenGLUniformBuffer::Bind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, Handle);

}

void XVerse::XOpenGLUniformBuffer::UnBind()
{
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

bool XVerse::XOpenGLUniformBuffer::SetData(size_t size, const void* data, EBufferUsage usage)
{
    glBufferData(GL_UNIFORM_BUFFER, size, data, XVerse::ToGLBufferUsage(usage));
    return IsValid();
}

bool XVerse::XOpenGLUniformBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	return true;
}

void* XVerse::XOpenGLUniformBuffer::Map(EAccessPolicy policy)
{
    Bind();
    return glMapBuffer(GL_UNIFORM_BUFFER, XVerse::ToGLAccessPolicy(policy));
}

void* XVerse::XOpenGLUniformBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_UNIFORM_BUFFER, offset, length, XVerse::ToGLAccessPolicy(policy));
}

bool XVerse::XOpenGLUniformBuffer::Unmap()
{
    Bind();
    return glUnmapBuffer(GL_UNIFORM_BUFFER);
}

void XVerse::XOpenGLUniformBuffer::BindBufferBase()
{
    Bind();
    glBindBufferBase(GL_UNIFORM_BUFFER, BindingSlot, Handle);
}

XVerse::XOpenGLShaderStorageBuffer::~XOpenGLShaderStorageBuffer()
{
    ReleaseRHI();
}

bool XVerse::XOpenGLShaderStorageBuffer::IsValid()
{
    return glIsBuffer(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLShaderStorageBuffer::InitRHI()
{
    glGenBuffers(1, &Handle);
    Bind();
    return IsValid();
}

bool XVerse::XOpenGLShaderStorageBuffer::ReleaseRHI()
{
    glDeleteBuffers(1, &Handle);
    return true;
}

bool XVerse::XOpenGLShaderStorageBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool XVerse::XOpenGLShaderStorageBuffer::UpdateRHI()
{
    return true;
}

void XVerse::XOpenGLShaderStorageBuffer::Bind()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, Handle);

}

void XVerse::XOpenGLShaderStorageBuffer::UnBind()
{
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

}

bool XVerse::XOpenGLShaderStorageBuffer::SetData(size_t size, const void*data, EBufferUsage usage)
{
    Bind();
    glBufferData(GL_SHADER_STORAGE_BUFFER, size, data, XVerse::ToGLBufferUsage(usage));
    return IsValid();
}

bool XVerse::XOpenGLShaderStorageBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, size, data);
	return true;
}

void* XVerse::XOpenGLShaderStorageBuffer::Map(EAccessPolicy policy)
{
    Bind();
    return glMapBuffer(GL_SHADER_STORAGE_BUFFER, XVerse::ToGLAccessPolicy(policy));
}

void* XVerse::XOpenGLShaderStorageBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_SHADER_STORAGE_BUFFER, offset,length, XVerse::ToGLAccessPolicy(policy));
}

bool XVerse::XOpenGLShaderStorageBuffer::Unmap()
{
    Bind();
    return glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void XVerse::XOpenGLShaderStorageBuffer::BindBufferBase()
{
    Bind();
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BindingSlot, Handle);
}

//==========================//

XVerse::XOpenGLDrawIndirectBuffer::~XOpenGLDrawIndirectBuffer()
{
    ReleaseRHI();
}

bool XVerse::XOpenGLDrawIndirectBuffer::IsValid()
{
    return glIsBuffer(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLDrawIndirectBuffer::InitRHI()
{
    glGenBuffers(1, &Handle);
    Bind();
    return IsValid();
}

bool XVerse::XOpenGLDrawIndirectBuffer::ReleaseRHI()
{
    glDeleteBuffers(1, &Handle);
    return true;
}

bool XVerse::XOpenGLDrawIndirectBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
    return false;
}

bool XVerse::XOpenGLDrawIndirectBuffer::UpdateRHI()
{
    return true;
}

void XVerse::XOpenGLDrawIndirectBuffer::Bind()
{
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, Handle);

}

void XVerse::XOpenGLDrawIndirectBuffer::UnBind()
{
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);

}

bool XVerse::XOpenGLDrawIndirectBuffer::SetData(size_t size, const void* data, EBufferUsage usage)
{
    Bind();
    glBufferData(GL_DRAW_INDIRECT_BUFFER, size, data, XVerse::ToGLBufferUsage(usage));
    return IsValid();
}

bool XVerse::XOpenGLDrawIndirectBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_DRAW_INDIRECT_BUFFER, offset, size, data);
	return true;
}

void* XVerse::XOpenGLDrawIndirectBuffer::Map(EAccessPolicy policy)
{
    Bind();
    return glMapBuffer(GL_DRAW_INDIRECT_BUFFER, XVerse::ToGLAccessPolicy(policy));
}

void* XVerse::XOpenGLDrawIndirectBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, offset, length, XVerse::ToGLAccessPolicy(policy));
}

bool XVerse::XOpenGLDrawIndirectBuffer::Unmap()
{
    Bind();
    return glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
}

void XVerse::XOpenGLDrawIndirectBuffer::BindBufferBase()
{
    Bind();
    // draw indirect buffer must bound to GL_SHADER_STORAGE_BUFFER  if write in compute shader
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, BindingSlot, Handle);
}


//==========================//

XVerse::XOpenGLAtomicCounterBuffer::~XOpenGLAtomicCounterBuffer()
{
	ReleaseRHI();
}

bool XVerse::XOpenGLAtomicCounterBuffer::IsValid()
{
	return glIsBuffer(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLAtomicCounterBuffer::InitRHI()
{
	glGenBuffers(1, &Handle);
	Bind();
	return IsValid();
}

bool XVerse::XOpenGLAtomicCounterBuffer::ReleaseRHI()
{
	glDeleteBuffers(1, &Handle);
	return true;
}

bool XVerse::XOpenGLAtomicCounterBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
	return false;
}

bool XVerse::XOpenGLAtomicCounterBuffer::UpdateRHI()
{
	return true;
}

void XVerse::XOpenGLAtomicCounterBuffer::Bind()
{
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, Handle);

}

void XVerse::XOpenGLAtomicCounterBuffer::UnBind()
{
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

}

bool XVerse::XOpenGLAtomicCounterBuffer::SetData(size_t size, const void* data, EBufferUsage usage)
{
	Bind();
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, size, data, XVerse::ToGLBufferUsage(usage));
	return IsValid();
}

bool XVerse::XOpenGLAtomicCounterBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, offset, size, data);
	return true;
}

void* XVerse::XOpenGLAtomicCounterBuffer::Map(EAccessPolicy policy)
{
	Bind();
	return glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, XVerse::ToGLAccessPolicy(policy));
}

void* XVerse::XOpenGLAtomicCounterBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_ATOMIC_COUNTER_BUFFER, offset, length, XVerse::ToGLAccessPolicy(policy));
}

bool XVerse::XOpenGLAtomicCounterBuffer::Unmap()
{
	Bind();
	return glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
}

void XVerse::XOpenGLAtomicCounterBuffer::BindBufferBase()
{
	Bind();
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, BindingSlot, Handle);
}



//==========================//

XVerse::XOpenGLDispatchIndirectBuffer::~XOpenGLDispatchIndirectBuffer()
{
	ReleaseRHI();
}

bool XVerse::XOpenGLDispatchIndirectBuffer::IsValid()
{
	return glIsBuffer(Handle) == GL_TRUE;
}

bool XVerse::XOpenGLDispatchIndirectBuffer::InitRHI()
{
	glGenBuffers(1, &Handle);
	Bind();
	return IsValid();
}

bool XVerse::XOpenGLDispatchIndirectBuffer::ReleaseRHI()
{
	glDeleteBuffers(1, &Handle);
	return true;
}

bool XVerse::XOpenGLDispatchIndirectBuffer::CopyFrom(std::shared_ptr<XRHIResource>)
{
	return false;
}

bool XVerse::XOpenGLDispatchIndirectBuffer::UpdateRHI()
{
	return true;
}

void XVerse::XOpenGLDispatchIndirectBuffer::Bind()
{
	glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, Handle);

}

void XVerse::XOpenGLDispatchIndirectBuffer::UnBind()
{
	glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, 0);

}

bool XVerse::XOpenGLDispatchIndirectBuffer::SetData(size_t size, const void* data, EBufferUsage usage)
{
	Bind();
	glBufferData(GL_DISPATCH_INDIRECT_BUFFER, size, data, XVerse::ToGLBufferUsage(usage));
	return IsValid();
}

bool XVerse::XOpenGLDispatchIndirectBuffer::UpdateSubData(size_t offset, size_t size, const void* data)
{
	Bind();
	glBufferSubData(GL_DISPATCH_INDIRECT_BUFFER, offset, size, data);
	return true;
}

void* XVerse::XOpenGLDispatchIndirectBuffer::Map(EAccessPolicy policy)
{
	Bind();
	return glMapBuffer(GL_DISPATCH_INDIRECT_BUFFER, XVerse::ToGLAccessPolicy(policy));
}

void* XVerse::XOpenGLDispatchIndirectBuffer::MapRange(int offset, size_t length, EAccessPolicy policy)
{
	Bind();
	return glMapBufferRange(GL_DISPATCH_INDIRECT_BUFFER, offset, length, XVerse::ToGLAccessPolicy(policy));
}

bool XVerse::XOpenGLDispatchIndirectBuffer::Unmap()
{
	Bind();
	return glUnmapBuffer(GL_DISPATCH_INDIRECT_BUFFER);
}

void XVerse::XOpenGLDispatchIndirectBuffer::BindBufferBase()
{
	Bind();
	glBindBufferBase(GL_DISPATCH_INDIRECT_BUFFER, BindingSlot, Handle);
}
