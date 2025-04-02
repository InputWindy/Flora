#pragma once
#include <Render/RHI/OpenGL/OpenGLRHI.h>
namespace XVerse
{
	class XOpenGLVertexBuffer :public XRHIVertexBuffer
	{
	public:
		XOpenGLVertexBuffer() = default;
		XOpenGLVertexBuffer(uint32_t InBindingSlot) :XRHIVertexBuffer(InBindingSlot) {}
		virtual ~XOpenGLVertexBuffer();
	public:
		/// <summary>
		/// whether GPU resources have been allocated successfully.
		/// </summary>
		virtual bool IsValid()override;

		/// <summary>
		/// try allocate GPU resource
		/// </summary>
		virtual bool InitRHI()override;

		/// <summary>
		/// try deallocate GPU resource
		/// </summary>
		virtual bool ReleaseRHI()override;

		/// <summary>
		/// deep copy
		/// <param name="rhs"></param>
		/// </summary>
		virtual bool CopyFrom(std::shared_ptr<XRHIResource>)override;

		/// <summary>
		/// synchronize CPU resources to the GPU without checking the correctness of the resources.
		/// </summary>
		virtual bool UpdateRHI()override;
	public:
		/// <summary>
		/// bind buffer
		/// </summary>
		virtual void Bind() override;

		/// <summary>
		/// unbind buffer
		/// </summary>
		virtual void UnBind() override;

		/// <summary>
		/// load data
		/// </summary>
		/// <param name="raw data size"></param>
		/// <param name="raw data pointer"></param>
		/// <param name="usage"></param>
		/// <returns></returns>
		virtual bool SetData(size_t, const void*, EBufferUsage) override; //GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY

		/// <summary>
		/// update sub data
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="size"></param>
		/// <param name="data"></param>
		/// <returns></returns>
		virtual bool UpdateSubData(size_t offset, size_t size, const void*data) override;

		/// <summary>
		/// begin read buffer data:
		/// policy:
		///		GL_READ_ONLY    
		///		GL_WRITE_ONLY    
		///		GL_READ_WRITE    
		/// </summary>
		/// <param name="accessPolicy"></param>
		/// <returns></returns>
		virtual void* Map(EAccessPolicy /*= GL_READ_WRITE*/) override;

		virtual void* MapRange(int, size_t, EAccessPolicy)override;

		/// <summary>
		/// end read buffer data
		/// </summary>
		/// <returns></returns>
		virtual bool Unmap() override;

		/// <summary>
		/// bind to slot
		/// </summary>
		/// <param name=""></param>
		virtual void BindBufferBase() override;
	public:
		virtual uint32_t GetHandle()const override { return Handle; }
	private:
		uint32_t Handle;

	};

	class XOpenGLIndexBuffer :public XRHIIndexBuffer
	{
	public:
		XOpenGLIndexBuffer() = default;
		XOpenGLIndexBuffer(uint32_t InBindingSlot) :XRHIIndexBuffer(InBindingSlot) {}
		virtual ~XOpenGLIndexBuffer();
	public:
		/// <summary>
		/// whether GPU resources have been allocated successfully.
		/// </summary>
		virtual bool IsValid()override;

		/// <summary>
		/// try allocate GPU resource
		/// </summary>
		virtual bool InitRHI()override;

		/// <summary>
		/// try deallocate GPU resource
		/// </summary>
		virtual bool ReleaseRHI()override;

		/// <summary>
		/// deep copy
		/// <param name="rhs"></param>
		/// </summary>
		virtual bool CopyFrom(std::shared_ptr<XRHIResource>)override;

		/// <summary>
		/// synchronize CPU resources to the GPU without checking the correctness of the resources.
		/// </summary>
		virtual bool UpdateRHI()override;
	public:
		/// <summary>
		/// bind buffer
		/// </summary>
		virtual void Bind() override;

		/// <summary>
		/// unbind buffer
		/// </summary>
		virtual void UnBind() override;

		/// <summary>
		/// load data
		/// </summary>
		/// <param name="raw data size"></param>
		/// <param name="raw data pointer"></param>
		/// <param name="usage"></param>
		/// <returns></returns>
		virtual bool SetData(size_t, const void*, EBufferUsage) override; //GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY

		/// <summary>
		/// update sub data
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="size"></param>
		/// <param name="data"></param>
		/// <returns></returns>
		virtual bool UpdateSubData(size_t offset, size_t size, const void* data) override;
		/// <summary>
		/// begin read buffer data:
		/// policy:
		///		GL_READ_ONLY    
		///		GL_WRITE_ONLY    
		///		GL_READ_WRITE    
		/// </summary>
		/// <param name="accessPolicy"></param>
		/// <returns></returns>
		virtual void* Map(EAccessPolicy /*= GL_READ_WRITE*/) override;

		virtual void* MapRange(int, size_t, EAccessPolicy)override;

		/// <summary>
		/// end read buffer data
		/// </summary>
		/// <returns></returns>
		virtual bool Unmap() override;

		/// <summary>
		/// bind to slot
		/// </summary>
		/// <param name=""></param>
		virtual void BindBufferBase() override;
	public:
		virtual uint32_t GetHandle()const override { return Handle; }
	private:
		uint32_t Handle;

	};

	class XOpenGLUniformBuffer :public XRHIUniformBuffer
	{
	public:
		XOpenGLUniformBuffer() = default;
		XOpenGLUniformBuffer(uint32_t InBindingSlot) :XRHIUniformBuffer(InBindingSlot) {}
		virtual ~XOpenGLUniformBuffer();
	public:
		/// <summary>
		/// whether GPU resources have been allocated successfully.
		/// </summary>
		virtual bool IsValid()override;

		/// <summary>
		/// try allocate GPU resource
		/// </summary>
		virtual bool InitRHI()override;

		/// <summary>
		/// try deallocate GPU resource
		/// </summary>
		virtual bool ReleaseRHI()override;

		/// <summary>
		/// deep copy
		/// <param name="rhs"></param>
		/// </summary>
		virtual bool CopyFrom(std::shared_ptr<XRHIResource>)override;

		/// <summary>
		/// synchronize CPU resources to the GPU without checking the correctness of the resources.
		/// </summary>
		virtual bool UpdateRHI()override;
	public:
		/// <summary>
		/// bind buffer
		/// </summary>
		virtual void Bind() override;

		/// <summary>
		/// unbind buffer
		/// </summary>
		virtual void UnBind() override;

		/// <summary>
		/// load data
		/// </summary>
		/// <param name="raw data size"></param>
		/// <param name="raw data pointer"></param>
		/// <param name="usage"></param>
		/// <returns></returns>
		virtual bool SetData(size_t, const void*, EBufferUsage) override; //GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY

		/// <summary>
		/// update sub data
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="size"></param>
		/// <param name="data"></param>
		/// <returns></returns>
		virtual bool UpdateSubData(size_t offset, size_t size, const void* data) override;

		/// <summary>
		/// begin read buffer data:
		/// policy:
		///		GL_READ_ONLY    
		///		GL_WRITE_ONLY    
		///		GL_READ_WRITE    
		/// </summary>
		/// <param name="accessPolicy"></param>
		/// <returns></returns>
		virtual void* Map(EAccessPolicy /*= GL_READ_WRITE*/) override;

		virtual void* MapRange(int, size_t, EAccessPolicy)override;

		/// <summary>
		/// end read buffer data
		/// </summary>
		/// <returns></returns>
		virtual bool Unmap() override;

		/// <summary>
		/// bind to slot
		/// </summary>
		/// <param name=""></param>
		virtual void BindBufferBase() override;
	public:
		virtual uint32_t GetHandle()const override { return Handle; }
	private:
		uint32_t Handle;

	};

	class XOpenGLShaderStorageBuffer :public XRHIShaderStorageBuffer
	{
	public:
		XOpenGLShaderStorageBuffer() = default;
		XOpenGLShaderStorageBuffer(uint32_t InBindingSlot) :XRHIShaderStorageBuffer(InBindingSlot) {}
		virtual ~XOpenGLShaderStorageBuffer();
	public:
		/// <summary>
		/// whether GPU resources have been allocated successfully.
		/// </summary>
		virtual bool IsValid()override;

		/// <summary>
		/// try allocate GPU resource
		/// </summary>
		virtual bool InitRHI()override;

		/// <summary>
		/// try deallocate GPU resource
		/// </summary>
		virtual bool ReleaseRHI()override;

		/// <summary>
		/// deep copy
		/// <param name="rhs"></param>
		/// </summary>
		virtual bool CopyFrom(std::shared_ptr<XRHIResource>)override;

		/// <summary>
		/// synchronize CPU resources to the GPU without checking the correctness of the resources.
		/// </summary>
		virtual bool UpdateRHI()override;
	public:
		/// <summary>
		/// bind buffer
		/// </summary>
		virtual void Bind() override;

		/// <summary>
		/// unbind buffer
		/// </summary>
		virtual void UnBind() override;

		/// <summary>
		/// load data
		/// </summary>
		/// <param name="raw data size"></param>
		/// <param name="raw data pointer"></param>
		/// <param name="usage"></param>
		/// <returns></returns>
		virtual bool SetData(size_t, const void*, EBufferUsage) override; //GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY

		/// <summary>
		/// update sub data
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="size"></param>
		/// <param name="data"></param>
		/// <returns></returns>
		virtual bool UpdateSubData(size_t offset, size_t size, const void* data) override;

		/// <summary>
		/// begin read buffer data:
		/// policy:
		///		GL_READ_ONLY    
		///		GL_WRITE_ONLY    
		///		GL_READ_WRITE    
		/// </summary>
		/// <param name="accessPolicy"></param>
		/// <returns></returns>
		virtual void* Map(EAccessPolicy /*= GL_READ_WRITE*/) override;

		virtual void* MapRange(int, size_t, EAccessPolicy)override;

		/// <summary>
		/// end read buffer data
		/// </summary>
		/// <returns></returns>
		virtual bool Unmap() override;

		/// <summary>
		/// bind to slot
		/// </summary>
		/// <param name=""></param>
		virtual void BindBufferBase() override;
	public:
		virtual uint32_t GetHandle()const override { return Handle; }
	private:
		uint32_t Handle;

	};

	class XOpenGLAtomicCounterBuffer :public XRHIAtomicCounterBuffer
	{
	public:
		XOpenGLAtomicCounterBuffer() = default;
		XOpenGLAtomicCounterBuffer(uint32_t InBindingSlot) :XRHIAtomicCounterBuffer(InBindingSlot) {}
		virtual ~XOpenGLAtomicCounterBuffer();
	public:
		/// <summary>
		/// whether GPU resources have been allocated successfully.
		/// </summary>
		virtual bool IsValid()override;

		/// <summary>
		/// try allocate GPU resource
		/// </summary>
		virtual bool InitRHI()override;

		/// <summary>
		/// try deallocate GPU resource
		/// </summary>
		virtual bool ReleaseRHI()override;

		/// <summary>
		/// deep copy
		/// <param name="rhs"></param>
		/// </summary>
		virtual bool CopyFrom(std::shared_ptr<XRHIResource>)override;

		/// <summary>
		/// synchronize CPU resources to the GPU without checking the correctness of the resources.
		/// </summary>
		virtual bool UpdateRHI()override;
	public:
		/// <summary>
		/// bind buffer
		/// </summary>
		virtual void Bind() override;

		/// <summary>
		/// unbind buffer
		/// </summary>
		virtual void UnBind() override;

		/// <summary>
		/// load data
		/// </summary>
		/// <param name="raw data size"></param>
		/// <param name="raw data pointer"></param>
		/// <param name="usage"></param>
		/// <returns></returns>
		virtual bool SetData(size_t, const void*, EBufferUsage) override; //GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY

		/// <summary>
		/// update sub data
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="size"></param>
		/// <param name="data"></param>
		/// <returns></returns>
		virtual bool UpdateSubData(size_t offset, size_t size, const void* data) override;

		/// <summary>
		/// begin read buffer data:
		/// policy:
		///		GL_READ_ONLY    
		///		GL_WRITE_ONLY    
		///		GL_READ_WRITE    
		/// </summary>
		/// <param name="accessPolicy"></param>
		/// <returns></returns>
		virtual void* Map(EAccessPolicy /*= GL_READ_WRITE*/) override;
		
		virtual void* MapRange(int, size_t, EAccessPolicy)override;

		/// <summary>
		/// end read buffer data
		/// </summary>
		/// <returns></returns>
		virtual bool Unmap() override;

		/// <summary>
		/// bind to slot
		/// </summary>
		/// <param name=""></param>
		virtual void BindBufferBase() override;
	public:
		virtual uint32_t GetHandle()const override { return Handle; }
	private:
		uint32_t Handle;

	};

	class XOpenGLDrawIndirectBuffer :public XRHIDrawIndirectBuffer
	{
	public:
		XOpenGLDrawIndirectBuffer() = default;
		XOpenGLDrawIndirectBuffer(uint32_t InBindingSlot) :XRHIDrawIndirectBuffer(InBindingSlot) {}
		virtual ~XOpenGLDrawIndirectBuffer();
	public:
		/// <summary>
		/// whether GPU resources have been allocated successfully.
		/// </summary>
		virtual bool IsValid()override;

		/// <summary>
		/// try allocate GPU resource
		/// </summary>
		virtual bool InitRHI()override;

		/// <summary>
		/// try deallocate GPU resource
		/// </summary>
		virtual bool ReleaseRHI()override;

		/// <summary>
		/// deep copy
		/// <param name="rhs"></param>
		/// </summary>
		virtual bool CopyFrom(std::shared_ptr<XRHIResource>)override;

		/// <summary>
		/// synchronize CPU resources to the GPU without checking the correctness of the resources.
		/// </summary>
		virtual bool UpdateRHI()override;
	public:
		/// <summary>
		/// bind buffer
		/// </summary>
		virtual void Bind() override;

		/// <summary>
		/// unbind buffer
		/// </summary>
		virtual void UnBind() override;

		/// <summary>
		/// load data
		/// </summary>
		/// <param name="raw data size"></param>
		/// <param name="raw data pointer"></param>
		/// <param name="usage"></param>
		/// <returns></returns>
		virtual bool SetData(size_t, const void*, EBufferUsage) override; //GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY

		/// <summary>
		/// update sub data
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="size"></param>
		/// <param name="data"></param>
		/// <returns></returns>
		virtual bool UpdateSubData(size_t offset, size_t size, const void* data) override;

		/// <summary>
		/// begin read buffer data:
		/// policy:
		///		GL_READ_ONLY    
		///		GL_WRITE_ONLY    
		///		GL_READ_WRITE    
		/// </summary>
		/// <param name="accessPolicy"></param>
		/// <returns></returns>
		virtual void* Map(EAccessPolicy /*= GL_READ_WRITE*/) override;

		virtual void* MapRange(int, size_t, EAccessPolicy)override;

		/// <summary>
		/// end read buffer data
		/// </summary>
		/// <returns></returns>
		virtual bool Unmap() override;

		/// <summary>
		/// bind to slot
		/// </summary>
		/// <param name=""></param>
		virtual void BindBufferBase() override;
	public:
		virtual uint32_t GetHandle()const override { return Handle; }
	private:
		uint32_t Handle;

	};

	class XOpenGLDispatchIndirectBuffer :public XRHIDispatchIndirectBuffer
	{
	public:
		XOpenGLDispatchIndirectBuffer() = default;
		XOpenGLDispatchIndirectBuffer(uint32_t InBindingSlot) :XRHIDispatchIndirectBuffer(InBindingSlot) {}
		virtual ~XOpenGLDispatchIndirectBuffer();
	public:
		/// <summary>
		/// whether GPU resources have been allocated successfully.
		/// </summary>
		virtual bool IsValid()override;

		/// <summary>
		/// try allocate GPU resource
		/// </summary>
		virtual bool InitRHI()override;

		/// <summary>
		/// try deallocate GPU resource
		/// </summary>
		virtual bool ReleaseRHI()override;

		/// <summary>
		/// deep copy
		/// <param name="rhs"></param>
		/// </summary>
		virtual bool CopyFrom(std::shared_ptr<XRHIResource>)override;

		/// <summary>
		/// synchronize CPU resources to the GPU without checking the correctness of the resources.
		/// </summary>
		virtual bool UpdateRHI()override;
	public:
		/// <summary>
		/// bind buffer
		/// </summary>
		virtual void Bind() override;

		/// <summary>
		/// unbind buffer
		/// </summary>
		virtual void UnBind() override;

		/// <summary>
		/// load data
		/// </summary>
		/// <param name="raw data size"></param>
		/// <param name="raw data pointer"></param>
		/// <param name="usage"></param>
		/// <returns></returns>
		virtual bool SetData(size_t, const void*, EBufferUsage) override; //GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY

		/// <summary>
		/// update sub data
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="size"></param>
		/// <param name="data"></param>
		/// <returns></returns>
		virtual bool UpdateSubData(size_t offset, size_t size, const void* data) override;

		/// <summary>
		/// begin read buffer data:
		/// policy:
		///		GL_READ_ONLY    
		///		GL_WRITE_ONLY    
		///		GL_READ_WRITE    
		/// </summary>
		/// <param name="accessPolicy"></param>
		/// <returns></returns>
		virtual void* Map(EAccessPolicy /*= GL_READ_WRITE*/) override;

		virtual void* MapRange(int, size_t, EAccessPolicy)override;

		/// <summary>
		/// end read buffer data
		/// </summary>
		/// <returns></returns>
		virtual bool Unmap() override;

		/// <summary>
		/// bind to slot
		/// </summary>
		/// <param name=""></param>
		virtual void BindBufferBase() override;
	public:
		virtual uint32_t GetHandle()const override { return Handle; }
	private:
		uint32_t Handle;

	};

}