#ifndef SHADER_GPU_SCENE_H
#define SHADER_GPU_SCENE_H

#include "Core/ShaderCore.glsl"
#include "ShaderDefines.h"
#include "ShaderStructs.h"

FStructuredBuffer(FVertex,VertexBuffer ,VERTEX_BUFFER_SLOT)
FStructuredBuffer(ivec3,TriangleBuffer,TRIANGLE_BUFFER_SLOT)
FStructuredBuffer(FMesh,MeshBuffer ,MESH_BUFFER_SLOT)
FStructuredBuffer(FScene,SceneBuffer ,SCENE_BUFFER_SLOT)

FStructuredBuffer(int,TriangleCollectionBuffer,TRIANGLE_COLLECTION_BUFFER_SLOT)
FStructuredBuffer(FResults,TriangleCollectionProfilingResultsBuffer,TRIANGLE_COLLECTION_PROFILING_SLOT)
FStructuredBuffer(vec4,DebugTriangleCollectionBuffer,DEBUG_SLOT)

void SetTriangleCollection(int TriId,int Collection)
{
	TriangleCollectionBufferPackedData[TriId] = Collection;
}

void SetDebugTriangleCollection(int TriId,vec4 Data)
{
	DebugTriangleCollectionBufferPackedData[TriId] = Data;
}

#endif
