cmake_minimum_required(VERSION 3.21)
project(RayTracer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-D__DEBUG -D__CMAKE -DCL_TARGET_OPENCL_VERSION=210 -O0")
#set(CMAKE_CXX_FLAGS "-O0")

include_directories(.)
include_directories(inc)
include_directories(/opt/X11/include)
link_directories(.)
link_directories(cllib)

add_executable(RayTracer
		src/GPUContext.cpp
		src/GPUKernel.cpp
		src/GPUArray.cpp

#		src/window.cpp

		src/main.cpp
)

add_executable(test test.cpp)

add_executable(cllib cllib/main.cpp cllib/CLGPUarray.hpp)

find_library(OPENCL_LIB OpenCL)
#find_library(X11_LIB X11)

message(${OPENCL_LIB})
target_link_libraries(RayTracer
		${OPENCL_LIB}
#		${X11_LIB}
)

target_link_libraries(test ${OPENCL_LIB})
