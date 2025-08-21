cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(PROJECT_NAME "lefishe-renderer")

project(${PROJECT_NAME})

# Detect architecture (x64 or x86)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PLATFORM_FOLDER "x64")
else()
    set(PLATFORM_FOLDER "x32")
endif()

# Set output directory for binaries (EXE)
set(OUTPUT_DIR "${PROJECT_SOURCE_DIR}/bin/${PLATFORM_FOLDER}/$<CONFIG>")

# Set output directory for intermediate files (OBJ, etc.)
set(INTERMEDIATE_DIR "${PROJECT_SOURCE_DIR}/bin/intermediate/${PLATFORM_FOLDER}/$<CONFIG>")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${INTERMEDIATE_DIR})


# For multi-config generators (like Visual Studio)
foreach(OUTPUTCONFIG Debug Release RelWithDebInfo MinSizeRel)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${OUTPUT_DIR})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${INTERMEDIATE_DIR})
endforeach()

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/glad/glad.c"
)

set(THIRDPARTY "${PROJECT_SOURCE_DIR}/external")

add_subdirectory(${THIRDPARTY})

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} 
    PRIVATE glfw
    PRIVATE glad
    PRIVATE glm
    PRIVATE imgui
    PRIVATE assimp
    PRIVATE stb
    PRIVATE spdlog
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE "${PROJECT_SOURCE_DIR}/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/include"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/resource"
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/resource
)


