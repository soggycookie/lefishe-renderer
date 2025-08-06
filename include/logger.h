#pragma once
#include "global_header.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "event_bus.h"
#include "spdlog/spdlog.h"



namespace Lefishe {


    class Logger {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& getLogger();

    private:
        static std::shared_ptr<spdlog::logger> m_logger;
    };
}

#define LOG_INFO(...) Lefishe::Logger::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) Lefishe::Logger::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Lefishe::Logger::getLogger()->error(__VA_ARGS__)
#define LOG_TRACE(...) Lefishe::Logger::getLogger()->trace(__VA_ARGS__)

#define LOG_INIT() 	Lefishe::Logger::Init()
