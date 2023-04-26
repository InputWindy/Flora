#pragma once
#include "RenderState.h"
#include "Uniform.h"
#include "Resource.h"
#include "Serialization.h"
class FLORA_API FMaterial :public std::enable_shared_from_this<FMaterial>, public IResource,public ISerialization
{
protected:
	FMaterial() = default;
public:
	virtual ~FMaterial() = default;

	inline std::string   GetName()const		 { return MaterialName; };
	inline std::string   GetCachePath()const { return CachePath; };
	inline FRenderState& GetRenderState()    { return RenderState; };
	inline std::string&  GetVertexShader()   { return VertShader; };
	inline std::string&  GetFragShader()     { return FragShader; };
public:
	virtual uint32_t GetHandle()const = 0;
public:
	virtual void Register()final;
	virtual bool Parse(IN FJson&)final;
	virtual bool Serialize(OUT FJson&)final;
protected:
	void GenerateUniforms();
	virtual void Reload() = 0;
protected:
	std::string MaterialName;

	std::string CachePath;

	FRenderState RenderState;

	std::string VertShader = "";
	std::string FragShader = "";
protected:
	std::vector<std::shared_ptr<FUniform>> Bools;
	std::vector<std::shared_ptr<FUniform>> Vectors;
	std::vector<std::shared_ptr<FUniform>> Matrices;
	std::vector<std::shared_ptr<FUniform>> Samplers;
};

