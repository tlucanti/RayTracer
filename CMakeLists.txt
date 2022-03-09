cmake_minimum_required(VERSION 3.21)
project(RayTracer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-D__DEBUG -D__CMAKE -Wall -Wextra -Werror -O0")

include_directories(.)
link_directories(.)

add_executable(RayTracer
		inc/GPUArray.hpp
		inc/defs.h
		inc/File.hpp
		inc/GPUKernel.hpp
		inc/GPUArrayException.hpp
		inc/Exception.hpp

		src/GPUKernel.cpp
		src/GPUArray.cpp
		src/main.cpp
)

find_library(OPENCL_LIB OpenCL)

target_link_libraries(RayTracer
		${OPENCL_LIB}
)
