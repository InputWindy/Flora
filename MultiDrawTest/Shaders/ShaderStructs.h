
// aligned 16
struct CStaticMesh
{
	int pos_offset;// global offset
	int tri_offset;// global offset
	int pos_num;
	int tri_num;
	vec4 sphere_box;//center3 + radius1 (local space)
};

// aligned 16
struct CSceneInstance
{
	int static_mesh_resource_id;
	int shading_model_id;
	int dummy0;// light cluster id
	int dummy1;// light map id
	mat4 model_matrix;
};

// aligned 16
struct CCamera
{
	vec4 view_frustum[6];//view space
	mat4 project_matrix;
};

// aligned 16
struct CSceneCullingParam
{
	vec4 camera_pos;
	mat4 view_matix;
	CCamera Camera;
};

// aligned 4
struct CVisibleInfo
{
	int first;
	int count;
};

struct FDrawElementsIndirectCommand
{
	uint  index_count;
	uint  instance_count;
	uint  index_offset;
	int   vertex_offset;
	uint  base_instance_id;
};

// align 16
struct CStat
{
	int StaticMeshNum;
	int SceneInstanceNum;
	int IndirectDrawNum;// modified every frame ( current draw call num )
	int VertexCnt;// 
	int VisibleInstanceNum;// unused
	int Dummy2;// unused
	int Dummy3;// unused
	int Dummy4;// unused
};