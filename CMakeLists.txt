#只需要配置PROJECT_NAME和PROJECT_FILE就可以切换要编译的项目，PROJECT_NAME对应VS中的项目名称以及编译出的exe的名称，PROJECT_FILE是项目根目录名称
#INCLUDE_MODULES添加子模块，MACRO_NAME添加预编译宏

#WORK_DIR请填写.exe的安装目录

#project settings
set(PROJECT_NAME "GI")
set(PROJECT_FILE GI)
set(CMAKE_MIN_VERSION 3.17)
set(CPP_VERSION 20)
set(WITH_IMGUI 1)
set(WORK_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

# pybind11 export
set(IsPythonLib False) 

#sub module
set(INCLUDE_MODULES
    FetchContent
)

#macro
set(MACRO_NAME
    -DGLM_FORCE_LEFT_HANDED
    -DNOMINMAX
    # -DPREVIEW_MODE
    # -DNOGDI

    -DAPP_NAME="${PROJECT_NAME}"

    -DROOT_DIR="${WORK_DIR}"
    -DBINARY_ROOT_DIR="${WORK_DIR}/bin"
    -DSHADER_ROOT_DIR="${CMAKE_SOURCE_DIR}/${PROJECT_FILE}/Shaders/"

    -DENGINE_CORE_DIR="${WORK_DIR}/Core"
    -DENGINE_CONTENT_DIR="${WORK_DIR}/Core/content"

    -DPROJECT_DIR="${WORK_DIR}/${PROJECT_FILE}"
    -DPROJECT_CONTENT_DIR="${WORK_DIR}/${PROJECT_FILE}/content"

    -DFLORA_SCALAR=float
)

#=================generic project=====================#
cmake_minimum_required(VERSION ${CMAKE_MIN_VERSION})
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD ${CPP_VERSION})

set(ProjectName ${PROJECT_FILE})

# 创建项目文件夹
if(EXISTS ${CMAKE_SOURCE_DIR}/${PROJECT_FILE})
else()
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJECT_FILE})
endif()

# 创建Shader文件夹
if(EXISTS ${CMAKE_SOURCE_DIR}/${PROJECT_FILE}/Shaders)
else()
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJECT_FILE}/Shaders)
endif()

# 创建模板文件
if(
    EXISTS ${CMAKE_SOURCE_DIR}/${PROJECT_FILE}/main.cpp )
else()

# 设置源目录和目标目录
set(SRC_DIR ${CMAKE_SOURCE_DIR}/template)
set(DST_DIR ${CMAKE_SOURCE_DIR}/${PROJECT_FILE})

# 递归查找所有 .in 文件
file(GLOB_RECURSE IN_FILES "${SRC_DIR}/*.in")

# 遍历每一个 .in 文件
foreach(IN_FILE ${IN_FILES})
    # 获取相对路径
    file(RELATIVE_PATH REL_PATH ${SRC_DIR} ${IN_FILE})

    # 去掉 .in 后缀并生成目标文件路径
    string(REPLACE ".in" "" OUT_FILE ${REL_PATH})
    set(OUT_FILE_PATH "${DST_DIR}/${OUT_FILE}")

    # 确保目标目录存在
    get_filename_component(OUT_DIR ${OUT_FILE_PATH} DIRECTORY)
    file(MAKE_DIRECTORY ${OUT_DIR})

    # 生成配置文件
    configure_file(${IN_FILE} ${OUT_FILE_PATH} @ONLY)
endforeach()

# configure_file(${CMAKE_SOURCE_DIR}/template/main.cpp.in ${CMAKE_SOURCE_DIR}/${PROJECT_FILE}/main.cpp @ONLY)
# configure_file(${CMAKE_SOURCE_DIR}/template/Editor.h.in ${CMAKE_SOURCE_DIR}/${PROJECT_FILE}/Editor.h @ONLY)
# configure_file(${CMAKE_SOURCE_DIR}/template/Editor.cpp.in ${CMAKE_SOURCE_DIR}/${PROJECT_FILE}/Editor.cpp @ONLY)
endif()

#compile source files
file(GLOB_RECURSE PROJECT_SOURCES ${PROJECT_FILE}/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS ${PROJECT_FILE}/*.h)
file(GLOB_RECURSE PROJECT_SHADERS 
    ${PROJECT_FILE}/*.glsl
    ${PROJECT_FILE}/*.vert
    ${PROJECT_FILE}/*.frag
    ${PROJECT_FILE}/*.comp
)

set(TARGET_SOURCES 
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    #${PROJECT_SHADERS}
#
## 如果希望在main中把加载fbx的逻辑用起来，需要加入下面这一些
    # 3rdparty/vcglib/wrap/io_trimesh/import_fbx.h
    # 3rdparty/vcglib/wrap/io_trimesh/io_mask.h
    # 3rdparty/vcglib/wrap/openfbx/src/miniz.h
    # 3rdparty/vcglib/wrap/openfbx/src/miniz.c
    # 3rdparty/vcglib/wrap/openfbx/src/ofbx.h
    # 3rdparty/vcglib/wrap/openfbx/src/ofbx.cpp
)

#cmake sub-directories
set(SUBDIRECTORIES 
    3rdparty
    Core 
)

#includes
set(TARGET_INCLUDE 
    ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty
)

#libs
set(TARGET_LINK
    ${PROJECT_FILE}
)


# Win32 or x64 build
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(OUTPUT_ARCH_DEST "Win32")
else()
  set(OUTPUT_ARCH_DEST "x64")
endif()

if(IsPythonLib)
    list(APPEND MACRO_NAME -DPY_MODULE_NAME=${PROJECT_FILE})
endif()

if(WIN32)
    list(APPEND TARGET_LINK PUBLIC Core)
    list(APPEND MACRO_NAME -DWITH_GLFW)
    list(APPEND MACRO_NAME -DWITH_GUI)
else()
    list(APPEND TARGET_LINK PUBLIC -ldl Core)
    list(APPEND MACRO_NAME -DWITH_EGL)
endif()

foreach(STRING ${INCLUDE_MODULES})
    include(${STRING})
endforeach()

foreach(STRING ${MACRO_NAME})
    add_definitions(${STRING})
endforeach()

if(WIN32)
set(WITH_GLFW 1)
else()
set(WITH_IMGUI 0)
set(WITH_EGL 1)
endif()

foreach(STRING ${SUBDIRECTORIES})
    add_subdirectory(${STRING})
endforeach()

# 查找 Python3 包，包括开发和解释器组件
find_package(Python3 REQUIRED COMPONENTS Development Interpreter)

if(IsPythonLib)
    # 查找 Pybind11 包
    find_package(pybind11 REQUIRED)
    pybind11_add_module(${PROJECT_FILE} ${PROJECT_SOURCES})
else()
    add_executable(${PROJECT_FILE} ${PROJECT_FILE}/main.cpp)
endif()

# 设置 .glsl 文件不参与编译
set_source_files_properties(${PROJECT_SHADERS} PROPERTIES HEADER_FILE_ONLY ON)

target_sources(${PROJECT_FILE} PUBLIC 
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_SHADERS}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_SHADERS}
)

target_include_directories(${PROJECT_FILE} PUBLIC ${TARGET_INCLUDE})

target_link_libraries(${TARGET_LINK} ${Python3_LIBRARIES} pybind11::module)



# ## libigl example 
# add_executable( main_libigl  ${PROJECT_FILE}/example/main_libigl.cpp )
# target_link_libraries(main_libigl PUBLIC igl::glfw  igl::embree )


# add_custom_command(TARGET ${PROJECT_FILE} POST_BUILD
#         COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:app>/assets
#         COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:app>/assets
# )
