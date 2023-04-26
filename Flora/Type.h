#pragma once
typedef unsigned int EWrapMode;
typedef unsigned int EFilterMode;
typedef unsigned int EInternalFormat;
typedef unsigned int ECompareMethod;
typedef unsigned int EOperation;
typedef unsigned int EBlendFactor;
typedef unsigned int EBlendFunctionMode;
typedef unsigned int EFrontFaceDef;
typedef unsigned int ECullMode;
typedef unsigned int EDrawMode;
typedef unsigned int ETextureTarget;
typedef unsigned int ERenderCommandType;

//ERenderCommandType
enum
{
	E_RHI_OpenGL,
	E_RHI_Vulkan,
	E_RHI_Dx12,
	E_RHI_Metal,
};

//EWrapMode_
enum :unsigned int
{
	EWrapMode_REPEAT,
	EWrapMode_MIRRORED_REPEAT,
	EWrapMode_CLAMP_TO_EDGE,
	EWrapMode_CLAMP_TO_BORDER,
};

//EFilterMode_
enum :unsigned int
{
	EFilterMode_NEAREST,
	EFilterMode_LINEAR,
	EFilterMode_NEAREST_MIPMAP_NEAREST,
	EFilterMode_LINEAR_MIPMAP_NEAREST,
	EFilterMode_NEAREST_MIPMAP_LINEAR,
	EFilterMode_LINEAR_MIPMAP_LINEAR
};

//EInternalFormat_
enum :unsigned int
{
	/*base format*/
	EInternalFormat_DEPTH_COMPONENT,
	EInternalFormat_RED,
	EInternalFormat_DEPTH_STENCIL,
	EInternalFormat_RG,
	EInternalFormat_RGB,
	EInternalFormat_RGBA,

	/*sized format*/
	EInternalFormat_R8,
	EInternalFormat_R16,
	EInternalFormat_R16F,
	EInternalFormat_R32F,
	EInternalFormat_R8I,
	EInternalFormat_R16I,
	EInternalFormat_R32I,
	EInternalFormat_R8UI,
	EInternalFormat_R16UI,
	EInternalFormat_R32UI,
	EInternalFormat_RG8,
	EInternalFormat_RG16,
	EInternalFormat_RG16F,
	EInternalFormat_RG32F,
	EInternalFormat_RG8I,
	EInternalFormat_RG16I,
	EInternalFormat_RG32I,
	EInternalFormat_RG8UI,
	EInternalFormat_RG16UI,
	EInternalFormat_RG32UI,
	EInternalFormat_RGB32F,
	EInternalFormat_RGB32I,
	EInternalFormat_RGB32UI,
	EInternalFormat_RGBA8,
	EInternalFormat_RGBA16,
	EInternalFormat_RGBA16F,
	EInternalFormat_RGBA32F,
	EInternalFormat_RGB8I,
	EInternalFormat_RGBA16I,
	EInternalFormat_RGBA32I,
	EInternalFormat_RGBA8UI,
	EInternalFormat_RGBA16UI,
	EInternalFormat_RGBA32UI,
};

//ECompareMethod_
enum :unsigned int {
	ECompareMethod_NEVER,
	ECompareMethod_LESS,
	ECompareMethod_EQUAL,
	ECompareMethod_LEQUAL,
	ECompareMethod_GREATER,
	ECompareMethod_NOTEQUAL,
	ECompareMethod_GEQUAL,
	ECompareMethod_ALWAYS,
};

//EOperation_
enum :unsigned int
{
	EOperation_KEEP,
	EOperation_ZERO,
	EOperation_REPLACE,
	EOperation_INCR,
	EOperation_INCR_WRAP,
	EOperation_DECR,
	EOperation_DECR_WRAP,
	EOperation_INVERT
};

//EBlendFactor_
enum :unsigned int {
	EBlendFactor_ZERO,
	EBlendFactor_ONE,
	EBlendFactor_SRC_COLOR,
	EBlendFactor_ONE_MINUS_SRC_COLOR,
	EBlendFactor_DST_COLOR,
	EBlendFactor_ONE_MINUS_DST_COLOR,
	EBlendFactor_SRC_ALPHA,
	EBlendFactor_ONE_MINUS_SRC_ALPHA,
	EBlendFactor_DST_ALPHA,
	EBlendFactor_ONE_MINUS_DST_ALPHA,
	EBlendFactor_CONSTANT_COLOR,
	EBlendFactor_ONE_MINUS_CONSTANT_COLOR,
	EBlendFactor_CONSTANT_ALPHA,
	EBlendFactor_ONE_MINUS_CONSTANT_ALPHA
};

//EBlendFunctionMode_
enum :unsigned int {
	EBlendFunctionMode_ADD,
	EBlendFunctionMode_SUB,
	EBlendFunctionMode_REVERSE_SUB
};

//EFrontFaceDef_
enum :unsigned int {
	EFrontFaceDef_CCW,
	EFrontFaceDef_CW
};

//ECullMode_
enum :unsigned int {
	ECullMode_FRONT,
	ECullMode_BACK,
	ECullMode_FRONT_AND_BACK
};

//EDrawMode_
enum :unsigned int
{
	EDrawMode_POINTS,
	EDrawMode_LINES,
	EDrawMode_LINE_LOOP,
	EDrawMode_LINE_STRIP,
	EDrawMode_TRIANGLES,
	EDrawMode_TRIANGLE_STRIP,
	EDrawMode_TRIANGLE_FAN,
	EDrawMode_QUADS
};


//ETextureTarget_
enum :unsigned int
{
	ETextureTarget_1D,
	ETextureTarget_2D,
	ETextureTarget_2D_Multisample,
	ETextureTarget_3D,
	ETextureTarget_3D_Multisample,
	ETextureTarget_Cubic,
	ETextureTarget_Cubic_PX,
	ETextureTarget_Cubic_NX,
	ETextureTarget_Cubic_PY,
	ETextureTarget_Cubic_NY,
	ETextureTarget_Cubic_PZ,
	ETextureTarget_Cubic_NZ,
	ETextureTarget_MAX
};