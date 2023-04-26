#include "RenderState.h"
#include "Application.h"
bool FRenderState::Parse(IN FJson& In)
{
    FRenderCommand* Rhi = FApplication::GetRHI();

    FJson DepthTest = In["DepthTest"];
    bEnableDepth = DepthTest["bEnableDepth"].asBool();
    bDepthReadOnly = DepthTest["bDepthReadOnly"].asBool();
    mCompareMethod = Rhi->StringToCompareMethod(DepthTest["CompareMethod"].asCString());

    FJson StencilTest = In["StencilTest"];
    bEnableStencil = StencilTest["bEnableStencil"].asBool();
    bStencilReadOnly = StencilTest["bStencilReadOnly"].asBool();
    mSFailOp = Rhi->StringToOperation(StencilTest["SFailOp"].asCString());
    mSPassZFailOp = Rhi->StringToOperation(StencilTest["SPassZFailOp"].asCString());
    mPassOp = Rhi->StringToOperation(StencilTest["PassOp"].asCString());
    mStencilRef = Rhi->StringToOperation(StencilTest["StencilRef"].asCString());

    FJson BlendTest = In["BlendTest"];
    bEnableBlend = BlendTest["bEnableBlend"].asBool();
    mBlendFuncMode = Rhi->StringToBlendFunc(BlendTest["BlendFuncMode"].asCString());
    mBlendSrc = Rhi->StringToBlendFactor(BlendTest["BlendSrc"].asCString());
    mBlendDest = Rhi->StringToBlendFactor(BlendTest["BlendDest"].asCString());

    FJson Culling = In["Culling"];
    bEnableCull = Culling["bEnableCull"].asBool();
    mFrontDefinition = Rhi->StringToFrontFace(Culling["FrontDefinition"].asCString());
    mCullMode = Rhi->StringToCullMode(Culling["CullMode"].asCString());
    return true;
}

bool FRenderState::Serialize(OUT FJson& Out)
{
    FRenderCommand* Rhi = FApplication::GetRHI();

    FJson DepthTest = Out["DepthTest"];
    DepthTest["bEnableDepth"] = bEnableDepth;
    DepthTest["bDepthReadOnly"] = bDepthReadOnly;
    DepthTest["CompareMethod"] = Rhi->CompareMethodToString(mCompareMethod);

    FJson StencilTest = Out["StencilTest"];
    StencilTest["bEnableStencil"] = bEnableStencil;
    StencilTest["bStencilReadOnly"] = bStencilReadOnly;
    StencilTest["SFailOp"] = Rhi->OperationToString(mSFailOp);
    StencilTest["SPassZFailOp"] = Rhi->OperationToString(mSPassZFailOp);
    StencilTest["PassOp"] = Rhi->OperationToString(mPassOp);
    StencilTest["StencilRef"] = mStencilRef;

    FJson BlendTest = Out["BlendTest"];
    BlendTest["bEnableBlend"] = bEnableBlend;
    BlendTest["BlendFuncMode"] = Rhi->BlendFuncToString(mBlendFuncMode);
    BlendTest["BlendSrc"] = Rhi->BlendFactorToString(mBlendSrc);
    BlendTest["BlendDest"] = Rhi->BlendFactorToString(mBlendDest);

    FJson Culling = Out["Culling"];
    Culling["bEnableCull"] = bEnableCull;
    Culling["mFrontDefinition"] = Rhi->FrontFaceToString(mFrontDefinition);
    Culling["mCullMode"] = Rhi->FrontFaceToString(mCullMode);
    return true;
}
