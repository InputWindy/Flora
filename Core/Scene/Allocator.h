#pragma once
#include <Common/Common.h>
#include <Common/Serialize.h>

#include <string>
#include <map>
#include <stack>
#include <unordered_set>
#include <tuple>
#include <memory>
#include <cassert>
#include <mutex>
#include <cstdint>  // 添加uintptr_t支持

namespace flora
{
	// 内存池常量定义（使用位移运算明确单位）
	constexpr size_t SMALL_MEMORY_POOL = 4 * (1 << 10);  // 4KB
	constexpr size_t LARGE_MEMORY_POOL = 4 * (1 << 20);  // 4MB

	// 类型声明宏（保持原始设计）
#define DECLARE_TYPE(ClassName) \
        static inline const std::string GetStaticType() {return #ClassName;}\
        virtual const std::string GetType()const override {return #ClassName;}

	// 分配对象接口（保持原始设计）
	struct IAllocable
	{
		virtual const std::string GetType()const = 0;
	};

	// 改进后的内存分配器模板类
	template<
		typename TAllocable,
		unsigned int MAX_ALLOC_SIZE = LARGE_MEMORY_POOL / (
			(sizeof(TAllocable) % alignof(TAllocable) == 0) ?
			sizeof(TAllocable) :
			((sizeof(TAllocable) / alignof(TAllocable)) + 1) * alignof(TAllocable)
			)
	>
	struct TAllocator : public XSingleton<TAllocator<TAllocable, MAX_ALLOC_SIZE>>
	{
	public:
		// 类型别名
		using ThisType = TAllocator<TAllocable, MAX_ALLOC_SIZE>;
		using AllocableType = TAllocable;

		~TAllocator()
		{
			std::lock_guard<std::mutex> lock(AllocMutex);

			// 析构所有已分配对象
			for (unsigned int idx : Used) {
				(Mem + idx)->~TAllocable();
			}

			// 释放对齐内存
#ifdef _WIN32
			_aligned_free(Mem);
#else
			free(Mem);  // Linux/macOS的aligned_alloc用free释放
#endif

			Mem = nullptr;
			Unused = {};
			Used = {};
		}

		// 分配对象（完美转发参数）
		template<typename... Args>
		static TAllocable* Alloc(Args&&... args)
		{
			std::lock_guard<std::mutex> lock(AllocMutex);

			InitializeMemoryPool();

			if (Unused.empty()) return nullptr;

			const unsigned int idx = Unused.top();
			Unused.pop();

			// 验证索引有效性
			assert(Used.find(idx) == Used.end() && "Duplicate allocation index!");
			Used.insert(idx);

			// 验证内存对齐
			assert(reinterpret_cast<uintptr_t>(Mem + idx) % alignof(TAllocable) == 0
				&& "Memory alignment error!");

			// Placement new构造对象
			return new (Mem + idx) TAllocable(std::forward<Args&&>(args)...);
		}

		// 释放对象
		static void Dealloc(TAllocable* allocable)
		{
			std::lock_guard<std::mutex> lock(AllocMutex);
			if (!allocable) return;

			// 计算偏移量并验证有效性
			const uintptr_t offset = reinterpret_cast<uintptr_t>(allocable)
				- reinterpret_cast<uintptr_t>(Mem);
			assert(offset % AlignSize == 0 && "Invalid object alignment!");
			const unsigned int idx = static_cast<unsigned int>(offset / AlignSize);

			// 析构对象并回收索引
			allocable->~TAllocable();
			Used.erase(idx);
			Unused.push(idx);
		}

		// 获取使用统计（线程安全）
		static std::tuple<unsigned int, unsigned int> GetNum()
		{
			std::lock_guard<std::mutex> lock(AllocMutex);
			return { Used.size(), Unused.size() };
		}

		// 获取已用索引集合（返回副本保证线程安全）
		static std::unordered_set<unsigned int> GetUsed()
		{
			std::lock_guard<std::mutex> lock(AllocMutex);
			return Used;
		}

		// 公开常量
		static inline constexpr unsigned int MaxSize = MAX_ALLOC_SIZE;
		static inline constexpr size_t AlignSize =
			(sizeof(TAllocable) % alignof(TAllocable) == 0) ?
			sizeof(TAllocable) :
			((sizeof(TAllocable) / alignof(TAllocable)) + 1) * alignof(TAllocable);

	private:
		// 初始化内存池（仅首次调用时执行）
		static void InitializeMemoryPool()
		{
			if (!Mem) {
				constexpr size_t totalSize = AlignSize * MAX_ALLOC_SIZE;

				// 跨平台对齐内存分配
#ifdef _WIN32
				Mem = static_cast<TAllocable*>(_aligned_malloc(totalSize, alignof(TAllocable)));
#else
				Mem = static_cast<TAllocable*>(aligned_alloc(alignof(TAllocable), totalSize));
#endif

				assert(Mem && "Memory allocation failed!");

				// 初始化索引栈
				for (unsigned int i = 0; i < MAX_ALLOC_SIZE; ++i) {
					Unused.push(i);
				}
			}
		}

	private:
		// 静态成员（C++17 inline初始化）
		static inline TAllocable* Mem = nullptr;
		static inline std::stack<unsigned int> Unused;
		static inline std::unordered_set<unsigned int> Used;
		static inline std::mutex AllocMutex;
	};
}