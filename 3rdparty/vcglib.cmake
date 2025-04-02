
set(EIGEN3_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcglib/eigenlib)
set(LIBIGL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcglib/wrap)

add_subdirectory(vcglib)
set(VCGDIR vcglib)
message(STATUS "- VCGLib - using custom VCGDIR path library")