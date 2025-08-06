#include "logger.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using namespace Lefishe;


std::shared_ptr<spdlog::logger> Logger::m_logger ;

void Logger::Init() {
	spdlog::set_pattern("%^[%T] %v%$");
	m_logger = spdlog::stdout_color_mt("Lefishe");
	spdlog::set_level(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Logger::getLogger() {
	return m_logger;
}