#pragma once

#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <queue>
#include <deque>
#include <memory>
#include <utility>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <cmath>
#include <stack>
#include <list>
#include <thread>
#include <mutex>

#include "glm/glm.hpp"


#ifdef DEBUG_MODE

enum ErrorLevel {
    INFO,
    WARNING,
    FATAL
};

#define ERROR_LEVEL(level, message) \
    switch(level){ \
            case ErrorLevel::INFO: \
                std::cout << "INFO::" << message << std::endl; \
                break; \
            case ErrorLevel::WARNING: \
                std::cout << "WARNING::" << message << std::endl; \
                break; \
            case ErrorLevel::FATAL: \
                std::cout << "FATAL::" << message << std::endl; \
                break; \
        }\

#define DEBUG_ASSERT(condition, message, level) \
    if (condition) { \
	    ERROR_LEVEL(level, message); \
    }\

#else
	#define DEBUG_ASSERT(condition, message, level, type) 
#endif

using UINT   = std::uint32_t;
using INT    = std::int32_t;
using FLOAT  = std::float_t;
using DOUBLE = std::double_t;
using STRING = std::string;
using BOOL   = bool;

using STRINGVIEW = std::string_view;
using CHAR_STRING = char*;

using SIZE = std::size_t;

using IVEC2 = glm::ivec2;
using DVEC2 = glm::dvec2;
using VEC2  = glm::vec2;
using VEC3  = glm::vec3;
using VEC4  = glm::vec4;
using MAT4  = glm::mat4;

