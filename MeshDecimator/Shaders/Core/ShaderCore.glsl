#ifndef SHADER_CORE_H
#define SHADER_CORE_H

#include "Buffer/StructuredBuffer.glsl"
#include "Buffer/ByteAddressBuffer.glsl"

#define NUM_THREADS(x,y,z) layout(local_size_x = x, local_size_y = y, local_size_z = z) in;

#define LOCAL_THREAD_ID_1D int(gl_LocalInvocationID.z * gl_WorkGroupSize.x * gl_WorkGroupSize.y + gl_LocalInvocationID.y * gl_WorkGroupSize.x + gl_LocalInvocationID.x)

#define THREAD_GROUP_ID_1D int(gl_WorkGroupID.x + gl_WorkGroupID.y * gl_NumWorkGroups.x + gl_NumWorkGroups.x * gl_NumWorkGroups.y * gl_WorkGroupID.z)

#define GROUP_THREAD_NUM int(gl_NumWorkGroups.x*gl_NumWorkGroups.y*gl_NumWorkGroups.z)

//#define GLOBAL_THREAD_ID_1D int(gl_GlobalInvocationIndex)

#define GLOBAL_THREAD_ID_1D int((gl_WorkGroupID.x * gl_WorkGroupSize.x + gl_LocalInvocationID.x)+ (gl_WorkGroupID.y * gl_WorkGroupSize.y + gl_LocalInvocationID.y) * (gl_NumWorkGroups * gl_WorkGroupSize.x)+ (gl_WorkGroupID.z * gl_WorkGroupSize.z + gl_LocalInvocationID.z) * (gl_NumWorkGroups * gl_WorkGroupSize.x) * (gl_NumWorkGroups * gl_WorkGroupSize.y))


#endif
