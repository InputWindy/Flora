#ifndef SHADER_BYTE_ADDR_BUFFER_H
#define SHADER_BYTE_ADDR_BUFFER_H

#define FByteAddressBuffer(Name,Slot) \
layout(std430, binding = Slot) buffer Name\
{\
	uint Name##PackedData[];\
};\
\
vec4 Load##Name##Vec4(int id)\
{\
	uint PackedData0 = Name##PackedData[id + 0];\
	uint PackedData1 = Name##PackedData[id + 1];\
	uint PackedData2 = Name##PackedData[id + 2];\
	uint PackedData3 = Name##PackedData[id + 3];\
	\
	vec4 UnpackedData;\
	UnpackedData.x = uintBitsToFloat(PackedData0);\
	UnpackedData.y = uintBitsToFloat(PackedData1);\
	UnpackedData.z = uintBitsToFloat(PackedData2);\
	UnpackedData.w = uintBitsToFloat(PackedData3);\
	\
	return UnpackedData;\
}\
vec3 Load##Name##Vec3(int id)\
{\
	uint PackedData0 = Name##PackedData[id + 0];\
	uint PackedData1 = Name##PackedData[id + 1];\
	uint PackedData2 = Name##PackedData[id + 2];\
	\
	vec3 UnpackedData;\
	UnpackedData.x = uintBitsToFloat(PackedData0);\
	UnpackedData.y = uintBitsToFloat(PackedData1);\
	UnpackedData.z = uintBitsToFloat(PackedData2);\
	\
	return UnpackedData;\
}\
vec2 Load##Name##Vec2(int id)\
{\
	uint PackedData0 = Name##PackedData[id + 0];\
	uint PackedData1 = Name##PackedData[id + 1];\
	\
	vec2 UnpackedData;\
	UnpackedData.x = uintBitsToFloat(PackedData0);\
	UnpackedData.y = uintBitsToFloat(PackedData1);\
	\
	return UnpackedData;\
}\
float Load##Name##Float(int id)\
{\
	uint PackedData0 = Name##PackedData[id + 0];\
	\
	float UnpackedData;\
	UnpackedData = uintBitsToFloat(PackedData0);\
	\
	return UnpackedData;\
}\
\
uint Load##Name##UInt(int id)\
{\
	return Name##PackedData[id];\
}\
\
uvec2 Load##Name##UInt2(int id)\
{\
	uint PackedData0 = Name##PackedData[id + 0];\
	uint PackedData1 = Name##PackedData[id + 1];\
	return uvec2(PackedData0,PackedData1);\
}\
uvec3 Load##Name##UInt3(int id)\
{\
	uint PackedData0 = Name##PackedData[id + 0];\
	uint PackedData1 = Name##PackedData[id + 1];\
	uint PackedData2 = Name##PackedData[id + 2];\
	return uvec3(PackedData0,PackedData1,PackedData2);\
}\
uvec4 Load##Name##UInt4(int id)\
{\
	uint PackedData0 = Name##PackedData[id + 0];\
	uint PackedData1 = Name##PackedData[id + 1];\
	uint PackedData2 = Name##PackedData[id + 2];\
	uint PackedData3 = Name##PackedData[id + 3];\
	return uvec4(PackedData0,PackedData1,PackedData2,PackedData3);\
}\
\
int Load##Name##Int(int id)\
{\
	return int(Name##PackedData[id]);\
}\
\
ivec2 Load##Name##Int2(int id)\
{\
	int PackedData0 = int(Name##PackedData[id + 0]);\
	int PackedData1 = int(Name##PackedData[id + 1]);\
	return ivec2(PackedData0,PackedData1);\
}\
ivec3 Load##Name##Int3(int id)\
{\
	int PackedData0 = int(Name##PackedData[id + 0]);\
	int PackedData1 = int(Name##PackedData[id + 1]);\
	int PackedData2 = int(Name##PackedData[id + 2]);\
	return ivec3(PackedData0,PackedData1,PackedData2);\
}\
ivec4 Load##Name##Int4(int id)\
{\
	int PackedData0 = int(Name##PackedData[id + 0]);\
	int PackedData1 = int(Name##PackedData[id + 1]);\
	int PackedData2 = int(Name##PackedData[id + 2]);\
	int PackedData3 = int(Name##PackedData[id + 3]);\
	return ivec4(PackedData0,PackedData1,PackedData2,PackedData3);\
}\
\
void Store##Name##Float(int id,float data)\
{\
	uint PackedData = floatBitsToUint(data);\
	Name##PackedData[id + 0] = PackedData;\
}\
\
void Store##Name##Vec2(int id,vec2 data)\
{\
	uint PackedData0 = floatBitsToUint(data.x);\
	uint PackedData1 = floatBitsToUint(data.y);\
	Name##PackedData[id + 0] = PackedData0;\
	Name##PackedData[id + 1] = PackedData1;\
}\
\
void Store##Name##Vec3(int id,vec3 data)\
{\
	uint PackedData0 = floatBitsToUint(data.x);\
	uint PackedData1 = floatBitsToUint(data.y);\
	uint PackedData2 = floatBitsToUint(data.z);\
	Name##PackedData[id + 0] = PackedData0;\
	Name##PackedData[id + 1] = PackedData1;\
	Name##PackedData[id + 2] = PackedData2;\
}\
\
void Store##Name##Vec4(int id,vec4 data)\
{\
	uint PackedData0 = floatBitsToUint(data.x);\
	uint PackedData1 = floatBitsToUint(data.y);\
	uint PackedData2 = floatBitsToUint(data.z);\
	uint PackedData3 = floatBitsToUint(data.w);\
	Name##PackedData[id + 0] = PackedData0;\
	Name##PackedData[id + 1] = PackedData1;\
	Name##PackedData[id + 2] = PackedData2;\
	Name##PackedData[id + 3] = PackedData3;\
}\
\
void Store##Name##UInt(int id,uint data)\
{\
	Name##PackedData[id + 0] = data.x;\
}\
\
void Store##Name##UInt2(int id,uvec2 data)\
{\
	Name##PackedData[id + 0] = data.x;\
	Name##PackedData[id + 1] = data.y;\
}\
\
void Store##Name##UInt3(int id,uvec3 data)\
{\
	Name##PackedData[id + 0] = data.x;\
	Name##PackedData[id + 1] = data.y;\
	Name##PackedData[id + 2] = data.z;\
}\
\
void Store##Name##UInt4(int id,uvec4 data)\
{\
	Name##PackedData[id + 0] = data.x;\
	Name##PackedData[id + 1] = data.y;\
	Name##PackedData[id + 2] = data.z;\
	Name##PackedData[id + 3] = data.w;\
}\
\
void Store##Name##Int(int id,int data)\
{\
	Name##PackedData[id + 0] = uint(data.x);\
}\
\
void Store##Name##Int2(int id,ivec2 data)\
{\
	Name##PackedData[id + 0] = uint(data.x);\
	Name##PackedData[id + 1] = uint(data.y);\
}\
\
void Store##Name##Int3(int id,ivec3 data)\
{\
	Name##PackedData[id + 0] = uint(data.x);\
	Name##PackedData[id + 1] = uint(data.y);\
	Name##PackedData[id + 2] = uint(data.z);\
}\
\
void Store##Name##Int4(int id,ivec4 data)\
{\
	Name##PackedData[id + 0] = uint(data.x);\
	Name##PackedData[id + 1] = uint(data.y);\
	Name##PackedData[id + 2] = uint(data.z);\
	Name##PackedData[id + 3] = uint(data.w);\
}

#endif
