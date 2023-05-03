#pragma once
#include "Core.h"
#include <string>
#include <assert.h>
#include <vector>
#include <sstream>

#define PUSH_ERROR_MSG(Category,...) \
FConsoleContext::Get().AddDebugLog(EMT_Error,Category,__FILE__,__FUNCTION__,__LINE__,__VA_ARGS__);\
++FConsoleContext::Get().ErrorNum;

#define PUSH_WARN_MSG(Category,...) \
FConsoleContext::Get().AddDebugLog(EMT_Warn,Category,__FILE__,__FUNCTION__,__LINE__,__VA_ARGS__);\
++FConsoleContext::Get().WarnNum;

#define PUSH_INFO_MSG(Category,...) \
FConsoleContext::Get().AddDebugLog(EMT_Info,Category,__FILE__,__FUNCTION__,__LINE__,__VA_ARGS__);\
++FConsoleContext::Get().InfoNum;

#define PUSH_TRACE_MSG(Category,...) \
FConsoleContext::Get().AddDebugLog(EMT_Trace,Category,__FILE__,__FUNCTION__,__LINE__,__VA_ARGS__);\
++FConsoleContext::Get().TraceNum;

using namespace std;
enum
{
	EMT_None  = 0,
	EMT_Error = 1 << 0,
	EMT_Warn  = 1 << 1,
	EMT_Trace = 1 << 2,
	EMT_Info  = 1 << 3,
};

struct FLORA_API FConsoleContext
{
	friend class EditorScript;

	struct FDebugMsg
	{
		uint8_t type = EMT_None;

		const char* category = "";

		const char* file	 = "";
		const char* function = "";
		int			line_num = 0;

		string		comment	 = "";
	};

	static inline FConsoleContext& Get() { static FConsoleContext msConsole; return msConsole; };

	void AddDebugLog(uint8_t Type, const char* category, const char* file, const char* function, int line_num, const char* fmt, ...);
	void Clear();
public:
	uint32_t ErrorNum = 0;
	uint32_t WarnNum  = 0;
	uint32_t TraceNum = 0;
	uint32_t InfoNum  = 0;
private:
	std::vector<FDebugMsg> MsgBuffer;
};