#include "RenderState.h"
#include "Application.h"
bool FRenderState::Parse(IN FJson& In)
{
    FRenderCommand* Rhi = FApplication::GetRHI();

    FJson& DepthTest = In["DepthTest"];
    bEnableDepth = DepthTest["EnableDepth"].asBool();
    bDepthReadOnly = DepthTest["DepthReadOnly"].asBool();
    mCompareMethod = Rhi->StringToCompareMethod(DepthTest["CompareMethod"].asCString());

    FJson& StencilTest = In["StencilTest"];
    bEnableStencil = StencilTest["EnableStencil"].asBool();
    bStencilReadOnly = StencilTest["StencilReadOnly"].asBool();
    mSFailOp = Rhi->StringToOperation(StencilTest["SFailOp"].asCString());
    mSPassZFailOp = Rhi->StringToOperation(StencilTest["SPassZFailOp"].asCString());
    mPassOp = Rhi->StringToOperation(StencilTest["PassOp"].asCString());
    mStencilRef = StencilTest["StencilRef"].asInt();

    FJson& BlendTest = In["BlendTest"];
    bEnableBlend = BlendTest["EnableBlend"].asBool();
    mBlendFuncMode = Rhi->StringToBlendFunc(BlendTest["BlendFuncMode"].asCString());
    mBlendSrc = Rhi->StringToBlendFactor(BlendTest["BlendSrc"].asCString());
    mBlendDest = Rhi->StringToBlendFactor(BlendTest["BlendDest"].asCString());

    FJson& Culling = In["Culling"];
    bEnableCull = Culling["EnableCull"].asBool();
    mFrontDefinition = Rhi->StringToFrontFace(Culling["FrontDefinition"].asCString());
    mCullMode = Rhi->StringToCullMode(Culling["CullMode"].asCString());
    return true;
}

bool FRenderState::Serialize(OUT FJson& Out)
{
    FRenderCommand* Rhi = FApplication::GetRHI();

    FJson& DepthTest = Out["DepthTest"];
    DepthTest["EnableDepth"] = bEnableDepth;
    DepthTest["DepthReadOnly"] = bDepthReadOnly;
    DepthTest["CompareMethod"] = Rhi->CompareMethodToString(mCompareMethod);

    FJson& StencilTest = Out["StencilTest"];
    StencilTest["EnableStencil"] = bEnableStencil;
    StencilTest["StencilReadOnly"] = bStencilReadOnly;
    StencilTest["SFailOp"] = Rhi->OperationToString(mSFailOp);
    StencilTest["SPassZFailOp"] = Rhi->OperationToString(mSPassZFailOp);
    StencilTest["PassOp"] = Rhi->OperationToString(mPassOp);
    StencilTest["StencilRef"] = mStencilRef;

    FJson& BlendTest = Out["BlendTest"];
    BlendTest["EnableBlend"] = bEnableBlend;
    BlendTest["BlendFuncMode"] = Rhi->BlendFuncToString(mBlendFuncMode);
    BlendTest["BlendSrc"] = Rhi->BlendFactorToString(mBlendSrc);
    BlendTest["BlendDest"] = Rhi->BlendFactorToString(mBlendDest);

    FJson& Culling = Out["Culling"];
    Culling["EnableCull"] = bEnableCull;
    Culling["FrontDefinition"] = Rhi->FrontFaceToString(mFrontDefinition);
    Culling["CullMode"] = Rhi->CullModeToString(mCullMode);
    return true;
}
