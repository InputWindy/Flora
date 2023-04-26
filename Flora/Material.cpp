#include "Material.h"
#include "ResourceManager.h"
void FMaterial::Register()
{
	FResourceManager& ResourceManager = FResourceManager::Get();
	if (!ResourceManager.FindObject<FMaterial>(MaterialName))
	{
		ResourceManager.Register<FMaterial>(shared_from_this());
	};
}

void FMaterial::GenerateUniforms()
{

}

bool FMaterial::Parse(IN FJson& In)
{
	MaterialName = In["MaterialName"].asString();
	VertShader = In["VertShader"].asString();
	FragShader = In["FragShader"].asString();
	FJson render_state = In["RenderState"];
	RenderState.Parse(render_state);
	Reload();
	return true;
}

bool FMaterial::Serialize(OUT FJson& Out)
{
	Out["MaterialName"] = MaterialName;
	Out["VertShader"] = VertShader;
	Out["FragShader"] = FragShader;
	FJson render_state = Out["RenderState"];
	RenderState.Serialize(render_state);
	return true;
}
