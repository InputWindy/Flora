#pragma once
#include "RenderState.h"
#include "Uniform.h"
#include "Resource.h"
#include "Serialization.h"
class FMaterial;
class FLORA_API FMaterialInstance:public std::enable_shared_from_this<FMaterialInstance>, public ISerialization
{
	friend class FMaterial;
public:
	FMaterialInstance() = default;
	virtual ~FMaterialInstance() = default;
public:
	inline std::vector<Ref<FUniform>>& GetBools()    { return Bools; };
	inline std::vector<Ref<FUniform>>& GetVectors()  { return Vectors; };
	inline std::vector<Ref<FUniform>>& GetMatrices() { return Matrices; };
	inline std::vector<Ref<FUniform>>& GetSamplers() { return Samplers; };
	inline Ref<FMaterial> GetMaterial()				 { return Material; };

	virtual bool Parse(IN FJson&)final;
	virtual bool Serialize(OUT FJson&)final;
protected:
	Ref<FMaterial> Material = nullptr;
	std::vector<Ref<FUniform>> Bools;
	std::vector<Ref<FUniform>> Vectors;
	std::vector<Ref<FUniform>> Matrices;
	std::vector<Ref<FUniform>> Samplers;
};

class FLORA_API FMaterial :public std::enable_shared_from_this<FMaterial>, public IResource,public ISerialization
{
	friend class FOpenGLInterface;
	friend class FMaterialInstance;
protected:
	FMaterial();
public:
	virtual ~FMaterial() = default;

	inline std::string   GetName()const		 { return MaterialName; };
	inline std::string   GetCachePath()const { return CachePath; };
	inline FRenderState& GetRenderState()    { return RenderState; };
	inline std::string&  GetVertexShader()   { return VertShader; };
	inline std::string&  GetFragShader()     { return FragShader; };

	static inline auto   GetSkyBoxMaterial() { return SkyBoxShader; };
	static inline auto   GetDrawDebugCubeMaterial() { return DrawDebugCubeShader; };
	static inline auto   GetStandardOpcacityMaterial() { return StandardOpacityShader; };
	static inline auto   GetStandardPostprocessMaterial() { return StandardPostprocessShader; };
	static inline auto   GetEquirectangularMapToCubemapMaterial() { return EquirectangularMapToCubemapShader; };
	
	Ref<FMaterialInstance> GetInstance();
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
protected:
	static inline Ref<FMaterial> EquirectangularMapToCubemapShader = nullptr;
	static inline Ref<FMaterial> SkyBoxShader = nullptr;
	static inline Ref<FMaterial> StandardOpacityShader = nullptr;
	static inline Ref<FMaterial> StandardPostprocessShader = nullptr;
	static inline Ref<FMaterial> DrawDebugCubeShader = nullptr;
};

