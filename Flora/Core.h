#pragma once
#include <Windows.h>
#include <winnls.h>
#include <string>
#ifdef FLORA_CORE
#define FLORA_API __declspec(dllexport)
#else
#define FLORA_API __declspec(dllimport)
#endif // FLORA_CORE

namespace Utils
{
	FLORA_API std::string unicode2utf8(const std::string& str);
}

#include <memory>
template<typename _T>
using Ref = std::shared_ptr<_T>;

template<typename _T>
using Scope = std::unique_ptr<_T>;

template<typename _T>
using Ptr = std::weak_ptr<_T>;

