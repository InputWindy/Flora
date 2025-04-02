# Libigl
## on\on\off： x
## on\on\on:   x
## on\off\off: good
# Build options
option(LIBIGL_BUILD_TESTS        "Build libigl unit test"                OFF)
option(LIBIGL_BUILD_TUTORIALS    "Build libigl tutorial"                 OFF)
option(LIBIGL_INSTALL            "Enable installation of libigl targets" ON)
option(LIBIGL_USE_STATIC_LIBRARY "Use libigl as static library"          ON)

# Module options
option(LIBIGL_EMBREE              "Build target igl::embree"              OFF)
option(LIBIGL_GLFW                "Build target igl::glfw"                ON)
option(LIBIGL_IMGUI               "Build target igl::imgui"               OFF)
option(LIBIGL_OPENGL              "Build target igl::opengl"              OFF)
option(LIBIGL_PNG                 "Build target igl::png"                 OFF)
option(LIBIGL_PREDICATES          "Build target igl::predicates"          OFF)
option(LIBIGL_XML                 "Build target igl::xml"                 OFF)


option(LIBIGL_COPYLEFT_CGAL       "Build target igl_copyleft::cgal"       OFF)
option(LIBIGL_COPYLEFT_COMISO     "Build target igl_copyleft::comiso"     OFF)
option(LIBIGL_COPYLEFT_CORE       "Build target igl_copyleft::core"       ON)
option(LIBIGL_COPYLEFT_CORK       "Build target igl_copyleft::cork"       ON)
option(LIBIGL_COPYLEFT_TETGEN     "Build target igl_copyleft::tetgen"     ON)

if(TARGET igl::core)
    return()
endif()

include(FetchContent)


option(FETCHCONTENT_FULLY_DISCONNECTED   "Disables all attempts to download or update content and assumes source dirs already exist" ON )
option(FETCHCONTENT_UPDATES_DISCONNECTED "Enables UPDATE_DISCONNECTED behavior for all content population" ON)
option(FETCHCONTENT_QUIET                "Enables QUIET option for all content population" OFF )
set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libigl)

FetchContent_Declare(
    libigl
    GIT_REPOSITORY https://github.com/libigl/libigl.git
    GIT_TAG v2.5.0
)
FetchContent_MakeAvailable(libigl)
