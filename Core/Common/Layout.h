#pragma once
#include <string>
#include <vector>
namespace flora
{
    enum class BufferElementDataType
	{
		Bool1, Bool2, Bool3, Bool4,
		Float1, Float2, Float3, Float4,
		UInt1, UInt2, UInt3, UInt4,
		Int1, Int2, Int3, Int4,
		Mat2, Mat3, Mat4,

		Max
	};

	/// <summary>
	/// buffer element 
	/// </summary>
	struct BufferElement
	{
		std::string Name;

		BufferElementDataType Type{};

		uint32_t Offset{};
		uint32_t Size{};
		bool bIsNormalized{};

		BufferElement() = default;
		BufferElement(BufferElementDataType type, const std::string& name, bool normalized = false);
	};

	/// <summary>
	/// the buffer layout describes the fields within a stride and the length of the stride.
	/// </summary>
	struct BufferLayout
	{
		BufferLayout() = default;
		BufferLayout(const std::initializer_list<BufferElement>&);

		std::vector<BufferElement>::iterator begin() { return Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return Elements.end(); }

		[[nodiscard]] std::vector<BufferElement>::const_iterator begin()const { return Elements.begin(); }
		[[nodiscard]] std::vector<BufferElement>::const_iterator end()const { return Elements.end(); }

		inline void AddElement(BufferElement ele) { Elements.emplace_back(ele); }
		inline void Clear() { Elements.clear(); }

		inline uint32_t GetStride()const { return Stride; }
		inline bool GetElement(const std::string& name, BufferElement& inout)const
		{
			for (size_t Idx = 0; Idx < Elements.size(); Idx++)
			{
				if (Elements[Idx].Name == name)
				{
					inout = Elements[Idx];
					return true;
				}
			}
			return false;
		}
	public:
		void CalculateOffsetAndStride();
	private:
		std::vector<BufferElement> Elements;
		uint32_t Stride = 0;
	};
}