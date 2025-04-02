cmake_minimum_required(VERSION 3.17)
project(Core LANGUAGES CXX)

add_definitions(-DNOMINMAX)
add_definitions(-DNOGDI)

if (FLORA_BUILD_WITH_DOUBLE_SCALAR)
	message(STATUS "Building with double precision")
	set(FLORA_SCALAR "double")
    add_definitions(-DFLORA_SCALAR=double)
else()
	message(STATUS "Building with single precision")
	set(FLORA_SCALAR "float")
    add_definitions(-DFLORA_SCALAR=float)
endif()


#compile source files
file(GLOB_RECURSE CORE_SOURCES ./*.cpp)
file(GLOB_RECURSE CORE_HEADERS ./*.h ./*.hpp)
file(GLOB_RECURSE CORE_SHADERS ./*.glsl)

add_library(Core STATIC
${CORE_SOURCES} 
${CORE_HEADERS} 
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
${CORE_SOURCES} 
${CORE_HEADERS} 
${CORE_SHADERS} 
)

file(GLOB CORE_FILES
        ./*.c
        ./*.cpp
        ./*.h
        ./*.hpp
        ./**/*.c
        ./**/*.h
        ./**/*.hpp
        ./**/*.cpp
)

# 使用 file(GLOB ...) 获取所有.c和.h文件
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${CORE_FILES} )
target_include_directories(Core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty
    ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/libigl/eigen-src
    ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/libigl/libigl-src/include
)

if(WITH_IMGUI)
    target_link_libraries(Core glad glfw glm imgui stb spdlog::spdlog json11 assimp argparse vcglib metis Spout_static)
else()
    if(WIN32)
        target_link_libraries(Core glad glfw glm stb spdlog::spdlog json11 assimp argparse  vcglib metis Spout_static)
    else()
        target_link_libraries(Core glad glfw glm stb spdlog::spdlog json11 assimp EGL argparse vcglib metis)
    endif()
endif()

target_compile_features(Core PRIVATE cxx_std_20)
set_target_properties(Core PROPERTIES CXX_EXTENSIONS OFF)
# target_include_directories(core PUBLIC window)
# target_include_directories(core PUBLIC render_core)
# target_include_directories(core PUBLIC common)
# target_include_directories(core PUBLIC resource)
# target_include_directories(core PUBLIC scene)
# target_include_directories(core PUBLIC geo)
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(Core PUBLIC MESHLAB_SCALAR=${MESHLAB_SCALAR})


if (MSVC)
    target_compile_options(Core PRIVATE /W3)
else()
    target_compile_options(Core PRIVATE -Wall -Wextra -Wpedantic)
endif()

# install(TARGETS core LIBRARY DESTINATION lib)
