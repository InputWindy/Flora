
#define FStructuredBuffer(T,Name,Slot,Size) \
layout(std430, binding = Slot) buffer Name\
{\
	T Name##PackedData[Size];\
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