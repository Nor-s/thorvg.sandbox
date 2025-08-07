#include "logger.h"

namespace core
{

std::shared_ptr<spdlog::logger> Logger::sLogger{nullptr};
std::string Logger::sPrefix{""};

Logger::Logger()
{
	if(sPrefix == "")
	{
		SetLogPrefix("log");
	}
	std::vector<spdlog::sink_ptr> logSinks;
	logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	// logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(sPrefix + ".log", true));

	logSinks[0]->set_pattern("%^[%n][%H:%M:%S.%e --%L] %v%$ {%s::%! #%#}");
	// logSinks[1]->set_pattern("[%n][%L][%H:%M:%S.%e] %v");

	sLogger = std::make_shared<spdlog::logger>(sPrefix, begin(logSinks), end(logSinks));
	spdlog::register_logger(sLogger);
	sLogger->set_level(spdlog::level::trace);
	sLogger->flush_on(spdlog::level::trace);
}

void Logger::SetLogPrefix(std::string_view prefix)
{
	sPrefix = prefix;
}

const std::shared_ptr<spdlog::logger>& Logger::getLogger() const
{
	return sLogger;
}

}	 // namespace core