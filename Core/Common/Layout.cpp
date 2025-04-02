#include <Common/Layout.h>
#include <glad/glad.h>
namespace flora
{
	uint32_t GetBufferElementComponentNum(flora::BufferElementDataType type)
	{
		switch (type)
		{

		case flora::BufferElementDataType::Bool1: return 1;
		case flora::BufferElementDataType::Bool2: return 2;
		case flora::BufferElementDataType::Bool3: return 3;
		case flora::BufferElementDataType::Bool4: return 4;
		case flora::BufferElementDataType::Float1:return 1;
		case flora::BufferElementDataType::Float2:return 2;
		case flora::BufferElementDataType::Float3:return 3;
		case flora::BufferElementDataType::Float4:return 4;
		case flora::BufferElementDataType::UInt1: return 1;
		case flora::BufferElementDataType::UInt2: return 2;
		case flora::BufferElementDataType::UInt3: return 3;
		case flora::BufferElementDataType::UInt4: return 4;
		case flora::BufferElementDataType::Int1:  return 1;
		case flora::BufferElementDataType::Int2:  return 2;
		case flora::BufferElementDataType::Int3:  return 3;
		case flora::BufferElementDataType::Int4:  return 4;
		case flora::BufferElementDataType::Mat2:  return 2 * 2;
		case flora::BufferElementDataType::Mat3:  return 3 * 3;
		case flora::BufferElementDataType::Mat4:  return 4 * 4;
		case flora::BufferElementDataType::Max:
		default:
			break;
		}
		return 0;
	}

	uint32_t GetBufferElementDataTypeSize(flora::BufferElementDataType type)
	{
		switch (type)
		{

		case flora::BufferElementDataType::Bool1: return 1 * 1;
		case flora::BufferElementDataType::Bool2: return 1 * 2;
		case flora::BufferElementDataType::Bool3: return 1 * 3;
		case flora::BufferElementDataType::Bool4: return 1 * 4;
		case flora::BufferElementDataType::Float1:return 4 * 1;
		case flora::BufferElementDataType::Float2:return 4 * 2;
		case flora::BufferElementDataType::Float3:return 4 * 3;
		case flora::BufferElementDataType::Float4:return 4 * 4;
		case flora::BufferElementDataType::UInt1: return 4 * 1;
		case flora::BufferElementDataType::UInt2: return 4 * 2;
		case flora::BufferElementDataType::UInt3: return 4 * 3;
		case flora::BufferElementDataType::UInt4: return 4 * 4;
		case flora::BufferElementDataType::Int1:  return 4 * 1;
		case flora::BufferElementDataType::Int2:  return 4 * 2;
		case flora::BufferElementDataType::Int3:  return 4 * 3;
		case flora::BufferElementDataType::Int4:  return 4 * 4;
		case flora::BufferElementDataType::Mat2:  return 4 * 2 * 2;
		case flora::BufferElementDataType::Mat3:  return 4 * 3 * 3;
		case flora::BufferElementDataType::Mat4:  return 4 * 4 * 4;
		case flora::BufferElementDataType::Max:
		default:
			break;
		}
		return 0;
	}

	std::string BufferElementDataTypeToString(flora::BufferElementDataType type)
	{
		switch (type)
		{
		case flora::BufferElementDataType::Bool1:return "Bool1";
		case flora::BufferElementDataType::Bool2:return "Bool2";
		case flora::BufferElementDataType::Bool3:return "Bool3";
		case flora::BufferElementDataType::Bool4:return "Bool4";
		case flora::BufferElementDataType::Float1:return "Float1";
		case flora::BufferElementDataType::Float2:return "Float2";
		case flora::BufferElementDataType::Float3:return "Float3";
		case flora::BufferElementDataType::Float4:return "Float4";
		case flora::BufferElementDataType::UInt1:return "UInt1";
		case flora::BufferElementDataType::UInt2:return "UInt2";
		case flora::BufferElementDataType::UInt3:return "UInt3";
		case flora::BufferElementDataType::UInt4:return "UInt4";
		case flora::BufferElementDataType::Int1:return "Int1";
		case flora::BufferElementDataType::Int2:return "Int2";
		case flora::BufferElementDataType::Int3:return "Int3";
		case flora::BufferElementDataType::Int4:return "Int4";
		case flora::BufferElementDataType::Mat2:return "Mat2";
		case flora::BufferElementDataType::Mat3:return "Mat3";
		case flora::BufferElementDataType::Mat4:return "Mat4";
		default:
			break;
		}
		return "";
	}

	flora::BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& elements)
		:Elements(elements)
	{
		CalculateOffsetAndStride();
	}

	void flora::BufferLayout::CalculateOffsetAndStride()
	{
		uint32_t offset = 0;
		Stride = 0;
		for (auto& element : Elements) {
			element.Offset = offset;
			offset += element.Size;
			Stride += element.Size;
		}
	}

	flora::BufferElement::BufferElement(BufferElementDataType type, const std::string& name, bool normalized)
	{
		Name = name;
		Type = type;
		Offset = 0;
		Size = GetBufferElementDataTypeSize(type);
		bIsNormalized = normalized;
	}
}