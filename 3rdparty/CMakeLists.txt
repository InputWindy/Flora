set(third_party_folder "ThirdParty")

# add libigl
# include(libigl.cmake)#不能直接include，libigl依赖使用的glad版本太低，路径还有冲突

# if(WIN32)
include(vcglib.cmake)
# endif()

# #pybind
# # 查找 Python3 包，包括开发和解释器组件
# find_package(Python3 REQUIRED COMPONENTS Development Interpreter)

add_subdirectory(pybind11)

if(WITH_IMGUI)
if(NOT TARGET imgui)
  include(imgui.cmake)
  set_target_properties(imgui PROPERTIES FOLDER ${third_party_folder}/imgui)
endif()
endif()

if(NOT TARGET metis)
  add_subdirectory(metis)
  set_target_properties(metis PROPERTIES FOLDER ${third_party_folder}/metis)
endif()

if(WIN32)

if(NOT TARGET Spout)
  add_subdirectory(Spout)
  target_include_directories(Spout PUBLIC ${third_party_folder}/Spout)
  set_target_properties(Spout PROPERTIES FOLDER ${third_party_folder}/Spout)
endif()

endif()

if(NOT TARGET glm)
  option(BUILD_STATIC_LIBS "" ON)
  option(BUILD_TESTING "" OFF)
  option(GLM_TEST_ENABLE "" OFF)
  add_subdirectory(glm)
  set_target_properties(glm_static PROPERTIES FOLDER ${third_party_folder}/glm)
endif()

if(NOT TARGET glad)
  set(glad_SOURCE_DIR ./glad)
  add_library(
    glad
    ${glad_SOURCE_DIR}/src/glad.c
    ${glad_SOURCE_DIR}/include/glad/glad.h
    ${glad_SOURCE_DIR}/include/KHR/khrplatform.h
  )
  target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include)
  set_target_properties(glad PROPERTIES FOLDER ${third_party_folder}/glad)
endif()

if(NOT TARGET glfw)
  option(GLFW_BUILD_EXAMPLES "" OFF)
  option(GLFW_BUILD_TESTS "" OFF)
  option(GLFW_BUILD_DOCS "" OFF)
  option(GLFW_INSTALL "" OFF)
  add_subdirectory(glfw)
  set_target_properties(glfw PROPERTIES FOLDER ${third_party_folder}/glfw)
  set_target_properties(update_mappings PROPERTIES FOLDER ${third_party_folder}/glfw)
endif()

if(NOT TARGET spdlog)
  option(SPDLOG_BUILD_EXAMPLE "" OFF)
  option(SPDLOG_INSTALL "" OFF)
  add_subdirectory(spdlog)
  set_target_properties(spdlog PROPERTIES FOLDER ${third_party_folder}/spdlog)
endif()

if(NOT TARGET stb)
  include(stb.cmake)
  set_target_properties(stb PROPERTIES FOLDER ${third_party_folder}/stb)
endif()

if(NOT TARGET json11)
  add_subdirectory(json11)

  if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(json11 PRIVATE /WX-)
  else()
    target_compile_options(json11 PRIVATE -Wno-error)
  endif()

  set_target_properties(json11 PROPERTIES FOLDER ${third_party_folder}/json11)
endif()

if(NOT TARGET assimp)
  option(BUILD_SHARED_LIBS "BUILD_SHARED_LIBS" OFF)

  # open fbx\ply\obj importer
  set(ASSIMP_BUILD_FBX_IMPORTER TRUE)
  set(ASSIMP_BUILD_OBJ_IMPORTER TRUE)
  set(ASSIMP_BUILD_PLY_IMPORTER TRUE)

  # open fbx\ply\obj exporter
  set(ASSIMP_BUILD_FBX_EXPORTER TRUE)
  set(ASSIMP_BUILD_OBJ_EXPORTER TRUE)
  set(ASSIMP_BUILD_PLY_EXPORTER TRUE)

  add_subdirectory(assimp)
  include_directories(assimp/include/)

  if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(assimp PRIVATE /WX-)
  else()
    target_compile_options(assimp PRIVATE -Wno-error)
  endif()

  # target_include_directories(assimp PUBLIC ./assimp/include )
  set_target_properties(assimp PROPERTIES FOLDER ${third_party_folder}/assimp)
endif()

if(NOT TARGET argparse)
  add_subdirectory(argparse-3.0)
  set_target_properties(argparse PROPERTIES FOLDER ${third_party_folder}/argparse-3.0)
endif()

# if(NOT TARGET TracyClient)
#   set(TRACY_ENABLE ON CACHE BOOL "Enable profiling")

#   # set(TRACY_NO_SYSTEM_TRACING ON CACHE BOOL "Disable System Tracing")
#   set(TRACY_ONLY_IPV4 ON CACHE BOOL "IPv4 only")

#   option(TRACY_ENABLE "Enable profiling" ON)

#   # option(TRACY_NO_SYSTEM_TRACING "Disable System Tracing" ON)
#   option(TRACY_ONLY_IPV4 "IPv4 only" ON)
#   message("Fetching tracy")
#   add_subdirectory(tracy)
# endif()
