#pragma once
#include "Core.h"
#include "Texture.h"
#include <vector>
#include <array>
#include <assert.h>

#include <glm/glm.hpp>
using namespace glm;

enum class EUniformType :unsigned int
{
	BOOL,
	INT, INT2, INT3, INT4,
	FLOAT, FLOAT2, FLOAT3, FLOAT4,
	DOUBLE, DOUBLE2, DOUBLE3, DOUBLE4,
	MAT2, MAT3, MAT4,
	MAT2_ARRAY, MAT3_ARRAY, MAT4_ARRAY,

	SAMPLER,
	MAX
};

//constructed by tiny parser
class FLORA_API FSamplerInfo
{
	friend class FMaterial;
	friend class FUniform;
	friend class FShaderParser;
public:
	FSamplerInfo() = default;
	FSamplerInfo(uint8_t slot, ETextureTarget target, Ref<FTexture> texture = nullptr):
		Slot(slot),Target(target),Texture(texture){};
	~FSamplerInfo() = default;

	FSamplerInfo(const FSamplerInfo&);
	FSamplerInfo(FSamplerInfo&&);

	void operator=(const FSamplerInfo&);
	void operator=(FSamplerInfo&&);

	inline void SetTexture(const Ref<FTexture> Tex) { Texture = Tex; };
	inline Ref<FTexture> GetTexture()const { return Texture; };
private:
	Ref<FTexture>   Texture = nullptr;
	ETextureTarget  Target  = ETextureTarget_2D;
	uint8_t			Slot    = 0;
};

//Matrix Array Size Must Be Fixed
template<typename TMat>
class FLORA_API FMatArray
{
public:
	FMatArray() = default;
	~FMatArray() = default;

	FMatArray(const FMatArray& rhs) :Data(rhs.Data) {};
	FMatArray(FMatArray&& rhs) :Data(std::move(rhs.Data)) {};

	inline void operator=(const FMatArray& rhs) { Data = rhs.Data; };
	inline void operator=(FMatArray&& rhs) { Data = rhs.Data; };

	inline TMat& operator[](uint32_t idx) { return Data[idx]; };
private:
	array<TMat,50> Data;
};

class FLORA_API FUniform :public std::enable_shared_from_this<FUniform>,public ISerialization
{
protected:
	FUniform() = default;
public:
	FUniform(const FUniform&) = delete;
	FUniform(FUniform&&) = delete;

	void operator=(const FUniform&) = delete;
	void operator=(FUniform&&) = delete;

	virtual~FUniform();

	template<typename T>
	requires 
	std::is_same_v<bool, T> or
		std::is_same_v<int, T> or std::is_same_v<ivec2, T> or std::is_same_v<ivec3, T> or std::is_same_v<ivec4, T> or
		std::is_same_v<float, T> or std::is_same_v<vec2, T> or std::is_same_v<vec3, T> or std::is_same_v<vec4, T> or
		std::is_same_v<double, T> or std::is_same_v<dvec2, T> or std::is_same_v<dvec3, T> or std::is_same_v<dvec4, T> or
		std::is_same_v<mat2, T> or std::is_same_v<mat3, T> or std::is_same_v<mat4, T> or
		std::is_same_v<FMatArray<mat2>, T> or std::is_same_v<FMatArray<mat3>, T> or std::is_same_v<FMatArray<mat4>, T> or
		std::is_same_v<FSamplerInfo, T>
	static Ref<FUniform> MakeShared(const char* Name, bool Display = true);

	template<typename T>
		requires
		std::is_same_v<bool, T> or
		std::is_same_v<int, T> or std::is_same_v<ivec2, T> or std::is_same_v<ivec3, T> or std::is_same_v<ivec4, T> or
		std::is_same_v<float, T> or std::is_same_v<vec2, T> or std::is_same_v<vec3, T> or std::is_same_v<vec4, T> or
		std::is_same_v<double, T> or std::is_same_v<dvec2, T> or std::is_same_v<dvec3, T> or std::is_same_v<dvec4, T> or
		std::is_same_v<mat2, T> or std::is_same_v<mat3, T> or std::is_same_v<mat4, T> or
		std::is_same_v<FMatArray<mat2>, T> or std::is_same_v<FMatArray<mat3>, T> or std::is_same_v<FMatArray<mat4>, T> or
		std::is_same_v<FSamplerInfo, T>
	inline T * StaticCast() { return (T*)Buf; };

	template<typename T>
		requires
		std::is_same_v<bool, T> or
		std::is_same_v<int, T> or std::is_same_v<ivec2, T> or std::is_same_v<ivec3, T> or std::is_same_v<ivec4, T> or
		std::is_same_v<float, T> or std::is_same_v<vec2, T> or std::is_same_v<vec3, T> or std::is_same_v<vec4, T> or
		std::is_same_v<double, T> or std::is_same_v<dvec2, T> or std::is_same_v<dvec3, T> or std::is_same_v<dvec4, T> or
		std::is_same_v<mat2, T> or std::is_same_v<mat3, T> or std::is_same_v<mat4, T> or
		std::is_same_v<FMatArray<mat2>, T> or std::is_same_v<FMatArray<mat3>, T> or std::is_same_v<FMatArray<mat4>, T> or
		std::is_same_v<FSamplerInfo, T>
	inline T* DynamicCast() 
	{
		static T t;

		T* Res = nullptr;

		switch (Type)
		{
		case EUniformType::BOOL:      if (std::is_same_v<bool, T>)Res = (T*)Buf; break;
		case EUniformType::INT:       if (std::is_same_v<int, T>)Res = (T*)Buf; break;
		case EUniformType::INT2:      if (std::is_same_v<ivec2, T>)Res = (T*)Buf; break;
		case EUniformType::INT3:      if (std::is_same_v<ivec3, T>)Res = (T*)Buf; break;
		case EUniformType::INT4:      if (std::is_same_v<ivec4, T>)Res = (T*)Buf; break;
		case EUniformType::FLOAT:     if (std::is_same_v<float, T>)Res = (T*)Buf; break;
		case EUniformType::FLOAT2:    if (std::is_same_v<vec2, T>)Res = (T*)Buf; break;
		case EUniformType::FLOAT3:    if (std::is_same_v<vec3, T>)Res = (T*)Buf; break;
		case EUniformType::FLOAT4:    if (std::is_same_v<vec4, T>)Res = (T*)Buf; break;
		case EUniformType::DOUBLE:    if (std::is_same_v<double, T>)Res = (T*)Buf; break;
		case EUniformType::DOUBLE2:   if (std::is_same_v<dvec2, T>)Res = (T*)Buf; break;
		case EUniformType::DOUBLE3:   if (std::is_same_v<dvec3, T>)Res = (T*)Buf; break;
		case EUniformType::DOUBLE4:   if (std::is_same_v<dvec4, T>)Res = (T*)Buf; break;
		case EUniformType::MAT2:	  if (std::is_same_v<mat2, T>)Res = (T*)Buf; break;
		case EUniformType::MAT3:	  if (std::is_same_v<mat3, T>)Res = (T*)Buf; break;
		case EUniformType::MAT4:	  if (std::is_same_v<mat4, T>)Res = (T*)Buf; break;
		case EUniformType::MAT2_ARRAY:if (std::is_same_v<FMatArray<mat2>, T>)Res = (T*)Buf; break;
		case EUniformType::MAT3_ARRAY:if (std::is_same_v<FMatArray<mat3>, T>)Res = (T*)Buf; break;
		case EUniformType::MAT4_ARRAY:if (std::is_same_v<FMatArray<mat4>, T>)Res = (T*)Buf; break;
		case EUniformType::SAMPLER:   
			if (std::is_same_v<FSamplerInfo, T>)Res = (T*)Buf; break;
		case EUniformType::MAX:assert(0, "EUniformType::MAX");
			break;
		default:
			break;
		}
		return Res;
	};

	//deep copy
	Ref<FUniform> Clone();

	//move copy
	void MoveTemp(Ref<FUniform>& InOut);
public:
	virtual bool Parse(IN FJson&)final;
	virtual bool Serialize(OUT FJson&)final;
public:
	inline string		GetName()const   { return UniformName; };
	inline EUniformType GetType()const   { return Type; };
	inline bool			IsDisplay()const { return bDisplay; };
protected:
	string   UniformName;
	EUniformType Type = EUniformType::MAX;
protected:
	bool     bDisplay = true;
	void*	 Buf	  = nullptr;
	uint32_t BufSize  = 0;
};