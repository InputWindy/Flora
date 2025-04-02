#pragma once
#include <string>
#include <vector>
#include <map>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <cassert>
#include <functional>
#include <unordered_map>
#include <map>
#include <utility>
// #include <format>

#include <Common/Layout.h>
#include <Common/Log.h>

// #define XRHI_ASSERT_ERROR(...) flora::GetRHI()->AssertError(std::format(__VA_ARGS__));

namespace flora
{
	#define MAX_MIP_LEVEL 8
}

namespace flora
{
	enum class ERHIFeatureLevel
	{
		FL_OpenGL,
		FL_Vulkan,
		FL_Dx12,
		FL_Metal,

		FL_MAX_COUNT
	};

	enum class ERHIResourceType
	{
		RT_Shader,
		RT_ShaderProgram,
		RT_FrameBuffer,
		RT_RenderBuffer,
		RT_Query,
		RT_Sampler,
		RT_Texture2D,
		RT_Texture3D,
		RT_TextureCube,
		RT_VertexBuffer,
		RT_IndexBuffer,
		RT_ShaderStorageBuffer,
		RT_AtomicCounterBuffer,
		RT_DrawIndirectBuffer,
		RT_DispatchIndirectBuffer,
		RT_UniformBuffer,
		RT_VertexArray,

		RT_MAX_COUNT
	};

	typedef uint32_t ECubeFace;
	enum ECubeFace_
	{
		PX,
		NX,
		PY,
		NY,
		PZ,
		NZ,

		MAX_COUNT
	};

	enum class EDrawMode
	{
		DM_POINTS,
		DM_LINE_STRIP,
		DM_LINE_LOOP,
		DM_LINES,
		DM_LINE_STRIP_ADJACENCY,
		DM_LINES_ADJACENCY,
		DM_TRIANGLE_STRIP,
		DM_TRIANGLE_FAN,
		DM_TRIANGLES,
		DM_TRIANGLE_STRIP_ADJACENCY,
		DM_TRIANGLES_ADJACENCY,
		DM_PATCHES,

		DM_MAX_COUNT
	};

	typedef uint32_t EMemoryBarrierBit;
	enum EMemoryBarrierBit_
	{
		MBB_VERTEX_ATTRIB_ARRAY_BARRIER_BIT				= 1 << 0 ,
		MBB_ELEMENT_ARRAY_BARRIER_BIT					= 1 << 1 ,
		MBB_UNIFORM_BARRIER_BIT							= 1 << 2 ,
		MBB_TEXTURE_FETCH_BARRIER_BIT					= 1 << 3 ,
		MBB_SHADER_IMAGE_ACCESS_BARRIER_BIT				= 1 << 4 ,
		MBB_COMMAND_BARRIER_BIT							= 1 << 5 ,
		MBB_PIXEL_BUFFER_BARRIER_BIT					= 1 << 6 ,
		MBB_TEXTURE_UPDATE_BARRIER_BIT					= 1 << 7 ,
		MBB_BUFFER_UPDATE_BARRIER_BIT					= 1 << 8 ,
		MBB_FRAMEBUFFER_BARRIER_BIT						= 1 << 9 ,
		MBB_TRANSFORM_FEEDBACK_BARRIER_BIT				= 1 << 10,
		MBB_ATOMIC_COUNTER_BARRIER_BIT					= 1 << 11,
		MBB_SHADER_STORAGE_BARRIER_BIT					= 1 << 12,

		MBB_MAX_COUNT
	};

	typedef uint32_t EClearBufferBit;
	enum EClearBufferBit_
	{
		CBB_COLOR_BUFFER_BIT = 1 << 0,
		CBB_DEPTH_BUFFER_BIT = 1 << 1,
		CBB_STENCIL_BUFFER_BIT = 1 << 2,

		CBB_MAX_COUNT
	};

	enum class EFilterMode
	{
		FM_NEAREST,
		FM_LINEAR,
		FM_NEAREST_MIPMAP_NEAREST,
		FM_LINEAR_MIPMAP_NEAREST,
		FM_NEAREST_MIPMAP_LINEAR,
		FM_LINEAR_MIPMAP_LINEAR,

		FM_MAX_COUNT
	};

	enum class EWrapMode
	{
		WM_CLAMP_TO_BORDER,
		WM_CLAMP_TO_EDGE,
		WM_MIRRORED_REPEAT,
		WM_REPEAT,
		WM_MIRROR_CLAMP_TO_EDGE,

		WM_MAX_COUNT
	};

	enum class ETextureCompareMode
	{
		TCM_COMPARE_REF_TO_TEXTURE,
		TCM_NONE,

		TCM_MAX_COUNT
	};

	enum class EInternalFormat
	{
		IF_R8,
		IF_R8I,
		IF_R8UI,
		IF_R16,
		IF_R16I,
		IF_R16UI,
		IF_R32I,
		IF_R32UI,

		IF_RG8,
		IF_RG8I,
		IF_RG8UI,
		IF_RG16,
		IF_RG16I,
		IF_RG16UI,
		IF_RG32I,
		IF_RG32UI,

		IF_RGB8,
		IF_RGB8I,
		IF_RGB8UI,
		IF_RGB16,
		IF_RGB16I,
		IF_RGB16UI,
		IF_RGB32I,
		IF_RGB32UI,

		IF_RGBA8,
		IF_RGBA8I,
		IF_RGBA8UI,
		IF_RGBA16,
		IF_RGBA16I,
		IF_RGBA16UI,
		IF_RGBA32I,
		IF_RGBA32UI,

		IF_R16F,
		IF_R32F,

		IF_RG16F,
		IF_RG32F,

		IF_RGB16F,
		IF_RGB32F,

		IF_RGBA16F,
		IF_RGBA32F,

		/*
			only for render buffer
		*/
		IF_DEPTH_COMPONENT16,
		IF_DEPTH_COMPONENT24,
		IF_DEPTH_COMPONENT32F,
		IF_DEPTH24_STENCIL8,
		IF_DEPTH32F_STENCIL8,
		IF_STENCIL_INDEX,

		IF_MAX_COUNT
	};

	enum class EBufferTarget
	{
		BT_ARRAY_BUFFER,//vbo
		BT_ATOMIC_COUNTER_BUFFER,
		BT_COPY_READ_BUFFER,
		BT_COPY_WRITE_BUFFER,
		BT_DISPATCH_INDIRECT_BUFFER,
		BT_DRAW_INDIRECT_BUFFER,
		BT_ELEMENT_ARRAY_BUFFER,//ibo
		BT_PIXEL_PACK_BUFFER,
		BT_PIXEL_UNPACK_BUFFER,
		BT_QUERY_BUFFER,
		BT_SHADER_STORAGE_BUFFER,
		BT_TEXTURE_BUFFER,
		BT_TRANSFORM_FEEDBACK_BUFFER,
		BT_UNIFORM_BUFFER,

		BT_MAX_COUNT
	};

	enum class EBufferUsage
	{
		BU_STREAM_DRAW,
		BU_STREAM_READ,
		BU_STREAM_COPY,
		BU_STATIC_DRAW,
		BU_STATIC_READ,
		BU_STATIC_COPY,
		BU_DYNAMIC_DRAW,
		BU_DYNAMIC_READ,
		BU_DYNAMIC_COPY,

		BU_MAX_COUNT

	};

	enum class ETextureTarget
	{
		/*
			1D
		*/
		TT_TEXTURE_1D,
		TT_PROXY_TEXTURE_1D,

		/*
			2D
		*/
		TT_TEXTURE_2D,
		TT_PROXY_TEXTURE_2D,
		TT_TEXTURE_1D_ARRAY,
		TT_PROXY_TEXTURE_1D_ARRAY,
		TT_TEXTURE_RECTANGLE,
		TT_PROXY_TEXTURE_RECTANGLE,
		TT_TEXTURE_CUBE_MAP_POSITIVE_X,
		TT_TEXTURE_CUBE_MAP_NEGATIVE_X,
		TT_TEXTURE_CUBE_MAP_POSITIVE_Y,
		TT_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		TT_TEXTURE_CUBE_MAP_POSITIVE_Z,
		TT_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		TT_PROXY_TEXTURE_CUBE_MAP,
		TT_TEXTURE_2D_MULTISAMPLE,
		TT_PROXY_TEXTURE_2D_MULTISAMPLE,

		/*
			3D
		*/
		TT_TEXTURE_3D,
		TT_PROXY_TEXTURE_3D,
		TT_TEXTURE_2D_ARRAY,
		TT_PROXY_TEXTURE_2D_ARRAY,
		TT_TEXTURE_2D_MULTISAMPLE_ARRAY,
		TT_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY,

		/*
			for texture binding
		*/
		TT_TEXTURE_CUBE_MAP,
		TT_TEXTURE_CUBE_MAP_ARRAY,
		TT_TEXTURE_BUFFER,

		TT_MAX_COUNT
	};


	enum class EShaderType
	{
		ST_VERTEX_SHADER,
		ST_TESS_CONTROL_SHADER,
		ST_TESS_EVALUATION_SHADER,
		ST_GEOMETRY_SHADER,
		ST_FRAGMENT_SHADER,
		ST_COMPUTE_SHADER,

		ST_MAX_COUNT
	};

	enum class ECompareFunc
	{
		CF_LESS,       // -Default- Passes if the incoming value is less than the stored depth value.
		CF_NEVER,      // Never passes.
		CF_EQUAL,      // Passes if the incoming value is equal to the stored depth value.
		CF_LEQUAL,     // Passes if the incoming value is less than or equal to the stored depth value.
		CF_GREATER,    // Passes if the incoming value is greater than the stored depth value.
		CF_NOTEQUAL,   // Passes if the incoming value is not equal to the stored depth value.
		CF_GEQUAL,     // Passes if the incoming value is greater than or equal to the stored depth value.
		CF_ALWAYS,     // Always passes.

		CF_MAX_COUNT         // Count
	};

	enum class EStencilOp
	{
		SP_KEEP,      // -Default- Keeps the current value.
		SP_ZERO,      // Sets the stencil buffer value to 0.
		SP_REPLACE,   // Sets the stencil buffer value to ref, as specified by glStencilFunc.
		SP_INCR,      // Increments the current stencil buffer value. Clamps to the maximum representable unsigned value.
		SP_INCR_WRAP, // Increments the current stencil buffer value. Wraps stencil buffer value to zero when incrementing the maximum representable unsigned value.
		SP_DECR,      // Decrements the current stencil buffer value. Clamps to 0.
		SP_DECR_WRAP, // Decrements the current stencil buffer value. Wraps stencil buffer value to the maximum representable unsigned value when decrementing a stencil buffer value of zero.
		SP_INVERT,    // Bitwise inverts the current stencil buffer value.

		SP_MAX_COUNT        // Count
	};

	enum class EBlendEquation
	{
		BE_FUNC_ADD,             // -Default- 
		BE_FUNC_SUBTRACT,        //
		BE_FUNC_REVERSE_SUBTRACT,//
		BE_MIN,                  //
		BE_MAX,                  //
		//
		BE_MAX_COUNT             //
	};

	enum class EBlendFactor
	{
		BF_ZERO,                    // -Default For DST-
		BF_ONE,                     // -Default For SRC-
		BF_SRC_COLOR,               // 
		BF_ONE_MINUS_SRC_COLOR,     // 
		BF_DST_COLOR,               // 
		BF_ONE_MINUS_DST_COLOR,     // 
		BF_SRC_ALPHA,               // 
		BF_ONE_MINUS_SRC_ALPHA,     // 
		BF_DST_ALPHA,               // 
		BF_ONE_MINUS_DST_ALPHA,     // 
		BF_CONSTANT_COLOR,          // 
		BF_ONE_MINUS_CONSTANT_COLOR,//  
		BF_CONSTANT_ALPHA,          // 
		BF_ONE_MINUS_CONSTANT_ALPHA,//  
		BF_SRC_ALPHA_SATURATE,      // 
		BF_SRC1_COLOR,              // 
		BF_ONE_MINUS_SRC1_COLOR,    // 
		BF_SRC1_ALPHA,              // 
		BF_ONE_MINUS_SRC1_ALPHA,    // 
		// 
		BF_MAX_COUNT                // Count
	};

	enum class ECullFace
	{
		CF_BACK,            // -Default-
		CF_FRONT,
		CF_FRONT_AND_BACK,  // If mode is FRONT_AND_BACK, no facets are drawn, but other primitives such as points and lines are drawn.

		CF_MAX_COUNT
	};

	enum class EFrontFace
	{
		FF_CCW,// -Default- Counter Clock Wise
		FF_CW,


		FF_MAX_COUNT
	};

	enum class EDataType
	{
		DT_UNSIGNED_BYTE,   // -Default- jpg/jpeg/png
		DT_BYTE,
		DT_UNSIGNED_SHORT,
		DT_SHORT,
		DT_UNSIGNED_INT,
		DT_INT,
		DT_HALF_FLOAT,
		DT_FLOAT,           // Hdr

		DT_MAX_COUNT
	};

	enum class EFormat
	{
		F_RED,
		F_RG,
		F_RGB,      // -Default-
		F_BGR,
		F_RGBA,
		F_BGRA,
		F_RED_INTEGER,
		F_RG_INTEGER,
		F_RGB_INTEGER,
		F_BGR_INTEGER,
		F_RGBA_INTEGER,
		F_BGRA_INTEGER,
		F_STENCIL_INDEX,
		F_DEPTH_COMPONENT,
		F_DEPTH_STENCIL,

		F_MAX_COUNT
	};

	enum class EAccessPolicy
	{
		AP_READ_ONLY,
		AP_WRITE_ONLY,
		AP_READ_WRITE,

		AP_MAX_COUNT
	};

	enum class EQueryTarget
	{
		EQT_SAMPLES_PASSED,
		EQT_ANY_SAMPLES_PASSED,
		EQT_ANY_SAMPLES_PASSED_CONSERVATIVE,
		EQT_PRIMITIVES_GENERATED,
		EQT_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,
		EQT_TIME_ELAPSED,

		EQT_MAX_COUNT
	};

	flora::EInternalFormat MatchFormat(flora::EFormat, flora::EDataType);
	std::pair<flora::EFormat, flora::EDataType> MatchInternalFormat(flora::EInternalFormat);

	template<typename T>
	bool MatchDataType(flora::EDataType);
}

namespace flora
{
	class XRHIResource;
	class XRHIShader;
	class XRHIShaderProgram;
	class XRHIFrameBuffer;
	class XRHIRenderBuffer;
	class XRHISampler;
	class XRHITexture;
	class XRHITexture2D;
	class XRHIVolumeTexture;
	class XRHITextureCube;
	class XRHIBuffer;
	class XRHIVertexArray;
	class XRHIQuery;

	class XRHI;

	struct XRHIShaderCreateInfo;
	struct XRHIShaderProgramCreateInfo;
	struct XRHIFrameBufferCreateInfo;
	struct XRHIRenderBufferCreateInfo;
	struct XRHISamplerCreateInfo;
	struct XRHITextureCreateInfo;
	struct XRHIBufferCreateInfo;
	struct XRHIVertexArrayCreateInfo;
	struct XRHIQueryCreateInfo;

	struct XFrameBufferAttachment;
	struct XMaterialState;
}

namespace flora
{
	struct XRHIShaderCreateInfo
	{
		EShaderType ShaderType;
		std::string ShaderCode;
	};
	struct XRHIShaderProgramCreateInfo
	{
		std::shared_ptr<XRHIShader> VertexShader;
		std::shared_ptr<XRHIShader> TessControlShader;
		std::shared_ptr<XRHIShader> TessEvaluationShader;
		std::shared_ptr<XRHIShader> GeometryShader;
		std::shared_ptr<XRHIShader> FragmentShader;

		/*
		* compute
		*/
		std::shared_ptr<XRHIShader> ComputeShader;
	};
	struct XRHIFrameBufferCreateInfo
	{
		std::vector<XFrameBufferAttachment> ColorAttachments;

		std::shared_ptr<XRHIRenderBuffer> DepthStencilAttachment;

		std::vector<uint32_t> DrawBuffers;
	};
	struct XRHIRenderBufferCreateInfo
	{
		uint32_t Width = 1280;
		uint32_t Height = 720;

		EInternalFormat InternalFormat = EInternalFormat::IF_DEPTH24_STENCIL8;
	};
	struct XRHISamplerCreateInfo
	{
		EWrapMode WrapR = EWrapMode::WM_REPEAT;
		EWrapMode WrapS = EWrapMode::WM_REPEAT;
		EWrapMode WrapT = EWrapMode::WM_REPEAT;

		EFilterMode MinFilterMode = EFilterMode::FM_LINEAR;
		EFilterMode MagFilterMode = EFilterMode::FM_LINEAR;

		uint8_t MinMipLevel = 0;
		uint8_t MaxMipLevel = 0;
		float   LodBias = 0;

		std::vector<float> BorderColor = { 0,0,0,0 };

		ETextureCompareMode CompareMode = ETextureCompareMode::TCM_COMPARE_REF_TO_TEXTURE;
		ECompareFunc CompareFunc = ECompareFunc::CF_LESS;
	};
	struct XRHITextureCreateInfo
	{
		ETextureTarget TextureType = ETextureTarget::TT_TEXTURE_2D;

		uint32_t SizeX = 256;
		uint32_t SizeY = 256;

		/// <summary>
		/// volume texture
		/// </summary>
		uint32_t SizeZ = 256;

		EInternalFormat InternalFormat = EInternalFormat::IF_RGBA32F;
		uint32_t MipLevels = 1;

		XRHISamplerCreateInfo DefaultSamplerInfo;
	};
	struct XRHIBufferCreateInfo
	{
		EBufferTarget BufferTarget = EBufferTarget::BT_ARRAY_BUFFER;
		uint32_t BindingSlot = 0;
	};
	struct XRHIVertexArrayCreateInfo
	{
		std::shared_ptr<XRHIBuffer> VertexBuffer;
		std::shared_ptr<XRHIBuffer> IndexBuffer;

		BufferLayout VertexBufferLayout;
	};

	struct XRHIQueryCreateInfo
	{
		EQueryTarget QueryTarget = EQueryTarget::EQT_MAX_COUNT;
	};
}

namespace flora
{
	/// <summary>
	/// GPU resource
	/// </summary>
	class XRHIResource :public std::enable_shared_from_this<XRHIResource>
	{
	public:
		/*
			Non-copyable
		*/
		XRHIResource(const XRHIResource&) = delete;
		XRHIResource(XRHIResource&&) = delete;

		void operator=(const XRHIResource&) = delete;
		void operator=(XRHIResource&&) = delete;
	protected:
		XRHIResource() = default;
	public:
		virtual ~XRHIResource() = default;
	public:
		/*
			sharable
		*/
		template<typename T>
		T* DynamicCast()
		{
			return dynamic_cast<T*>(this);
		}
		template<typename T>
		std::shared_ptr<T> DynamicPointerCast()
		{
			return std::dynamic_pointer_cast<T>(this->shared_from_this());
		}
		template<typename T>
		T* StaticCast()
		{
			return static_cast<T*>(this);
		}
		template<typename T>
		std::shared_ptr<T> StaticPointerCast()
		{
			return std::static_pointer_cast<T>(this->shared_from_this());
		}
		template<typename T>
		T* ReinterpretCast()
		{
			return reinterpret_cast<T*>(this);
		}
	public:
		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const = 0;

		/// <summary>
		/// whether GPU resources have been allocated successfully.
		/// </summary>
		virtual bool IsValid() = 0;

		/// <summary>
		/// try allocate GPU resource
		/// </summary>
		virtual bool InitRHI() = 0;

		/// <summary>
		/// try deallocate GPU resource
		/// </summary>
		virtual bool ReleaseRHI() = 0;

		/// <summary>
		/// deep copy
		/// <param name="rhs"></param>
		/// </summary>
		virtual bool CopyFrom(std::shared_ptr<XRHIResource>) = 0;

		/// <summary>
		/// synchronize CPU resources to the GPU without checking the correctness of the resources.
		/// </summary>
		virtual bool UpdateRHI() = 0;
	};

	class XTexture;

	class XRHI
	{
	protected:
		XRHI() = default;

	public:
		template<ERHIFeatureLevel FL = flora::ERHIFeatureLevel::FL_OpenGL>
		static std::shared_ptr<XRHI> InitRHI(void*);

		virtual ~XRHI() {  }
	public:
		virtual std::shared_ptr<XRHIShader>				CreateShader(const XRHIShaderCreateInfo&) = 0;
		virtual std::shared_ptr<XRHIShaderProgram>		CreateShaderProgram(const XRHIShaderProgramCreateInfo&) = 0;
		virtual std::shared_ptr<XRHISampler>			CreateSampler(const XRHISamplerCreateInfo&) = 0;
		virtual std::shared_ptr<XRHITexture>			CreateTexture(const XRHITextureCreateInfo&) = 0;
		virtual std::shared_ptr<XRHIBuffer>				CreateBuffer(const XRHIBufferCreateInfo&) = 0;
		virtual std::shared_ptr<XRHIVertexArray>		CreateVertexArray(const XRHIVertexArrayCreateInfo&) = 0;
		virtual std::shared_ptr<XRHIRenderBuffer>		CreateRenderBuffer(const XRHIRenderBufferCreateInfo&) = 0;
		virtual std::shared_ptr<XRHIFrameBuffer>		CreateFrameBuffer(const XRHIFrameBufferCreateInfo&) = 0;
		virtual std::shared_ptr<XRHIQuery>				CreateQuery(const XRHIQueryCreateInfo&) = 0;
	public:
		virtual int GetFormatCompNum(flora::EFormat) = 0;
		virtual int GetDataTypeSize(flora::EDataType) = 0;
	public:
		static std::shared_ptr<XRHITextureCube> PanoramaToCubemap(uint32_t CubeSize, std::shared_ptr<XRHITexture2D>);
		static std::shared_ptr<XRHITexture2D> CubemapFaceToTexture2D(ECubeFace Face, std::shared_ptr<XRHITextureCube>);
		static std::shared_ptr<XTexture> CreateStaticTexture(std::shared_ptr<XRHITexture>);
		static std::vector< std::shared_ptr<XTexture>> CreateStaticTextureMipChain(std::shared_ptr<XRHITexture>);
		static std::shared_ptr<XRHITextureCube> FacesToCubemap(std::vector< std::shared_ptr<XRHITexture2D>>);

	public:
		virtual void SetupMaterialState(XMaterialState&) = 0;

		/*
			Operations
		*/
		virtual void DrawElement(EDrawMode, std::shared_ptr<XRHIVertexArray>, size_t) = 0;

		virtual void DrawElementInstanced(EDrawMode, std::shared_ptr<XRHIVertexArray>, size_t, uint32_t = 1) = 0;

		virtual void ClearFrameBuffer(EClearBufferBit) = 0;

		virtual void ClearColor(float r, float g, float b, float a) = 0;

		virtual void ResizeViewport(uint32_t, uint32_t, uint32_t, uint32_t) = 0;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="num_groups_x"></param>
		/// <param name="num_groups_y"></param>
		/// <param name="num_groups_z"></param>
		virtual void DispatchCompute(uint32_t, uint32_t, uint32_t) = 0;

		/// <summary>
		/// The offset into the buffer object currently bound to the GL_DISPATCH_INDIRECT_BUFFER buffer target at which the dispatch parameters are stored.
		/// </summary>
		/// <param name="offset"></param>
		virtual void DispatchComputeIndirect(uint32_t) = 0;

		/// <summary>
		///  indirect draw array
		/// </summary>
		/// <param name=""></param>
		virtual void DrawArrayIndirect(EDrawMode,std::shared_ptr<class XRHIDrawIndirectBuffer>) = 0;

		/// <summary>
		/// indirect draw element
		/// </summary>
		/// <param name=" draw mode"></param>
		/// <param name=" cmd buffer"></param>
		/// <param name=" ibo "></param>
		virtual void DrawElementIndirect(EDrawMode, std::shared_ptr<class XRHIDrawIndirectBuffer>, std::shared_ptr<XRHIBuffer>) = 0;

		virtual void Transition(std::shared_ptr<XRHIBuffer>, EBufferTarget) = 0;


		/// <summary>
		/// Memory barriers ensure that the visibility of all write operations prior to a synchronization point inserted in the rendering pipeline for subsequent read operations.
		/// In brief,flush memory write command.
		/// </summary>
		/// <param name="memory type"></param>
		virtual void MemoryBarrier(EMemoryBarrierBit) = 0;

		virtual void MemoryBarrier(EBufferTarget) = 0;

		virtual void FlushRenderCommand() = 0;

		virtual void FinishRenderCommand() = 0;

		virtual void AssertError(const std::string&) = 0;

	};

	XRHI* GetRHI();
}

namespace flora
{
	/// <summary>
	/// shader code
	/// </summary>
	class XRHIShader :public XRHIResource
	{
	protected:
		XRHIShader() = default;
		XRHIShader(EShaderType InShaderType) :ShaderType(InShaderType) {}
	public:
		~XRHIShader() override = default;
	public:
		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_Shader; }

		/// <summary>
		/// load source code
		/// </summary>
		/// <param name="glsl shade code"></param>
		virtual void SetSourceCode(const std::string&) = 0;

		/// <summary>
		/// get compile log (if failed)
		/// </summary>
		/// <param name=""></param>
		virtual void GetShaderInfoLog(std::string&) = 0;

		/// <summary>
		/// compile shader source code
		/// </summary>
		/// <returns></returns>
		virtual bool Compile() = 0;

		/// <summary>
		/// check
		/// </summary>
		/// <returns></returns>
		virtual bool IsCompiled() = 0;
	protected:
		EShaderType ShaderType = EShaderType::ST_COMPUTE_SHADER;
	};

	struct XMaterialState
	{
		//rasterizer
		bool bEnableScissorTest = false;
		int OffsetX = 0;
		int OffsetY = 0;
		int Width = 0;
		int Height = 0;

		//depth
		bool bEnableDepth = true;
		bool bIsDepthReadOnly = false;
		bool bEnableDepthBias = false;
		ECompareFunc DepthCompareFunc = ECompareFunc::CF_LESS;
		float BiasOffsetFactor = 0.f;
		float BiasOffsetScale = 0.f;

		//stencil
		bool bEnableStencil = false;
		bool bIsStencilReadOnly = true;
		ECompareFunc StencilCompareFunc = ECompareFunc::CF_LESS;
		EStencilOp SFailOp = EStencilOp::SP_KEEP;
		EStencilOp SPassZFailOp = EStencilOp::SP_KEEP;
		EStencilOp PassOp = EStencilOp::SP_KEEP;
		unsigned int StencilRef = 0;

		//blend
		bool bEnableBlend = false;
		EBlendEquation BlendEquation = EBlendEquation::BE_FUNC_ADD;
		EBlendFactor BlendSrc = EBlendFactor::BF_ONE;
		EBlendFactor BlendDest = EBlendFactor::BF_ZERO;

		//Cull
		bool bEnableCull = true;
		EFrontFace FrontFace = EFrontFace::FF_CW;
		ECullFace CullFace = ECullFace::CF_BACK;

		static XMaterialState Opacity() { return XMaterialState(); }
		static XMaterialState Transparency()
		{
			XMaterialState Ret;
			Ret.bEnableBlend = true;
			Ret.BlendSrc = EBlendFactor::BF_SRC_ALPHA;
			Ret.BlendDest = EBlendFactor::BF_ONE_MINUS_SRC_ALPHA;

			Ret.bEnableDepth = false;
			Ret.bEnableCull = false;

			return Ret;
		}
		static XMaterialState OpcatityStencil(int InStencil)
		{
			XMaterialState Ret;
			Ret.bEnableStencil = true;
			Ret.StencilRef = InStencil;
			return Ret;
		}
		static XMaterialState TransparencyStencil(int InStencil)
		{
			XMaterialState Ret = Transparency();
			Ret.bEnableStencil = true;
			Ret.StencilRef = InStencil;
			return Ret;
		}

	};

	/// <summary>
	/// material binary program
	/// </summary>
	class XRHIShaderProgram :public XRHIResource
	{
	protected:
		XRHIShaderProgram() = default;
	public:
		~XRHIShaderProgram() override = default;
	public:
		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_ShaderProgram; }

		/// <summary>
		/// Add shader
		/// </summary>
		/// <param name=""></param>
		virtual void AttachShader(std::shared_ptr<XRHIShader>)const = 0;

		/// <summary>
		/// link
		/// </summary>
		/// <returns></returns>
		virtual bool Link() const = 0;

		/// <summary>
		/// check link status
		/// </summary>
		/// <returns></returns>
		virtual bool IsLinked() const = 0;

		/// <summary>
		/// bind render context
		/// </summary>
		virtual void Use()  const = 0;

		/// <summary>
		/// get link error info log
		/// </summary>
		virtual void GetShaderProgramInfoLog(std::string&) const = 0;

		/// <summary>
		/// </summary>
		/// <param name="buffer"></param>
		virtual void SetBuffer(std::shared_ptr<flora::XRHIBuffer>) const = 0;

		/// <summary>
		/// glsl uniform samplerXX
		/// </summary>
		/// <param name="name"></param>
		/// <param name="texture unit"></param>
		/// <param name="texture object"></param>
		/// <param name="sampler"></param>
		virtual void SetTexture(const char*, uint32_t, std::shared_ptr<class XRHITexture>, std::shared_ptr<class XRHISampler>) const = 0;

		/// <summary>
		/// </summary>
		/// <param name="texture"></param>
		/// <param name="image_unit"></param>
		/// <param name="mip"></param>
		/// <param name="access_policy"></param>
		virtual void SetTextureImage(std::shared_ptr<class XRHITexture>, uint32_t, uint32_t = 0, uint32_t = 0/* = GL_READ_WRITE*/) const = 0;

		/// <summary>
		/// </summary>
		/// <param name="texture"></param>
		/// <param name="image_unit"></param>
		/// <param name="mip"></param>
		/// <param name="access_policy"></param>
		virtual void SetTextureImage(std::shared_ptr<class XRHITexture>, uint32_t, uint32_t = 0, EAccessPolicy = EAccessPolicy::AP_READ_WRITE) const = 0;

		virtual void SetBool1(const char*, bool) const = 0;
		virtual void SetBool2(const char*, bool, bool) const = 0;
		virtual void SetBool3(const char*, bool, bool, bool) const = 0;
		virtual void SetBool4(const char*, bool, bool, bool, bool) const = 0;

		virtual void SetInt1(const char*, int) const = 0;
		virtual void SetInt2(const char*, int, int) const = 0;
		virtual void SetInt3(const char*, int, int, int) const = 0;
		virtual void SetInt4(const char*, int, int, int, int) const = 0;

		virtual void SetUInt1(const char*, uint32_t) const = 0;
		virtual void SetUInt2(const char*, uint32_t, uint32_t) const = 0;
		virtual void SetUInt3(const char*, uint32_t, uint32_t, uint32_t) const = 0;
		virtual void SetUInt4(const char*, uint32_t, uint32_t, uint32_t, uint32_t) const = 0;

		virtual void SetFloat1(const char*, float) const = 0;
		virtual void SetFloat2(const char*, float, float) const = 0;
		virtual void SetFloat3(const char*, float, float, float) const = 0;
		virtual void SetFloat4(const char*, float, float, float, float) const = 0;

		virtual void SetFloat1Array(const char*, size_t, float*) const = 0;
		virtual void SetFloat2Array(const char*, size_t, float*) const = 0;
		virtual void SetFloat3Array(const char*, size_t, float*) const = 0;
		virtual void SetFloat4Array(const char*, size_t, float*) const = 0;

		virtual void SetInt1Array(const char*, size_t, int*) const = 0;
		virtual void SetInt2Array(const char*, size_t, int*) const = 0;
		virtual void SetInt3Array(const char*, size_t, int*) const = 0;
		virtual void SetInt4Array(const char*, size_t, int*) const = 0;

		virtual void SetUInt1Array(const char*, size_t, uint32_t*) const = 0;
		virtual void SetUInt2Array(const char*, size_t, uint32_t*) const = 0;
		virtual void SetUInt3Array(const char*, size_t, uint32_t*) const = 0;
		virtual void SetUInt4Array(const char*, size_t, uint32_t*) const = 0;

		virtual void SetMatrixFloat2x2Array(const char*, size_t, bool, float*) const = 0;
		virtual void SetMatrixFloat3x3Array(const char*, size_t, bool, float*) const = 0;
		virtual void SetMatrixFloat4x4Array(const char*, size_t, bool, float*) const = 0;
		virtual void SetMatrixFloat2x3Array(const char*, size_t, bool, float*) const = 0;
		virtual void SetMatrixFloat3x2Array(const char*, size_t, bool, float*) const = 0;
		virtual void SetMatrixFloat2x4Array(const char*, size_t, bool, float*) const = 0;
		virtual void SetMatrixFloat4x2Array(const char*, size_t, bool, float*) const = 0;
		virtual void SetMatrixFloat3x4Array(const char*, size_t, bool, float*) const = 0;
		virtual void SetMatrixFloat4x3Array(const char*, size_t, bool, float*) const = 0;
	};

	/// <summary>
	/// texture sampler state
	/// </summary>
	class XRHISampler :public XRHIResource
	{
		friend class XRHITexture;
	protected:
		XRHISampler() = default;
	public:
		~XRHISampler() override = default;
	public:
		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_Sampler; }

		/// <summary>
		/// bind sampler
		/// </summary>
		/// <param name="tex unit"></param>
		virtual void Bind(uint32_t) = 0;
		virtual void UnBind(uint32_t) = 0;

		/*virtual void SetMinFilter(EFilterMode)const = 0;
		virtual void SetMagFilter(EFilterMode)const = 0;

		virtual void SetWrapR(EWrapMode)const = 0;
		virtual void SetWrapS(EWrapMode)const = 0;
		virtual void SetWrapT(EWrapMode)const = 0;

		virtual void SetMinLod(uint8_t)const = 0;
		virtual void SetMaxLod(uint8_t)const = 0;

		virtual void SetLodBias(float)const = 0;

		virtual void SetBorderColor(float, float, float, float)const = 0;

		virtual void SetCompareMode(ETextureCompareMode)const = 0;
		virtual void SetCompareFunc(ECompareFunc)const = 0;*/

		virtual void SetSamplerState(const XRHISamplerCreateInfo&) = 0;
	};

	class XTexture;

	/// <summary>
	/// rhi texture base type
	/// </summary>
	class XRHITexture :public XRHIResource
	{
	protected:
		XRHITexture() = default;
		XRHITexture(uint32_t InMipLevels, EInternalFormat InInternalFormat) :MipLevels(InMipLevels), InternalFormat(InInternalFormat) {}
	public:
		~XRHITexture() override = default;
	public:
		virtual void Bind() = 0;

		virtual void UnBind() = 0;

	public:
		inline auto GetMipLevels()		const { return MipLevels; }
		inline auto GetInternalFormat()	const { return InternalFormat; }

		/*virtual void SetMinFilter(EFilterMode)const = 0;
		virtual void SetMagFilter(EFilterMode)const = 0;

		virtual void SetWrapR(EWrapMode)const = 0;
		virtual void SetWrapS(EWrapMode)const = 0;
		virtual void SetWrapT(EWrapMode)const = 0;

		virtual void SetMinLod(uint8_t)const = 0;
		virtual void SetMaxLod(uint8_t)const = 0;

		virtual void SetLodBias(float)const = 0;

		virtual void SetBorderColor(float, float, float, float)const = 0;

		virtual void SetCompareMode(ETextureCompareMode)const = 0;
		virtual void SetCompareFunc(ECompareFunc)const = 0;*/

		virtual void SetSamplerState(const XRHISamplerCreateInfo&) = 0;

		virtual uint32_t GetHandle()const = 0;
	protected:
		/// <summary>
		/// mip level counts
		/// </summary>
		uint32_t MipLevels = 1;

		/// <summary>
		/// texture gpu format
		/// </summary>
		EInternalFormat InternalFormat = EInternalFormat::IF_RGBA32F;/* = GL_RGBA32F;*/
	};

	/// <summary>
	/// 2d texture
	/// </summary>
	class XRHITexture2D :public XRHITexture
	{
	protected:
		XRHITexture2D() = default;
		XRHITexture2D(uint32_t InMipLevels, EInternalFormat InInternalFormat, uint32_t InX, uint32_t InY) :XRHITexture(InMipLevels, InInternalFormat)
		{
			SizeX[0] = InX;
			SizeY[0] = InY;
		}
	public:
		~XRHITexture2D() override = default;
	public:
		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_Texture2D; }

		/// <summary>
		/// read pixel data from specified mip level
		/// </summary>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out buffer size"></param>
		/// <param name="out pixel buffer"></param>
		virtual void ReadPixels(int level, EFormat format, EDataType type, void* pixels) = 0;


		template<typename T>
		bool ReadPixels(std::vector<T>& OutPixels,int level = 0)
		{
			auto ReadFormat = MatchInternalFormat(GetInternalFormat());

			int SizeX = GetSizeX(level);
			int SizeY = GetSizeY(level);
			int DataTypeSize = GetRHI()->GetDataTypeSize(ReadFormat.second);
			int FormatCompNum = GetRHI()->GetFormatCompNum(ReadFormat.first);
			int BufferSize = SizeX * SizeY * DataTypeSize * FormatCompNum;

			if (!MatchDataType<T>(ReadFormat.second))return false;

			std::byte* Buffer = (std::byte*)malloc(BufferSize);
			{
				ReadPixels(level, ReadFormat.first, ReadFormat.second, Buffer);
			}

			OutPixels.clear();
			OutPixels.resize(BufferSize / sizeof(T));

			memcpy(OutPixels.data(), Buffer, BufferSize);

			OutPixels.shrink_to_fit();

			free(Buffer);

			return true;
		}

		/// <summary>
		/// set pixel data
		/// </summary>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out data"></param>
		virtual void SetPixels(uint32_t, EFormat, EDataType, const void*) = 0;

	public:
		virtual void AddNewMip(uint32_t InSizeX, uint32_t InSizeY) = 0;
	public:
		inline uint32_t GetSizeX(int MipLevel = 0) const { return SizeX[MipLevel]; }
		inline uint32_t GetSizeY(int MipLevel = 0) const { return SizeY[MipLevel]; }

	protected:
		uint32_t SizeX[MAX_MIP_LEVEL] = {};
		uint32_t SizeY[MAX_MIP_LEVEL] = {};
	};

	/// <summary>
	/// 3d texture(volume render target)
	/// </summary>
	class XRHIVolumeTexture :public XRHITexture
	{
	protected:
		XRHIVolumeTexture() = default;
		XRHIVolumeTexture(uint32_t InMipLevels, EInternalFormat InInternalFormat, uint32_t InX, uint32_t InY, uint32_t InZ) :XRHITexture(InMipLevels, InInternalFormat), SizeX(InX), SizeY(InY), SizeZ(InZ) {}
	public:
		~XRHIVolumeTexture() override = default;
	public:
		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_Texture3D; }

		/// <summary>
		/// read pixel data from specified mip level
		/// </summary>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out buffer size"></param>
		/// <param name="out pixel buffer"></param>
		virtual void ReadPixels(int level, EFormat format, EDataType type, void* pixels) = 0;

		/// <summary>
		/// set pixel data
		/// </summary>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out data"></param>
		virtual void SetPixels(uint32_t, EFormat, EDataType, const void*) = 0;
	public:
		inline auto GetSizeX() const { return SizeX; }
		inline auto GetSizeY() const { return SizeY; }
		inline auto GetSizeZ() const { return SizeZ; }

	protected:
		uint32_t SizeX = 0;
		uint32_t SizeY = 0;
		uint32_t SizeZ = 0;
	};

	/// <summary>
	/// 3d texture(volume render target)
	/// </summary>
	class XRHITextureCube :public XRHITexture
	{
	protected:
		XRHITextureCube() = default;
		XRHITextureCube(uint32_t InMipLevels, EInternalFormat InInternalFormat, uint32_t InX) :XRHITexture(InMipLevels, InInternalFormat), SizeX(InX) {}
	public:
		~XRHITextureCube() override = default;
	public:
		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_TextureCube; }

		/// <summary>
		/// read pixel data from specified mip level
		/// </summary>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out buffer size"></param>
		/// <param name="out pixel buffer"></param>
		virtual void ReadPixels(int level, EFormat format, EDataType type, void* pixels) = 0;

		/// <summary>
		/// set pixel data
		/// </summary>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out data"></param>
		virtual void SetPixels(uint32_t, EFormat, EDataType, const void*) = 0;

		/// <summary>
		/// read pixel data from specified mip level and face 
		/// </summary>
		/// <param name="cube face"></param>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out buffer size"></param>
		/// <param name="out pixel buffer"></param>
		virtual void ReadPixels(ECubeFace face, int level, EFormat format, EDataType type, void* pixels) = 0;

		/// <summary>
		/// set pixel data and face 
		/// </summary>
		/// <param name="cube face"></param>
		/// <param name="mip level"></param>
		/// <param name="out format"></param>
		/// <param name="out type"></param>
		/// <param name="out data"></param>
		virtual void SetPixels(ECubeFace face, uint32_t, EFormat, EDataType, const void*) = 0;
	public:
		inline auto GetSizeX() const { return SizeX; }

	protected:
		uint32_t SizeX = 0;
	};

	/// <summary>
	/// gpu buffer type
	/// </summary>
	class XRHIBuffer :public XRHIResource
	{
		friend class XRHI;
		friend class XOpenGLRHI;
	protected:
		XRHIBuffer() = default;
		XRHIBuffer(uint32_t InBindingSlot) :BindingSlot(InBindingSlot) {}
	public:
		~XRHIBuffer() override = default;
	public:
		/// <summary>
		/// bind buffer
		/// </summary>
		virtual void Bind() = 0;

		/// <summary>
		/// unbind buffer
		/// </summary>
		virtual void UnBind() = 0;

		/// <summary>
		/// load data
		/// </summary>
		/// <param name="raw data size"></param>
		/// <param name="raw data pointer"></param>
		/// <param name="usage"></param>
		/// <returns></returns>
		virtual bool SetData(size_t, const void*, EBufferUsage) = 0; //GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY

		/// <summary>
		/// update sub data
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="size"></param>
		/// <param name="data"></param>
		/// <returns></returns>
		virtual bool UpdateSubData(size_t,size_t, const void*) = 0;

		/// <summary>
		/// begin read buffer data:
		/// policy:
		///		GL_READ_ONLY    
		///		GL_WRITE_ONLY    
		///		GL_READ_WRITE    
		/// </summary>
		/// <param name="accessPolicy"></param>
		/// <returns></returns>
		virtual void* Map(EAccessPolicy /*= GL_READ_WRITE*/) = 0;

		/// <summary>
		/// begin read buffer data:
		/// policy:
		///		GL_READ_ONLY    
		///		GL_WRITE_ONLY    
		///		GL_READ_WRITE 
		/// </summary>
		/// <param name="offset"></param>
		/// <param name="length"></param>
		/// <param name="accessPolicy"></param>
		/// <returns></returns>
		virtual void* MapRange(int, size_t, EAccessPolicy) = 0;

		/// <summary>
		/// end read buffer data
		/// </summary>
		/// <returns></returns>
		virtual bool Unmap() = 0;

		/// <summary>
		/// bind to slot
		/// </summary>
		/// <param name=""></param>
		virtual void BindBufferBase() = 0;
	protected:
		virtual uint32_t GetHandle()const = 0;

	protected:
		uint32_t BindingSlot = 0;
	};

	/// <summary>
	/// vbo
	/// </summary>
	class XRHIVertexBuffer :public XRHIBuffer
	{
	protected:
		XRHIVertexBuffer() = default;
		XRHIVertexBuffer(uint32_t InBindingSlot) :XRHIBuffer(InBindingSlot) {}
	public:
		~XRHIVertexBuffer() override = default;

		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_VertexBuffer; }
	};

	/// <summary>
	/// ibo
	/// </summary>
	class XRHIIndexBuffer :public XRHIBuffer
	{
	protected:
		XRHIIndexBuffer() = default;
		XRHIIndexBuffer(uint32_t InBindingSlot) :XRHIBuffer(InBindingSlot) {}
	public:
		~XRHIIndexBuffer() override = default;

		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_IndexBuffer; }
	};

	/// <summary>
	/// ssbo
	/// </summary>
	class XRHIShaderStorageBuffer :public XRHIBuffer
	{
	protected:
		XRHIShaderStorageBuffer() = default;
		XRHIShaderStorageBuffer(uint32_t InBindingSlot) :XRHIBuffer(InBindingSlot) {}
	public:
		~XRHIShaderStorageBuffer() override = default;

		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_ShaderStorageBuffer; }
	};

	/// <summary>
	/// uniform block
	/// </summary>
	class XRHIUniformBuffer :public XRHIBuffer
	{
	protected:
		XRHIUniformBuffer() = default;
		XRHIUniformBuffer(uint32_t InBindingSlot) :XRHIBuffer(InBindingSlot) {}
	public:
		~XRHIUniformBuffer() override = default;

		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_UniformBuffer; }
	};

	/// <summary>
	/// uniform block
	/// </summary>
	class XRHIAtomicCounterBuffer :public XRHIBuffer
	{
	protected:
		XRHIAtomicCounterBuffer() = default;
		XRHIAtomicCounterBuffer(uint32_t InBindingSlot) :XRHIBuffer(InBindingSlot) {}
	public:
		~XRHIAtomicCounterBuffer() override = default;

		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_AtomicCounterBuffer; }
	};

	struct XDrawElementsIndirectCommand
	{
		unsigned int  count;
		unsigned int  instanceCount;
		unsigned int  firstIndex;
		int  baseVertex;
		unsigned int  baseInstance;
	};

	struct XDrawArraysIndirectCommand
	{
		unsigned int  count;
		unsigned int  instanceCount;
		unsigned int  first;
		unsigned int  baseInstance;
	} ;

	struct XDispatchIndirectCommand
	{
		unsigned int  num_groups_x;
		unsigned int  num_groups_y;
		unsigned int  num_groups_z;
	} ;

	/// <summary>
	/// uniform block
	/// </summary>
	class XRHIDrawIndirectBuffer :public XRHIBuffer
	{
	protected:
		XRHIDrawIndirectBuffer() = default;
		XRHIDrawIndirectBuffer(uint32_t InBindingSlot) :XRHIBuffer(InBindingSlot) {}
	public:
		~XRHIDrawIndirectBuffer() override = default;

		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_DrawIndirectBuffer; }


	public:
		template<typename T>
		void SetZero(EBufferUsage Usage, int N)
		{
			CmdNum = N;

			int Size = sizeof(T) * N;

			T* Tmp = (T*)malloc(Size);
			memset(Tmp, 0, Size);
			SetData(Size, Tmp, Usage);

			free(Tmp);
		};

		template<typename T>
		void ReadBack(std::vector<T>& CmdBuffer)
		{
			CmdBuffer.clear();
			CmdBuffer.resize(CmdNum);

			GetRHI()->MemoryBarrier(MBB_COMMAND_BARRIER_BIT);

			if (T* Buffer = (T*)Map(EAccessPolicy::AP_READ_ONLY))
			{
				memcpy(CmdBuffer.data(), Buffer, sizeof(T) * CmdNum);
				Unmap();
			}
		};

		inline int GetCmdNum()const { return CmdNum; };
	protected:
		int CmdNum;
	};

	/// <summary>
	/// uniform block
	/// </summary>
	class XRHIDispatchIndirectBuffer :public XRHIBuffer
	{
	protected:
		XRHIDispatchIndirectBuffer() = default;
		XRHIDispatchIndirectBuffer(uint32_t InBindingSlot) :XRHIBuffer(InBindingSlot) {}
	public:
		~XRHIDispatchIndirectBuffer() override = default;

		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_DispatchIndirectBuffer; }
	};

	/// <summary>
	/// vao
	/// </summary>
	class XRHIVertexArray :public XRHIResource
	{
	protected:
		XRHIVertexArray() = default;
	public:
		~XRHIVertexArray() override = default;
	public:
		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_VertexArray; }

		virtual void Bind()   const = 0;

		virtual void UnBind() const = 0;

		virtual void Setup(std::shared_ptr<XRHIBuffer>, std::shared_ptr<XRHIBuffer>, const BufferLayout&) = 0;
	};

	class XRHIRenderBuffer :public XRHIResource
	{
	protected:
		XRHIRenderBuffer() = default;
		XRHIRenderBuffer(uint32_t InX, uint32_t InY, EInternalFormat InInternalFormat) :SizeX(InX), SizeY(InY), InternalFormat(InInternalFormat) {}
	public:
		~XRHIRenderBuffer() override = default;
	public:
		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_RenderBuffer; }

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		inline EInternalFormat GetInternalFormat()const { return InternalFormat; }
		inline uint32_t GetSizeX()const { return SizeX; }
		inline uint32_t GetSizeY()const { return SizeY; }

	protected:
		uint32_t SizeX = 0;
		uint32_t SizeY = 0;

		EInternalFormat InternalFormat = EInternalFormat::IF_DEPTH24_STENCIL8;// = GL_DEPTH24_STENCIL8;
	};

	//frame buffer color output slot
	struct XFrameBufferAttachment
	{
		std::string Name;
		std::shared_ptr<XRHITexture> ColorBuffer;
		uint32_t MipLevel = 0;

		ECubeFace CubeFace;

		XFrameBufferAttachment() {}
		XFrameBufferAttachment(std::string InName, std::shared_ptr<XRHITexture> InColorBuffer, uint32_t InMipLevel, ECubeFace InCubeFace = PX)
			:Name(InName), ColorBuffer(InColorBuffer), MipLevel(InMipLevel), CubeFace(InCubeFace) {}
	};

	class XRHIFrameBuffer :public XRHIResource
	{
	protected:
		XRHIFrameBuffer() = default;
		XRHIFrameBuffer(const std::shared_ptr<XRHIRenderBuffer> InDepthStencilAttachment) :DepthStencilAttachment(InDepthStencilAttachment) {}

	public:
		~XRHIFrameBuffer() override = default;
	public:
		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_FrameBuffer; }

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual bool IsComplete() = 0;

		virtual void ClearBuffer(EClearBufferBit InBufferBit) = 0;

		/// <summary>
		/// clear color output
		/// </summary>
		/// <param name="slot"></param>
		/// <param name="r"></param>
		/// <param name="g"></param>
		/// <param name="b"></param>
		/// <param name="a"></param>
		virtual void ClearColor(int, float, float, float, float) = 0;
	public:
		inline const auto& GetColorAttachments()const { return ColorAttachments; }
		inline const auto& GetDepthStencilAttachment()const { return DepthStencilAttachment; }
		inline const auto& GetEnabledOutputAttachments() const { return DrawBuffers; }

		inline void SetColorAttachment(const std::vector<XFrameBufferAttachment>& InColorAttachments) { ColorAttachments = InColorAttachments; }
		inline void EnableOutputAttachments(const std::vector<uint32_t>& InDrawBuffers) { DrawBuffers = InDrawBuffers; }

	protected:
		/// <summary>
		/// key:texture
		/// value:bind mip level
		/// </summary>
		std::vector<XFrameBufferAttachment> ColorAttachments;
		std::shared_ptr<XRHIRenderBuffer> DepthStencilAttachment;

		std::vector<uint32_t> DrawBuffers;
	};

	class XRHIQuery :public XRHIResource
	{
	protected:
		XRHIQuery() = default;
	public:
		~XRHIQuery() override = default;
	public:
		/// <summary>
		/// resource type
		/// </summary>
		/// <returns></returns>
		virtual ERHIResourceType GetType()const { return ERHIResourceType::RT_Query; }

		virtual EQueryTarget GetQueryTarget()const = 0;

		virtual void BeginQuery() = 0;
		virtual void EndQuery() = 0;
	};

	class XRHITimeQuery : public XRHIQuery
	{
	protected:
		XRHITimeQuery() = default;
	public:
		virtual EQueryTarget GetQueryTarget()const override { return EQueryTarget::EQT_TIME_ELAPSED; };

		virtual double GetElapsedTime()const = 0;
	};

	class XRHIPrimitiveQuery : public XRHIQuery
	{
	protected:
		XRHIPrimitiveQuery() = default;
	public:
		virtual EQueryTarget GetQueryTarget()const override { return EQueryTarget::EQT_PRIMITIVES_GENERATED; };

		virtual unsigned int GetPrimitivesGenerated()const = 0;
	};


}

// gpu buffer
namespace flora
{
	struct FBaseBuffer
	{
	public:
		std::shared_ptr<XRHIBuffer> GetRef() { return Gpu; };
	protected:
		std::shared_ptr<XRHIBuffer> Gpu = nullptr;
	};

	template<int Slot, EBufferUsage Usage>
	struct FByteAddressBuffer:public FBaseBuffer
	{
		void InitRHI(void* Buffer, size_t BufferSize)
		{
			if (!Gpu)
			{
				XRHIBufferCreateInfo Info;
				Info.BindingSlot = Slot;
				Info.BufferTarget = EBufferTarget::BT_SHADER_STORAGE_BUFFER;

				Gpu = GetRHI()->CreateBuffer(Info);
				Gpu->SetData(BufferSize, Buffer, Usage);

				if (Data || Size > 0)
				{
					free(Data);
					Data = nullptr;
					Size = 0;
				}

				Data = malloc(BufferSize);
				Size = BufferSize;

				if (Buffer)memcpy(Data, Buffer, BufferSize);
				
			}
		}

		bool ReadBack(void* Buffer, size_t BufferSize, EAccessPolicy Policy)
		{
			GetRHI()->MemoryBarrier(MBB_SHADER_STORAGE_BARRIER_BIT);

			if (void* Ptr = Gpu->Map(Policy))
			{
				memcpy(Buffer, Ptr, BufferSize);

				Gpu->Unmap();

				return Ptr != nullptr;
			}

			return false;
		}

		template<typename T>
		bool ReadBack(std::vector<T>& InoutData, EAccessPolicy Policy = EAccessPolicy::AP_READ_ONLY)
		{
			InoutData.clear();

			ReadBack(Data,Size, Policy);
			if (Size % sizeof(T) == 0)
			{
				InoutData.resize(Size / sizeof(T));
				memcpy(InoutData.data(), Data, Size);

				return true;
			};

			return false;
		}
	private:
		void* Data = nullptr;
		size_t Size = 0;
	};

	template<int Slot, typename T, EBufferUsage Usage, EBufferTarget Target = EBufferTarget::BT_SHADER_STORAGE_BUFFER>
	struct FStructuredBuffer :public FBaseBuffer
	{
		void Clear()
		{
			PackedData.clear();
			Gpu = nullptr;
		}

		void Resize(int N)
		{
			Clear();
			PackedData.resize(N);
		}

		void Resize(int N,T Data)
		{
			Clear();
			PackedData.resize(N,Data);
		}

		void InitRHI()
		{
			Gpu = nullptr;

			if (!Gpu)
			{
				XRHIBufferCreateInfo Info;
				Info.BindingSlot = Slot;
				Info.BufferTarget = Target;

				Gpu = GetRHI()->CreateBuffer(Info);
				Gpu->SetData(PackedData.size() * sizeof(T), PackedData.data(), Usage);
			}
		}

		void UpdateSubData(int idx)
		{
			if (idx < PackedData.size())
			{
				Gpu->UpdateSubData(sizeof(T) * idx, sizeof(T), &(PackedData[idx]));
			}
		}

		void UpdateAllData()
		{
			if (PackedData.size() > 0)
			{
				Gpu->UpdateSubData(0, sizeof(T) * PackedData.size(), &(PackedData[0]));
			}
		}

		bool ReadBack()
		{
			GetRHI()->MemoryBarrier(Target);

			if (void* Ptr = Gpu->Map(EAccessPolicy::AP_READ_ONLY))
			{
				if (Ptr)
				{
					memcpy(PackedData.data(), Ptr, PackedData.size() * sizeof(T));
				}

				Gpu->Unmap();

				return Ptr != nullptr;
			}

			return false;
		}

		T& operator[](int id)
		{
			return PackedData[id];
		}

		T* GetData()
		{
			return PackedData.data();
		}

		inline int GetNum()const { return PackedData.size(); };
	private:
		std::vector<T> PackedData;
	};

	template<int Slot>
	struct FCommandBuffer :public FBaseBuffer
	{
		void InitRHI(int N)
		{
			XRHIBufferCreateInfo Info;
			Info.BindingSlot = Slot;
			Info.BufferTarget = EBufferTarget::BT_DRAW_INDIRECT_BUFFER;

			Gpu = GetRHI()->CreateBuffer(Info);
			Gpu->DynamicPointerCast<XRHIDrawIndirectBuffer>()->SetZero<XDrawElementsIndirectCommand>(EBufferUsage::BU_DYNAMIC_COPY, N);
		}

		template<typename T>
		void ReadBack(std::vector<T>& CmdBuffer)
		{
			Gpu->DynamicPointerCast<XRHIDrawIndirectBuffer>()->ReadBack(CmdBuffer);
		}

		inline int GetCmdNum()const { return Gpu->DynamicPointerCast<XRHIDrawIndirectBuffer>()->GetCmdNum(); }
	};
}

// profiling
namespace flora
{
	typedef unsigned int EQueryCategory;
	enum EQueryCategory_
	{
		EQC_TIME ,
		EQC_PRIMITIVE ,
		EQC_MAX_COUNT ,
	};

	struct FQueryResult 
	{
		FQueryResult() = default;
		FQueryResult(const char* InName) :Name(InName) {};
		FQueryResult(FQueryResult* Rhs) :Name(Rhs->Name) {};
		std::string Name;
	};

	class FQueryManager :public XSingleton<FQueryManager>
	{
	public:
		template<typename T>
		void Register(EQueryCategory Cate,T* Result)
		{
			bool bFind = false;

			for (auto& Res : Categories[Cate])
			{
				if (Result->Name == Res->Name)
				{
					bFind = true;
					Res = std::make_shared<T>(Result);
					break;
				}
			}

			if (!bFind)
			{
				Categories[Cate].push_back(std::make_shared<T>(Result));
			}
			
		}
	public:
		const std::vector<std::shared_ptr<FQueryResult>>& operator[](EQueryCategory Category)const { return Categories[Category]; };
	protected:
		std::vector<std::shared_ptr<FQueryResult>> Categories[EQC_MAX_COUNT];
	};

#define SCOPE_TIMER(Name) flora::FScopeTimer Timer(#Name);
	struct FScopeTimer
	{
		struct TimeResult :public FQueryResult
		{
			TimeResult() = default;

			TimeResult(const char* InName) :FQueryResult(InName) {};
			TimeResult(TimeResult* Rhs) :FQueryResult(Rhs), ElapsedTime(Rhs->ElapsedTime){};

			double ElapsedTime = 0.0;
		};

		FScopeTimer(const char* InName) :Name(InName)
		{
			XRHIQueryCreateInfo Info;
			Info.QueryTarget = EQueryTarget::EQT_TIME_ELAPSED;
			Query = GetRHI()->CreateQuery(Info)->DynamicPointerCast<XRHITimeQuery>();
			Query->BeginQuery();
		}

		virtual ~FScopeTimer()
		{
			Query->EndQuery();

			TimeResult* Result = new TimeResult(Name.c_str());
			Result->ElapsedTime = Query->GetElapsedTime() / 1e6;

			FQueryManager::Get().Register(EQC_TIME, Result);

			Query = nullptr;
		}
	private:
		std::shared_ptr<XRHITimeQuery> Query;
		std::string Name;
	};
}