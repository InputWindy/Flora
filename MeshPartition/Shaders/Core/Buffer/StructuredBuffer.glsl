#ifndef SHADER_STRUCTURED_BUFFER_H
#define SHADER_STRUCTURED_BUFFER_H

#define FStructuredBuffer(T,Name,Slot) \
layout(std430, binding = Slot) buffer Name\
{\
	T Name##PackedData[];\
};\
\
T Get##Name(int id)\
{\
	return Name##PackedData[id];\
}\
\
void Set##Name(int id,T data)\
{\
	Name##PackedData[id] = data;\
}

#endif
