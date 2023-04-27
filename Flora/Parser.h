#pragma once
#include "Core.h"
#include "Type.h"
#include "Uniform.h"
#include <vector>
#include <string>
#include <unordered_map>
class FLORA_API FShaderParser
{
public:
	FShaderParser(const char* Code);
	~FShaderParser() = default;

	std::vector<Ref<FUniform>>	Uniforms;
private:
	void Process(std::string& b);
	void ProcessToken(std::size_t& pos, std::vector<std::string>& v,int Case);
private:
	static inline std::unordered_map<std::string, FShaderParser>	HeaderFiles;//glsl include
};

