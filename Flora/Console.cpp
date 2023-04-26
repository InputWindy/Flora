#include "Console.h"
#include <stdarg.h>
#include <iostream>

void FConsoleContext::AddDebugLog(uint8_t Type, const char* category, const char* file, const char* function, int line_num,const char* fmt,...)
{
    auto power = [](int n, int m)->int
    {
        int result = 1;
        for (int i = 0; i < m; i++) {
            result *= n;
        }
        return result;
    };
    auto tostring = [](const char* format, va_list args)->string
    {
        stringstream ss;
        // 遍历格式字符串
        while (*format != '\0')
        {
            if (*format == '%')
            { // 遇到%符号
                format++; // 跳过%
                switch (*format)
                {
                case'd':
                {
                    int num = va_arg(args, int);
                    if (num < 0)
                    {
                        ss << '-';
                        num = -num;
                    }
                    ss << to_string(num);
                    break;
                }
                case'f':
                {
                    double num = va_arg(args, double);
                    if (num < 0)
                    {
                        ss << '-';
                        num = -num;
                    }
                    ss << to_string(num);
                    break;
                }
                case'c':
                {
                    char ch = va_arg(args, char);
                    ss << (ch);
                }
                case's':
                {
                    char* str = va_arg(args, char*);
                    while (*str != '\0')
                    {
                        ss << (*str);
                        str++;
                    }
                }
                default:
                {
                    ss << '%';
                    break;
                }
                }
            }
            else
            {
                ss << (*format);
            }
            format++;
        }
        return ss.str();
    };

	FDebugMsg Msg;
	Msg.type = Type;
	Msg.category = category;

	va_list args;
    va_start(args, fmt);
    Msg.comment = tostring(fmt, args);
    va_end(args);
    

	Msg.file = file;
	Msg.function = function;
	Msg.line_num = line_num;

	MsgBuffer.emplace_back(std::move(Msg));
}

void FConsoleContext::Clear()
{
	ErrorNum = 0;
	WarnNum = 0;
	TraceNum = 0;
	InfoNum = 0;

	MsgBuffer.clear();
}
