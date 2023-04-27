#include "Parser.h"
#include <sstream>
#include <iostream>
using namespace std;
FShaderParser::FShaderParser(const char* Code)
{
	string c = Code;
	Process(c);
}

void FShaderParser::Process(string& b)
{
	std::istringstream iss(b);
	std::vector<std::string> 
		tokens{ std::istream_iterator<std::string>{iss},std::istream_iterator<std::string>{} };

	size_t pos = 0;
	for (std::size_t i = 0; i < tokens.size(); i++)
	{
		int Case = -1;
		if ((pos = tokens[i].find("#include")) != std::string::npos)Case = 0;
		else if ((pos = tokens[i].find("uniform")) != std::string::npos)
			Case = 1;
		else Case = -1;
		ProcessToken(i, tokens, Case);
	}
}

void FShaderParser::ProcessToken(std::size_t& pos, std::vector<std::string>& v, int Case)
{
	switch (Case)
	{
	case 0:
	{
		break;
	}
	case 1: 
	{
		int token_pos = pos;
		while (v[token_pos].find(";") == std::string::npos and token_pos < v.size())++token_pos;

		assert(token_pos != v.size() - 1);

		string VarName, GlslType; bool bDisplay = true;

		VarName   = v[token_pos].substr(0, v[token_pos].size() - 1);
		GlslType  = v[token_pos - 1];
		bDisplay = v[pos - 1] != std::string("//[internal]");

		Ref<FUniform> Uniform;
		if (GlslType.find("bool") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<bool>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("int") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<int>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("ivec2") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<ivec2>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("ivec3") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<ivec3>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("ivec4") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<ivec4>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("float") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<float>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("vec2") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<vec2>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("vec3") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<vec3>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("vec4") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<vec4>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("double") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<float>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("dvec2") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<dvec2>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("dvec3") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<dvec3>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("dvec4") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<dvec4>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("mat2") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<mat2>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("mat3") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<mat3>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("mat4") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<mat4>(VarName.c_str(), bDisplay);
		}
		else if (GlslType.find("sampler1D") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<FSamplerInfo>(VarName.c_str(), bDisplay);
			FSamplerInfo* Sampler = Uniform->DynamicCast<FSamplerInfo>();
			Sampler->Target = ETextureTarget_1D;
		}
		else if (GlslType.find("sampler2D") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<FSamplerInfo>(VarName.c_str(), bDisplay);
			FSamplerInfo* Sampler = Uniform->DynamicCast<FSamplerInfo>();
			Sampler->Target = ETextureTarget_2D;
		}
		else if (GlslType.find("sampler3D") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<FSamplerInfo>(VarName.c_str(), bDisplay);
			FSamplerInfo* Sampler = Uniform->DynamicCast<FSamplerInfo>();
			Sampler->Target = ETextureTarget_3D;
		}
		else if (GlslType.find("samplerCube") != std::string::npos)
		{
			Uniform = FUniform::MakeShared<FSamplerInfo>(VarName.c_str(), bDisplay);
			FSamplerInfo* Sampler = Uniform->DynamicCast<FSamplerInfo>();
			Sampler->Target = ETextureTarget_Cubic;
		}
		else assert(0);

		if (Uniform)
		{
			Uniforms.push_back(Uniform);
		}

		break;
	}
	default:
		break;
	}
}
