#pragma once
#include <cstring>
#include <Render/RHI/OpenGL/OpenGLRHI.h>
namespace flora
{
	class XOpenGLTexture2D :public XRHITexture2D
	{
	public:
		XOpenGLTexture2D() = default;
		XOpenGLTexture2D(uint32_t InMipLevels, EInternalFormat InInternalFormat, uint32_t InX, uint32_t InY) :XRHITexture2D(InMipLevels, InInternalFormat, InX, InY) {}
		virtual ~XOpenGLTexture2D();
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

		/// <summary>
		/// Append Next Mip Level
		/// </summary>
		virtual void AddNewMip(uint32_t InSizeX, uint32_t InSizeY) override;
	public:
		virtual void Bind() override;

		virtual void UnBind() override;

		/// <summary>
		/// read pixel data from specified mip level
		/// </summary>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out buffer size"></param>
		/// <param name="out pixel buffer"></param>
		virtual void ReadPixels(int level, EFormat format, EDataType type, void* pixels)override;

		/// <summary>
		/// set pixel data
		/// </summary>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out data"></param>
		virtual void SetPixels(uint32_t, EFormat, EDataType, const void*) override;
	public:
		/*virtual void SetMinFilter(EFilterMode)const override;
		virtual void SetMagFilter(EFilterMode)const override;

		virtual void SetWrapR(EWrapMode)const override;
		virtual void SetWrapS(EWrapMode)const override;
		virtual void SetWrapT(EWrapMode)const override;

		virtual void SetMinLod(uint8_t)const override;
		virtual void SetMaxLod(uint8_t)const override;

		virtual void SetLodBias(float)const override;

		virtual void SetBorderColor(float, float, float, float)const override;

		virtual void SetCompareMode(ETextureCompareMode)const override;
		virtual void SetCompareFunc(ECompareFunc)const override;*/

		virtual void SetSamplerState(const XRHISamplerCreateInfo&) override;
	public:
		virtual uint32_t GetHandle()const override { return Handle; }
	private:
		uint32_t Handle;
	};

	class XOpenGLVolumeTexture :public XRHIVolumeTexture
	{
	public:
		XOpenGLVolumeTexture() = default;
		XOpenGLVolumeTexture(uint32_t InMipLevels, EInternalFormat InInternalFormat, uint32_t InX, uint32_t InY, uint32_t InZ) :XRHIVolumeTexture(InMipLevels, InInternalFormat, InX, InY, InZ) {}
		virtual ~XOpenGLVolumeTexture();
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
		virtual void Bind() override;

		virtual void UnBind() override;

		/// <summary>
		/// read pixel data from specified mip level
		/// </summary>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out buffer size"></param>
		/// <param name="out pixel buffer"></param>
		virtual void ReadPixels(int level, EFormat format, EDataType type, void* pixels)override;

		/// <summary>
		/// set pixel data
		/// </summary>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out data"></param>
		virtual void SetPixels(uint32_t, EFormat, EDataType, const void*) override;
	public:
		/*virtual void SetMinFilter(EFilterMode)const override;
		virtual void SetMagFilter(EFilterMode)const override;

		virtual void SetWrapR(EWrapMode)const override;
		virtual void SetWrapS(EWrapMode)const override;
		virtual void SetWrapT(EWrapMode)const override;

		virtual void SetMinLod(uint8_t)const override;
		virtual void SetMaxLod(uint8_t)const override;

		virtual void SetLodBias(float)const override;

		virtual void SetBorderColor(float, float, float, float)const override;

		virtual void SetCompareMode(ETextureCompareMode)const override;
		virtual void SetCompareFunc(ECompareFunc)const override;*/

		virtual void SetSamplerState(const XRHISamplerCreateInfo&) override;
	public:
		virtual uint32_t GetHandle()const override { return Handle; }
	private:
		uint32_t Handle;
	};

	class XOpenGLTextureCube :public XRHITextureCube
	{
	public:
		XOpenGLTextureCube() = default;
		XOpenGLTextureCube(uint32_t InMipLevels, EInternalFormat InInternalFormat, uint32_t InX) :XRHITextureCube(InMipLevels, InInternalFormat, InX) {}
		virtual ~XOpenGLTextureCube();
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
		virtual void Bind() override;

		virtual void UnBind() override;

		/// <summary>
		/// read pixel data from specified mip level
		/// </summary>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out buffer size"></param>
		/// <param name="out pixel buffer"></param>
		virtual void ReadPixels(int level, EFormat format, EDataType type, void* pixels)override;

		/// <summary>
		/// set pixel data
		/// </summary>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out data"></param>
		virtual void SetPixels(uint32_t, EFormat, EDataType, const void*) override;

		/// <summary>
		/// read pixel data from specified mip level and face 
		/// </summary>
		/// <param name="cube face"></param>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out buffer size"></param>
		/// <param name="out pixel buffer"></param>
		virtual void ReadPixels(ECubeFace face, int level, EFormat format, EDataType type, void* pixels) override;

		/// <summary>
		/// set pixel data and face 
		/// </summary>
		/// <param name="cube face"></param>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out data"></param>
		virtual void SetPixels(ECubeFace face, uint32_t, EFormat, EDataType, const void*) override;
	public:
		/*virtual void SetMinFilter(EFilterMode)const override;
		virtual void SetMagFilter(EFilterMode)const override;

		virtual void SetWrapR(EWrapMode)const override;
		virtual void SetWrapS(EWrapMode)const override;
		virtual void SetWrapT(EWrapMode)const override;

		virtual void SetMinLod(uint8_t)const override;
		virtual void SetMaxLod(uint8_t)const override;

		virtual void SetLodBias(float)const override;

		virtual void SetBorderColor(float, float, float, float)const override;

		virtual void SetCompareMode(ETextureCompareMode)const override;
		virtual void SetCompareFunc(ECompareFunc)const override;*/

		virtual void SetSamplerState(const XRHISamplerCreateInfo&) override;
	public:
		virtual uint32_t GetHandle()const override { return Handle; }
	private:
		uint32_t Handle;
	};

};
