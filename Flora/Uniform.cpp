#include "Uniform.h"
#include "RenderCommand.h"
#include "Application.h"
#include "ResourceManager.h"
#include <glm/glm.hpp>
using namespace glm;

const char* UniformTypeToString(EUniformType Type)
{
	switch (Type)
	{
	case EUniformType::BOOL:return	     "BOOL"      ;
	case EUniformType::INT:return	     "INT"       ;
	case EUniformType::INT2:return       "INT2"      ;
	case EUniformType::INT3:return       "INT3"      ;
	case EUniformType::INT4:return       "INT4"      ;
	case EUniformType::FLOAT:return      "FLOAT"     ;
	case EUniformType::FLOAT2: return    "FLOAT2"    ;
	case EUniformType::FLOAT3: return    "FLOAT3"    ;
	case EUniformType::FLOAT4: return    "FLOAT4"    ;
	case EUniformType::DOUBLE:return     "DOUBLE"    ;
	case EUniformType::DOUBLE2:	return   "DOUBLE2"   ;
	case EUniformType::DOUBLE3:	return   "DOUBLE3"   ;
	case EUniformType::DOUBLE4:	return   "DOUBLE4"   ;
	case EUniformType::MAT2:return       "MAT2"      ;
	case EUniformType::MAT3:return       "MAT3"      ;
	case EUniformType::MAT4:return       "MAT4"      ;
	case EUniformType::MAT2_ARRAY:return "MAT2_ARRAY";
	case EUniformType::MAT3_ARRAY:return "MAT3_ARRAY";
	case EUniformType::MAT4_ARRAY:return "MAT4_ARRAY";
	case EUniformType::SAMPLER:return    "SAMPLER"   ;
	case EUniformType::MAX:return		 "MAX"       ;
		break;
	default:
		break;
	}
	return "";
};
EUniformType StringToUniformType(const char* Str)
{
	string ss(Str);
	if		(ss == "BOOL"      )return EUniformType::BOOL;
	else if (ss == "INT"       )return EUniformType::INT;
	else if (ss == "INT2"      )return EUniformType::INT2;
	else if (ss == "INT3"      )return EUniformType::INT3;
	else if (ss == "INT4"      )return EUniformType::INT4;
	else if (ss == "FLOAT"     )return EUniformType::FLOAT;
	else if (ss == "FLOAT2"    )return EUniformType::FLOAT2;
	else if (ss == "FLOAT3"    )return EUniformType::FLOAT3;
	else if (ss == "FLOAT4"    )return EUniformType::FLOAT4;
	else if (ss == "DOUBLE"    )return EUniformType::DOUBLE;
	else if (ss == "DOUBLE2"   )return EUniformType::DOUBLE2;
	else if (ss == "DOUBLE3"   )return EUniformType::DOUBLE3;
	else if (ss == "DOUBLE4"   )return EUniformType::DOUBLE4;
	else if (ss == "MAT2"      )return EUniformType::MAT2;
	else if (ss == "MAT3"      )return EUniformType::MAT3;
	else if (ss == "MAT4"      )return EUniformType::MAT4;
	else if (ss == "MAT2_ARRAY")return EUniformType::MAT2_ARRAY;
	else if (ss == "MAT3_ARRAY")return EUniformType::MAT3_ARRAY;
	else if (ss == "MAT4_ARRAY")return EUniformType::MAT4_ARRAY;
	else if (ss == "SAMPLER"   )return EUniformType::SAMPLER;
	else if (ss == "MAX"       )return EUniformType::MAX;
	else assert(0);
	return EUniformType::MAX;
};

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

bool FUniform::Parse(IN FJson& In)
{
	In["UniformName"] = UniformName;
	In["UniformType"] = UniformTypeToString(Type);
	In["Display"]     = bDisplay;
	FJson Buffer = In["Buffer"];
	switch (Type)
	{
	case EUniformType::BOOL:Buffer["Data"] = *(bool*)Buf; break;
	case EUniformType::INT:Buffer["Data"] = *(int*)Buf; break;
	case EUniformType::INT2:
	{
		ivec2* u_iv = (ivec2*)Buf;
		FJson Data = Buffer["Data"];
		Data["x"] = u_iv->x;
		Data["y"] = u_iv->y;
		break;
	}
	case EUniformType::INT3:
	{
		ivec3* u_iv = (ivec3*)Buf;
		FJson Data = Buffer["Data"];
		Data["x"] = u_iv->x;
		Data["y"] = u_iv->y;
		Data["z"] = u_iv->z;
		break;
	}
	case EUniformType::INT4:
	{
		ivec4* u_iv = (ivec4*)Buf;
		FJson Data = Buffer["Data"];
		Data["x"] = u_iv->x;
		Data["y"] = u_iv->y;
		Data["z"] = u_iv->z;
		Data["w"] = u_iv->w;
		break;
	}
	case EUniformType::FLOAT:Buffer["Data"] = *(float*)Buf; break;
	case EUniformType::FLOAT2:
	{
		vec2* u_iv = (vec2*)Buf;
		FJson Data = Buffer["Data"];
		Data["x"] = u_iv->x;
		Data["y"] = u_iv->y;
		break;
	}
	case EUniformType::FLOAT3:
	{
		vec3* u_iv = (vec3*)Buf;
		FJson Data = Buffer["Data"];
		Data["x"] = u_iv->x;
		Data["y"] = u_iv->y;
		Data["z"] = u_iv->z;
		break;
	}
	case EUniformType::FLOAT4:
	{
		vec4* u_iv = (vec4*)Buf;
		FJson Data = Buffer["Data"];
		Data["x"] = u_iv->x;
		Data["y"] = u_iv->y;
		Data["z"] = u_iv->z;
		Data["w"] = u_iv->w;
		break;
	}
	case EUniformType::DOUBLE:Buffer["Data"] = *(double*)Buf; break;
	case EUniformType::DOUBLE2:
	{
		dvec2* u_iv = (dvec2*)Buf;
		FJson Data = Buffer["Data"];
		Data["x"] = u_iv->x;
		Data["y"] = u_iv->y;
		break;
	}
	case EUniformType::DOUBLE3:
	{
		dvec3* u_iv = (dvec3*)Buf;
		FJson Data = Buffer["Data"];
		Data["x"] = u_iv->x;
		Data["y"] = u_iv->y;
		Data["z"] = u_iv->z;
		break;
	}
	case EUniformType::DOUBLE4:
	{
		dvec4* u_iv = (dvec4*)Buf;
		FJson Data = Buffer["Data"];
		Data["x"] = u_iv->x;
		Data["y"] = u_iv->y;
		Data["z"] = u_iv->z;
		Data["w"] = u_iv->w;
		break;
	}
	case EUniformType::MAT2:break;
	case EUniformType::MAT3:break;
	case EUniformType::MAT4:break;
	case EUniformType::MAT2_ARRAY:break;
	case EUniformType::MAT3_ARRAY:break;
	case EUniformType::MAT4_ARRAY:break;
	case EUniformType::SAMPLER:
	{
		FSamplerInfo* u_sampler = (FSamplerInfo*)Buf;
		FJson Data = Buffer["Data"];
		Data["Slot"] = u_sampler->Slot;
		Data["TextureTarget"] = FApplication::GetRHI()->TextureTargetToString(u_sampler->Target);
		Data["Texture"] = u_sampler->GetTexture()->GetName();
		break;
	}
	case EUniformType::MAX:
		break;
	default:
		break;
	}
	return true;
}

bool FUniform::Serialize(OUT FJson& Out)
{
	UniformName = Out["UniformName"].asString();
	bDisplay = Out["Display"].asBool();
	Type = StringToUniformType(Out["UniformType"].asCString());
	FJson Buffer = Out["Buffer"];
	switch (Type)
	{
	case EUniformType::BOOL:*(bool*)Buf = Buffer["Data"].asBool(); break;
	case EUniformType::INT:*(int*)Buf = Buffer["Data"].asInt(); break;
	case EUniformType::INT2:
	{
		ivec2* u_iv = (ivec2*)Buf;
		FJson Data = Buffer["Data"];
		u_iv->x = Data["x"].asInt(); 
		u_iv->y = Data["y"].asInt();
		break;
	}
	case EUniformType::INT3:
	{
		ivec3* u_iv = (ivec3*)Buf;
		FJson Data = Buffer["Data"];
		u_iv->x = Data["x"].asInt();
		u_iv->y = Data["y"].asInt();
		u_iv->z = Data["z"].asInt();
		break;
	}
	case EUniformType::INT4:
	{
		ivec4* u_iv = (ivec4*)Buf;
		FJson Data = Buffer["Data"];
		u_iv->x = Data["x"].asInt();
		u_iv->y = Data["y"].asInt();
		u_iv->z = Data["z"].asInt();
		u_iv->w = Data["w"].asInt();
		break;
	}
	case EUniformType::FLOAT:Buffer["Data"] = *(float*)Buf; break;
	case EUniformType::FLOAT2:
	{
		vec2* u_iv = (vec2*)Buf;
		FJson Data = Buffer["Data"];
		u_iv->x = Data["x"].asFloat();
		u_iv->y = Data["y"].asFloat();
		break;
	}
	case EUniformType::FLOAT3:
	{
		vec3* u_iv = (vec3*)Buf;
		FJson Data = Buffer["Data"];
		u_iv->x = Data["x"].asFloat();
		u_iv->y = Data["y"].asFloat();
		u_iv->z = Data["z"].asFloat();
		break;
	}
	case EUniformType::FLOAT4:
	{
		vec4* u_iv = (vec4*)Buf;
		FJson Data = Buffer["Data"];
		u_iv->x = Data["x"].asFloat();
		u_iv->y = Data["y"].asFloat();
		u_iv->z = Data["z"].asFloat();
		u_iv->w = Data["w"].asFloat();
		break;
	}
	case EUniformType::DOUBLE:Buffer["Data"] = *(double*)Buf; break;
	case EUniformType::DOUBLE2:
	{
		dvec2* u_iv = (dvec2*)Buf;
		FJson Data = Buffer["Data"];
		u_iv->x = Data["x"].asDouble();
		u_iv->y = Data["y"].asDouble();
		break;
	}
	case EUniformType::DOUBLE3:
	{
		dvec3* u_iv = (dvec3*)Buf;
		FJson Data = Buffer["Data"];
		u_iv->x = Data["x"].asDouble();
		u_iv->y = Data["y"].asDouble();
		u_iv->z = Data["z"].asDouble();
		break;
	}
	case EUniformType::DOUBLE4:
	{
		dvec4* u_iv = (dvec4*)Buf;
		FJson Data = Buffer["Data"];
		u_iv->x = Data["x"].asDouble();
		u_iv->y = Data["y"].asDouble();
		u_iv->z = Data["z"].asDouble();
		u_iv->w = Data["w"].asDouble();
		break;
	}
	case EUniformType::MAT2:break;
	case EUniformType::MAT3:break;
	case EUniformType::MAT4:break;
	case EUniformType::MAT2_ARRAY:break;
	case EUniformType::MAT3_ARRAY:break;
	case EUniformType::MAT4_ARRAY:break;
	case EUniformType::SAMPLER:
	{
		FSamplerInfo* u_sampler = (FSamplerInfo*)Buf;
		FJson Data = Buffer["Data"];
		u_sampler->Slot = Data["Slot"].asUInt();
		u_sampler->Target = FApplication::GetRHI()->StringToTextureTarget(Data["TextureTarget"].asCString());
		u_sampler->SetTexture(FResourceManager::Get().FindObject<FTexture>(Data["Texture"].asString()));
		if (u_sampler->GetTexture() == nullptr)u_sampler->SetTexture(FTexture::GetWhite());
		break;
	}
	case EUniformType::MAX:
		break;
	default:
		break;
	}
	return true;
}

