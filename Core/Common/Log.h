#pragma once
#include <Common/Common.h>
#include <string.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
namespace flora
{
	class XLogger
	{
	public:
		XLogger() = default;
		~XLogger() = default;

		static inline void InitLogger(const std::string& LogPath)
		{
			spdlog::set_pattern("%^[%T] %n:%v%$");
			CoreStdLogger = spdlog::stdout_color_mt("Flora");

			//GlobalUtils::CreateDirectoryRecursive(LogPath);

			CoreBasicLogger = spdlog::basic_logger_mt("FloraBasic", LogPath);
			CoreStdLogger->set_level(spdlog::level::trace);
			CoreBasicLogger->set_level(spdlog::level::trace);
		}

		static inline void Shutdown()
		{
			CoreStdLogger->flush();
			CoreBasicLogger->flush();
			CoreStdLogger = nullptr;
			CoreBasicLogger = nullptr;
		}
		inline static std::shared_ptr<spdlog::logger>& GetCoreStdLogger() { return CoreStdLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetCoreBasicLogger() { return CoreBasicLogger; }

	private:
		static inline std::shared_ptr<spdlog::logger> CoreStdLogger;
		static inline std::shared_ptr<spdlog::logger> CoreBasicLogger;
	};
}

#define FLORA_CORE_ERROR(...)		::flora::XLogger::GetCoreStdLogger()->error(__VA_ARGS__);::flora::XLogger::GetCoreBasicLogger()->error(__VA_ARGS__);::flora::XLogger::GetCoreStdLogger()->flush();::flora::XLogger::GetCoreBasicLogger()->flush();
#define FLORA_CORE_WARN(...)		::flora::XLogger::GetCoreStdLogger()->warn(__VA_ARGS__); ::flora::XLogger::GetCoreBasicLogger()->warn(__VA_ARGS__); ::flora::XLogger::GetCoreStdLogger()->flush();::flora::XLogger::GetCoreBasicLogger()->flush();
#define FLORA_CORE_INFO(...)		::flora::XLogger::GetCoreStdLogger()->info(__VA_ARGS__); ::flora::XLogger::GetCoreBasicLogger()->info(__VA_ARGS__); ::flora::XLogger::GetCoreStdLogger()->flush();::flora::XLogger::GetCoreBasicLogger()->flush();
#define FLORA_CORE_TRACE(...)		::flora::XLogger::GetCoreStdLogger()->trace(__VA_ARGS__);::flora::XLogger::GetCoreBasicLogger()->trace(__VA_ARGS__);::flora::XLogger::GetCoreStdLogger()->flush();::flora::XLogger::GetCoreBasicLogger()->flush();