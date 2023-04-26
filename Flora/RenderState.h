#pragma once
#include "Type.h"
#include "Serialization.h"
struct FRenderState :public ISerialization
{
	//rasterizer


	//depth
	bool bEnableDepth = true;
	bool bDepthReadOnly = false;
	ECompareMethod mCompareMethod = ECompareMethod_LESS;

	//stencil
	bool bEnableStencil = false;
	bool bStencilReadOnly = true;
	EOperation mSFailOp = EOperation_KEEP;
	EOperation mSPassZFailOp = EOperation_KEEP;
	EOperation mPassOp = EOperation_KEEP;
	unsigned int mStencilRef = 1;

	//blend
	bool bEnableBlend = false;
	EBlendFunctionMode mBlendFuncMode = EBlendFunctionMode_ADD;
	EBlendFactor mBlendSrc = EBlendFactor_SRC_ALPHA;
	EBlendFactor mBlendDest = EBlendFactor_DST_ALPHA;

	//Cull
	bool bEnableCull = false;
	EFrontFaceDef mFrontDefinition = EFrontFaceDef_CCW;
	ECullMode mCullMode = ECullMode_BACK;

	virtual bool Parse(IN FJson&) override;
	virtual bool Serialize(OUT FJson&) override;
};

