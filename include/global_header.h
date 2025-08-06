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
#include <functional>

#include "glm/glm.hpp"
#include "logger.h"

namespace Lefishe {

    using UINT = std::uint32_t;
    using INT = std::int32_t;
    using FLOAT = std::float_t;
    using DOUBLE = std::double_t;
    using STRING = std::string;

    using STRINGVIEW = std::string_view;

    using SIZE = std::size_t;

    using IVEC2 = glm::ivec2;
    using DVEC2 = glm::dvec2;
    using VEC2 = glm::vec2;
    using VEC3 = glm::vec3;
    using VEC4 = glm::vec4;
    using MAT4 = glm::mat4;
    using TIMESTAMP = std::time_t;
}