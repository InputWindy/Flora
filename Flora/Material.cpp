#include "Material.h"
#include "ResourceManager.h"
#include "Parser.h"
FMaterial::FMaterial()
{
	Type = EResourceType::Material;
}
Ref<FMaterialInstance> FMaterial::GetInstance()
{
	Ref<FMaterialInstance> Res = make_shared<FMaterialInstance>();
	Res->Material = shared_from_this();
	for (size_t Idx = 0; Idx < Bools.size(); ++Idx)Res->Bools.push_back(Bools[Idx]);
	for (size_t Idx = 0; Idx < Vectors.size(); ++Idx)Res->Vectors.push_back(Vectors[Idx]);
	for (size_t Idx = 0; Idx < Matrices.size(); ++Idx)Res->Matrices.push_back(Matrices[Idx]);
	for (size_t Idx = 0; Idx < Samplers.size(); ++Idx)Res->Samplers.push_back(Samplers[Idx]);
	return Res;
}
void FMaterial::Register()
{
	FResourceManager& ResourceManager = FResourceManager::Get();
	if (ResourceManager.FindObject<FMaterial>(MaterialName))
	{
		ResourceManager.RemoveObject<FMaterial>(MaterialName);
	}
	ResourceManager.Register<FMaterial>(shared_from_this());
}

void FMaterial::GenerateUniforms()
{
	Bools.clear();
	Vectors.clear();
	Matrices.clear();
	Samplers.clear();

	auto ParseFunc = [this](const std::string& ShaderCode)
	{
		FShaderParser parser(ShaderCode.c_str());
		for (auto& Var : parser.Uniforms)
		{
			switch (Var->GetType())
			{
			case EUniformType::BOOL:if (Var->IsDisplay())Bools.push_back(Var); break;
			case EUniformType::INT: 
			case EUniformType::INT2:    
			case EUniformType::INT3:    
			case EUniformType::INT4:    
			case EUniformType::FLOAT:   
			case EUniformType::FLOAT2:  
			case EUniformType::FLOAT3:  
			case EUniformType::FLOAT4:  
			case EUniformType::DOUBLE:  
			case EUniformType::DOUBLE2: 
			case EUniformType::DOUBLE3: 
			case EUniformType::DOUBLE4:if (Var->IsDisplay())Vectors.push_back(Var); break;
			case EUniformType::MAT2: 
			case EUniformType::MAT3: 
			case EUniformType::MAT4: if (Var->IsDisplay())Matrices.push_back(Var); break;
			case EUniformType::SAMPLER:
			{
				if (Var->IsDisplay())Samplers.push_back(Var);
				FSamplerInfo* Sampler = Var->DynamicCast<FSamplerInfo>();
				Sampler->Slot = Samplers.size() - 1;
				break;
			}
			default:assert(0);break;
			}
		}
	};

	ParseFunc(VertShader);
	ParseFunc(FragShader);
}

bool FMaterial::Parse(IN FJson& In)
{
	MaterialName = In["MaterialName"].asString();
	VertShader = In["VertShader"].asString();
	FragShader = In["FragShader"].asString();
	RenderState.Parse(In["RenderState"]);

	CachePath = "/Cache/Material/" + MaterialName + ".fmaterial";

	Reload();
	return true;
}

bool FMaterial::Serialize(OUT FJson& Out)
{
	Out["MaterialName"] = MaterialName;
	Out["VertShader"] = VertShader;
	Out["FragShader"] = FragShader;
	RenderState.Serialize(Out["RenderState"]);
	return true;
}

bool FMaterialInstance::Parse(IN FJson& In)
{
	if (In["MaterialName"].isNull())
	{
		assert(0);
		return false;
	}
	Material = FResourceManager::Get().FindObject<FMaterial>(In["MaterialName"].asString());
	if (Material == nullptr)
	{
		assert(0);
		return false;
	}

	for (size_t Idx = 0; Idx < Bools.size(); ++Idx)Material->Bools.push_back(Bools[Idx]);
	for (size_t Idx = 0; Idx < Vectors.size(); ++Idx)Material->Vectors.push_back(Vectors[Idx]);
	for (size_t Idx = 0; Idx < Matrices.size(); ++Idx)Material->Matrices.push_back(Matrices[Idx]);
	for (size_t Idx = 0; Idx < Samplers.size(); ++Idx)Material->Samplers.push_back(Samplers[Idx]);
	
	FJson u_bools = In["Bools"];
	FJson u_vectors = In["Vectors"];
	FJson u_matrices = In["Matrices"];
	FJson u_samplers = In["Samplers"];

	assert(
		u_bools.size() == Bools.size()and
		u_vectors.size() == Vectors.size()and
		u_matrices.size() == Matrices.size()and
		u_samplers.size() == Samplers.size()
	);

	uint32_t Idx;
	Idx = 0;
	for (auto& u : u_bools)
	{
		Bools[Idx]->Parse(u);
		++Idx;
	}
	Idx = 0;
	for (auto& u : u_vectors)
	{
		Vectors[Idx]->Parse(u);
		++Idx;
	}
	Idx = 0;
	for (auto& u : u_matrices)
	{
		Matrices[Idx]->Parse(u);
		++Idx;
	}
	Idx = 0;
	for (auto& u : u_samplers)
	{
		Samplers[Idx]->Parse(u);
		++Idx;
	}
	return true;
}

bool FMaterialInstance::Serialize(OUT FJson& Out)
{
	Out["MaterialName"] = Material ? Material->GetName() : nullptr;
	FJson u_bools = Out["Bools"];
	FJson u_vectors = Out["Vectors"];
	FJson u_matrices = Out["Matrices"];
	FJson u_samplers = Out["Samplers"];
	for (size_t Idx = 0; Idx < Bools.size(); ++Idx)
	{
		FJson u_bool;
		Bools[Idx]->Serialize(u_bool);
		u_bools.append(u_bool);
	}
	for (size_t Idx = 0; Idx < Vectors.size(); ++Idx)
	{
		FJson u_v;
		Vectors[Idx]->Serialize(u_v);
		u_vectors.append(u_v);
	}
	for (size_t Idx = 0; Idx < Matrices.size(); ++Idx)
	{
		FJson u_mat;
		Matrices[Idx]->Serialize(u_mat);
		u_matrices.append(u_mat);
	}
	for (size_t Idx = 0; Idx < Samplers.size(); ++Idx)
	{
		FJson u_sampler;
		Samplers[Idx]->Serialize(u_sampler);
		u_samplers.append(u_sampler);
	}
	return true;
}
