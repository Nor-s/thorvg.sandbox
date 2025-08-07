#ifndef _NS_GRAPHICS_CORE_COMMONS_LOGGER_H_
#define _NS_GRAPHICS_CORE_COMMONS_LOGGER_H_

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#define SPDLOG_DEBUG_ON
#define SPDLOG_TRACE_ON

#define SPDLOG_NO_EXCEPTIONS

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "singleInstance.h"

#include <string>
#include <string_view>

namespace core
{

class Logger : public SingleInstance<Logger>
{
	friend class SingleInstance<Logger>;
	static std::shared_ptr<spdlog::logger> sLogger;
	static std::string sPrefix;

private:
	Logger();

public:
	~Logger() = default;

	static void SetLogPrefix(std::string_view prefix);
	[[nodiscard]] const std::shared_ptr<spdlog::logger>& getLogger() const;
};

}	 // namespace core

#define LOG_INFO(...) SPDLOG_LOGGER_INFO(::core::Logger::Get().getLogger(), __VA_ARGS__)
#define LOG_ERROR(...) SPDLOG_LOGGER_ERROR(::core::Logger::Get().getLogger(), __VA_ARGS__)
#define LOG_WARN(...) SPDLOG_LOGGER_WARN(::core::Logger::Get().getLogger(), __VA_ARGS__)
#define LOG_TRACE(...) SPDLOG_LOGGER_TRACE(::core::Logger::Get().getLogger(), __VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_LOGGER_CRITICAL(::core::Logger::Get().getLogger(), __VA_ARGS__)

#endif