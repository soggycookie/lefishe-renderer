
#glad

set(GLAD_DIR "${PROJECT_SOURCE_DIR}/external/glad")

add_library(glad INTERFACE)
target_include_directories(glad
    INTERFACE ${GLAD_DIR}/include
)

#glm

set(GLM_DIR "${PROJECT_SOURCE_DIR}/external/glm")
add_library(glm INTERFACE)
target_include_directories(glm
    INTERFACE ${GLM_DIR}/include
)


#stb

set(STB_DIR "${PROJECT_SOURCE_DIR}/external/stb")
add_library(stb INTERFACE)
target_include_directories(stb
    INTERFACE ${STB_DIR}/include
)

#imgui master

set(IMGUI_DIR "${PROJECT_SOURCE_DIR}/external/imgui-master/imgui")

file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR})
target_link_libraries(imgui PRIVATE glfw)


include(FetchContent)

# Declare GLFW dependency
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.8
  SOURCE_DIR     ${PROJECT_SOURCE_DIR}/external/glfw
)

# Configure GLFW options (optional)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Make GLFW available
FetchContent_MakeAvailable(glfw)
target_compile_definitions(glfw INTERFACE GLFW_INCLUDE_NONE)


set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG        master   
    SOURCE_DIR     ${PROJECT_SOURCE_DIR}/external/assimp
)

FetchContent_MakeAvailable(assimp)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.12.0
  SOURCE_DIR     ${PROJECT_SOURCE_DIR}/external/spdlog
)

FetchContent_MakeAvailable(spdlog)

