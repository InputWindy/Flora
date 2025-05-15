#pragma once
#include <Common/Common.h>
#include <string>
#include <map>
#include <stack>
#include <memory>
#include <cassert>

namespace flora
{
#define SMALL_ALLOC_SIZE 1000
#define MIDDLE_ALLOC_SIZE SMALL_ALLOC_SIZE * SMALL_ALLOC_SIZE
#define LARGE_ALLOC_SIZE  SMALL_ALLOC_SIZE * MIDDLE_ALLOC_SIZE

#define DECLARE_TYPE(ClassName) \
	static inline const std::string GetStaticType() {return #ClassName;}\
	virtual const std::string GetType()const override {return #ClassName;}

	struct IAllocable
	{
		virtual const std::string GetType()const = 0;
	};

	template<unsigned int MAX_ALLOC_SIZE = SMALL_ALLOC_SIZE>
	struct TAllocator:public XSingleton<TAllocator<MAX_ALLOC_SIZE>>
	{
		~TAllocator()
		{
			for (auto& Buffer : Mem)
			{
				free(Buffer.second);
				Buffer.second = nullptr;
			}

			Unused.clear();
		}

		template<typename TAllocable,typename ...Args>
		static TAllocable* Alloc(Args&&... args)
		{
			unsigned int TypeSize = sizeof(TAllocable);
			std::string Category = TAllocable::GetStaticType();

			// init memory
			if (!Mem[Category])
			{
				IAllocable* Buffer = (IAllocable*)malloc(MAX_ALLOC_SIZE * TypeSize);
				memset(Buffer, 0, MAX_ALLOC_SIZE * TypeSize);
				Mem[Category] = Buffer;

				int T = MAX_ALLOC_SIZE;
				while (T--)Unused[Category].push(T);
			}

			if (Unused[Category].empty())return nullptr;

			unsigned int Idx = Unused[Category].top(); Unused[Category].pop();

			assert(reinterpret_cast<unsigned int>(Mem[Category] + Idx) % alignof(TAllocable) == 0 && "error alignment !");

			return new(Mem[Category] + Idx) TAllocable(std::forward<Args>(args)...);
		}

		template<typename TAllocable>
		static void Dealloc(TAllocable* Allocable)
		{
			if (!Allocable)return;

			unsigned int TypeSize = sizeof(TAllocable);
			std::string Category = TAllocable::GetStaticType();

			assert(
				(((unsigned int)Allocable - (unsigned int)Mem[Category]) % TypeSize) == 0 &&
				"Dealloc bad allocable type!"
			);

			Allocable->~TAllocable();
			
			Unused[Category].push(((unsigned int)Allocable - (unsigned int)Mem[Category]) / TypeSize);
		}

		template<typename TAllocable>
		static unsigned int GetNum()
		{
			std::string Category = TAllocable::GetStaticType();

			return MAX_ALLOC_SIZE - Unused[Category].size();
		}

		static unsigned int GetNum()
		{
			unsigned int Num = 0;
			for (auto& U : Unused)
			{
				Num += MAX_ALLOC_SIZE - U.second.size();
			}

			return Num;
		}

	private:
		static inline std::map<std::string, IAllocable*> Mem;
		static inline std::map<std::string, std::stack<unsigned int>> Unused;
	};

}