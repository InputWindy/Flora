#include "Core/ShaderCore.glsl"
#include "ShaderDefines.h"
#include "ShaderStructs.h"
//
//// aligned 16
//struct CStaticMesh
//{
//	int pos_offset;// global offset
//	int tri_offset;// global offset
//	int pos_num;
//	int tri_num;
//	vec4 sphere_box;//center3 + radius1 (local space)
//};
//
//// aligned 16
//struct CSceneInstance
//{
//	int static_mesh_resource_id;
//	int shading_model_id;
//	int dummy0;// light cluster id
//	int dummy1;// light map id
//	mat4 model_matrix;
//};
//
//// aligned 16
//struct CCamera
//{
//	vec4 view_frustum[6];//view space
//	mat4 project_matrix;
//};
//
//// aligned 16
//struct CSceneCullingParam
//{
//	vec4 camera_pos;
//	mat4 view_matix;
//	CCamera Camera;
//};
//
/*
	static resource data . update only once .
*/
FByteAddressBuffer(Pos, POS_SLOT)   // read only
FByteAddressBuffer(Index, INDEX_SLOT)   // read only
FStructuredBuffer(CStaticMesh, CStaticMeshLists, STATIC_MESH_SLOT, MAX_STATIC_MESH_NUM)			// read only
FStructuredBuffer(CSceneInstance, CSceneInstanceLists, SCENE_INSTANCE_SLOT, MAX_INSTANCE_NUM)		// read only

int GetMeshId(int id)
{
	return GetCSceneInstanceLists(id).static_mesh_resource_id;
}

int GetShadingModel(int id)
{
	return GetCSceneInstanceLists(id).shading_model_id;
}

mat4 GetModelMatrix(int id)
{
	return GetCSceneInstanceLists(id).model_matrix;
}

/*
	dynamic buffer.modified every frame.
*/
FStructuredBuffer(CSceneCullingParam,CSceneCullingParamLists,CULL_PARAM_SLOT,1)				// by cpu

vec4 GetCameraPos()
{
	return GetCSceneCullingParamLists(0).camera_pos;
}

mat4 GetViewMatrix()
{
	return GetCSceneCullingParamLists(0).view_matix;
}

mat4 GetProjMatrix()
{
	return GetCSceneCullingParamLists(0).Camera.project_matrix;
}

void GetFrustum(inout vec4 frustum[6])
{
	CCamera Camera = GetCSceneCullingParamLists(0).Camera;
	frustum[0] = Camera.view_frustum[0];
	frustum[1] = Camera.view_frustum[1];
	frustum[2] = Camera.view_frustum[2];
	frustum[3] = Camera.view_frustum[3];
	frustum[4] = Camera.view_frustum[4];
	frustum[5] = Camera.view_frustum[5];
}

FStructuredBuffer(CVisibleInfo,VisibleInstance,VISIBLE_INSTANCE_SLOT,MAX_INSTANCE_NUM)						// intermediate buffer.write by instance culling .read in draw indirect pass.clear by finish rendering.

void MarkInvisibleInstance(int id)
{
	CVisibleInfo Info;
	Info.first = 0;
	Info.count = 0;
	SetVisibleInstance(id,Info);
}

void MarkVisibleInstance(int id,CVisibleInfo Info)
{	
	SetVisibleInstance(id,Info);
}

bool IsVisibleInstance(int id)
{
	return GetVisibleInstance(id).count > 0;
}

FByteAddressBuffer(DynamicIndexBuffer,DYNAMIC_INDEX_SLOT)										// dynamic ibo.used by indirect draw.write in gen indirect cmd pass.

void WriteDynamicIndex(int id,int data)
{
	StoreDynamicIndexBufferInt(id,data);
}

//struct FDrawElementsIndirectCommand 
//{
//	uint  index_count;
//	uint  instance_count;
//	uint  index_offset;
//	int   vertex_offset;
//	uint  base_instance_id;
//};
FStructuredBuffer(FDrawElementsIndirectCommand,IndirectDrawCommandBuffer,DRAW_COMMAND_SLOT,MAX_INSTANCE_NUM)	// indirect command buffer . write in gen indirec cmd pass
FStructuredBuffer(FDrawElementsIndirectCommand,IndirectDrawCommandBuffer1,DRAW_DYNAMIC_IBO_COMMAND_SLOT,1)	// indirect command buffer . write in gen indirec cmd pass

//// align 16
//struct CStat
//{
//	int StaticMeshNum;
//	int SceneInstanceNum;
//	int IndirectDrawNum;// modified every frame ( current draw call num )
//	int VertexCnt;// unused
//	int Dummy1;// unused
//	int Dummy2;// unused
//	int Dummy3;// unused
//	int Dummy4;// unused
//};
FStructuredBuffer(CStat,CStatList,STAT_SLOT,1)	

void AtomicAddVertexCntStat()
{
	atomicAdd(CStatListPackedData[0].VertexCnt,1);
}

void AtomicAddVisibleInstanceNum()
{
	atomicAdd(CStatListPackedData[0].VisibleInstanceNum,1);
}

int AtomicAddIndirectDrawNum(int n)
{
	return atomicAdd(CStatListPackedData[0].IndirectDrawNum,n);
}

void ResetVertexCntStat()
{
	CStatListPackedData[0].VertexCnt = 0;
}

int GetStaticMeshNum()
{
	return GetCStatList(0).StaticMeshNum;
}

int GetSceneInstanceNum()
{
	return GetCStatList(0).SceneInstanceNum;
}

int GetIndirectDrawNum()
{
	return GetCStatList(0).IndirectDrawNum;
}

void ResetIndirectDrawNum()
{
	CStatListPackedData[0].IndirectDrawNum = 0;
}

void ResetVertexCnt()
{
	CStatListPackedData[0].VertexCnt = 0;
}

void ResetStat()
{
	CStatListPackedData[0].IndirectDrawNum = 0;
	CStatListPackedData[0].VertexCnt = 0;
	CStatListPackedData[0].VisibleInstanceNum = 0;
}

void ResetIndirectDrawCmd(int id)
{
	FDrawElementsIndirectCommand Tmp;
	Tmp.index_count = uint(0);
	Tmp.instance_count = uint(0);
	Tmp.index_offset = uint(0);
	Tmp.vertex_offset = 0;
	Tmp.base_instance_id = uint(0);
	SetIndirectDrawCommandBuffer(id,Tmp);
	SetIndirectDrawCommandBuffer1(0,Tmp);
}


/*
	debug
*/
FStructuredBuffer(uint,Debug0,DEBUG_0_SLOT,MAX_DEBUG_NUM)
FStructuredBuffer(mat4,Debug1,DEBUG_1_SLOT,MAX_DEBUG_NUM)
//FStructuredBuffer(int,DebugInt,15,MAX_DEBUG_NUM)

