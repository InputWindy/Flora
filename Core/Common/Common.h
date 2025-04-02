#pragma once
#include <string>
#include <memory>
namespace flora
{
	template<typename T>
	class XSingleton
	{
	protected:
		XSingleton() {}

	public:
		static T& Get() noexcept(std::is_nothrow_constructible<T>::value)
		{
			static T instance;
			return instance;
		}
		virtual ~XSingleton() noexcept = default;
		XSingleton(const XSingleton&) = delete;
		XSingleton& operator=(const XSingleton&) = delete;
	};

	struct INoneCopyable
	{
		INoneCopyable() = default;
		virtual ~INoneCopyable() = default;

		INoneCopyable(const INoneCopyable&) = delete;
		INoneCopyable(INoneCopyable&&) = delete;

		void operator=(const INoneCopyable&) = delete;
		void operator=(INoneCopyable&&) = delete;
	};

	
	struct ISharable :public std::enable_shared_from_this<ISharable>
	{
		ISharable() = default;
		virtual ~ISharable() = default;

		template<typename T>
		T* DynamicCast()
		{
			return dynamic_cast<T*>(this);
		}
		template<typename T>
		std::shared_ptr<T> DynamicPointerCast()
		{
			return std::dynamic_pointer_cast<T>(this->shared_from_this());
		}
		template<typename T>
		T* StaticCast()
		{
			return static_cast<T*>(this);
		}
		template<typename T>
		std::shared_ptr<T> StaticPointerCast()
		{
			return std::static_pointer_cast<T>(this->shared_from_this());
		}
		template<typename T>
		T* ReinterpretCast()
		{
			return reinterpret_cast<T*>(this);
		}
	};

	struct GlobalUtils
	{
		static bool CreateDirectoryRecursive(const std::string& path);
	};
	
	void PrintProgressBar(float cc, int allNum);

	int MinPowerOfTwo(int x);
	

	std::pair<int, int> CalGroupNum(int ClusterNum);

}



