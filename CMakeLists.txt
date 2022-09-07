cmake_minimum_required(VERSION 3.21)
project(RayTracer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-D__DEBUG -D__CMAKE -DCL_TARGET_OPENCL_VERSION=210 -O0")
#set(CMAKE_CXX_FLAGS "-O0")
set(CMAKE_CXX_COMPILER "clang++")

include_directories(.)
include_directories(cllib)
include_directories(mlxlib)
include_directories(inc)
link_directories(.)
link_directories(cllib)
link_directories(mlxlib)

add_executable(old
		old/GPUContext.cpp
		old/GPUKernel.cpp
		old/GPUArray.cpp

#		old/window.cpp

		old/main.cpp
)

add_executable(test test.cpp)
add_executable(cllib cllib/main.cpp)
add_executable(mlxlib mlxlib/main.cpp)

add_executable(RayTracer src/main.cpp)

find_library(OPENCL_LIB OpenCL)
#find_library(MLX_LIB mlx)
set(MLX_LIB mlxlib/libmlx.so)
#find_library(X11_LIB X11)

message(${OPENCL_LIB})
message(${MLX_LIB})
target_link_libraries(RayTracer
#		${X11_LIB}
)

target_link_libraries(test ${OPENCL_LIB})
target_link_libraries(cllib ${OPENCL_LIB})
target_link_libraries(mlxlib mlx Xext X11 m z)

target_link_libraries(RayTracer ${OPENCL_LIB} mlx Xext X11 m z)
