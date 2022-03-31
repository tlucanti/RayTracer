cmake_minimum_required(VERSION 3.21)
project(RayTracer)

set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_FLAGS "-D__DEBUG -D__CMAKE -O0")
set(CMAKE_CXX_FLAGS "-I/usr/local/include/SDL2 -D_THREAD_SAFE")

include_directories(.)
include_directories(inc)
include_directories(/opt/X11/include)
link_directories(.)

add_executable(RayTracer
		src/GPUContext.cpp
		src/GPUKernel.cpp
		src/GPUArray.cpp

		src/window.cpp

		src/main.cpp
)

find_library(OPENCL_LIB OpenCL)
find_library(X11_LIB X11)

target_link_libraries(RayTracer
		${OPENCL_LIB}
		${X11_LIB}
)
