#include "Uniform.h"
#include <glm/glm.hpp>
using namespace glm;
FSamplerInfo::FSamplerInfo(const FSamplerInfo& rhs)
	:Texture(rhs.Texture),Slot(rhs.Slot),Target(rhs.Target)
{
}

FSamplerInfo::FSamplerInfo(FSamplerInfo&& rhs)
	:Texture(rhs.Texture), Slot(rhs.Slot), Target(rhs.Target)
{
	rhs.SetTexture(nullptr);
}

void FSamplerInfo::operator=(const FSamplerInfo& rhs)
{
	Texture = rhs.Texture;
	Slot = rhs.Slot;
	Target = rhs.Target;
}

void FSamplerInfo::operator=(FSamplerInfo&& rhs)
{
	Texture = rhs.Texture;
	Slot = rhs.Slot;
	Target = rhs.Target;
	rhs.Texture = nullptr;
}

struct FUniformEnabler : public FUniform {};

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<bool>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(bool);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::BOOL;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<int>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(int);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::INT;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<ivec2>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(int);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::INT;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<ivec3>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(ivec3);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::INT3;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<ivec4>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(ivec4);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::INT4;
	return Res;
}

//////////////////////////////////////////////
template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<float>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(float);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::FLOAT;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<vec2>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(vec2);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::FLOAT2;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<vec3>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(vec3);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::FLOAT3;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<vec4>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(vec4);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::FLOAT4;
	return Res;
}

//////////////////////////////////////////////
template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<double>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(double);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::DOUBLE;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<dvec2>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(dvec2);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::DOUBLE2;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<dvec3>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(dvec3);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::DOUBLE3;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<dvec4>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(dvec4);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::DOUBLE4;
	return Res;
}

////////////////////////////////////
template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<mat2>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(mat2);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::MAT2;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<mat3>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(mat3);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::MAT3;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<mat4>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(mat4);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::MAT4;
	return Res;
}

////////////////////////////////////
template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<FMatArray<mat2>>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(FMatArray<mat2>);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::MAT2_ARRAY;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<FMatArray<mat3>>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(FMatArray<mat3>);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::MAT3_ARRAY;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<FMatArray<mat4>>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(FMatArray<mat4>);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::MAT4_ARRAY;
	return Res;
}

template<>
Ref<FUniform> FLORA_API FUniform::MakeShared<FSamplerInfo>(const char* Name, bool Display)
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = Name;
	Res->bDisplay = Display;
	Res->BufSize = sizeof(FSamplerInfo);
	Res->Buf = malloc(Res->BufSize);
	Res->Type = EUniformType::SAMPLER;
	return Res;
}

FUniform::~FUniform()
{
	if (Buf)
	{
		free(Buf);
		Buf = nullptr;
	}
}

Ref<FUniform> FUniform::Clone()
{
	Ref<FUniform> Res = make_shared<FUniformEnabler>();
	Res->UniformName = UniformName;
	Res->bDisplay = bDisplay;
	Res->BufSize = BufSize;
	Res->Buf = malloc(BufSize);
	Res->Type = Type;
	memcpy(Res->Buf,Buf, BufSize);
	return Res;
}

void FUniform::MoveTemp(Ref<FUniform>& InOut)
{
	InOut->UniformName = UniformName;
	InOut->bDisplay = bDisplay;
	InOut->BufSize = BufSize;
	InOut->Buf = malloc(BufSize);
	InOut->Type = Type;
	memcpy(InOut->Buf, Buf, BufSize);
	Buf = nullptr;
}

