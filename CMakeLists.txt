cmake_minimum_required(VERSION 3.21)
project(RayTracer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_FLAGS "-O0 -g3 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__DEBUG $ENV{__ENV_CXX_OPT} -D__CPP -D__VERBOSE -DRTX_RAY_MARCHER")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCL_TARGET_OPENCL_VERSION=210")

#set(CMAKE_CXX_FLAGS "-O0")
set(CMAKE_CXX_COMPILER "clang++")

if (APPLE)
	message("macos")
	include_directories(mlxlib/mlxosx)
	link_directories(mlxlib/mlxosx)
	find_library(OPENGL_LIB OpenGL)
	find_library(APPKIT_LIB AppKit)
	set(MLX_LIB mlx z pthread ${OPENGL_LIB} ${APPKIT_LIB})
else()
	message("linux")
	include_directories(mlxlib/mlxlinux)
	link_directories(mlxlib/mlxlinux)
	set(MLX_LIB mlx Xext X11 m z pthread)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__DEBUG")

include_directories(.)
include_directories(include)
include_directories(cllib)
include_directories(cllib/include)
include_directories(mlxlib)
include_directories(cl)

link_directories(.)
link_directories(cllib)

add_executable(test test.cpp)
add_executable(cllib cllib/main.cpp)
add_executable(mlxlib mlxlib/main.cpp)

add_executable(RayTracer
	src/main.cpp

	cllib/src/CLbuiltinprog.cpp
	cllib/src/CLcontext.cpp
	cllib/src/CLdevice.cpp
	cllib/src/CLexception.cpp
	cllib/src/CLkernel.cpp

	src/event_hooks.cpp
	src/global.cpp
	src/init_gpu.cpp
	src/init_mlx.cpp
	src/init_scene.cpp
	src/parse_scene.cpp
	src/linalg.cpp
	src/objects.cpp
)

find_library(OPENCL_LIB OpenCL)
#find_library(X11_LIB X11)

message(${OPENCL_LIB})
message(${MLX_LIB})
target_link_libraries(RayTracer
#		${X11_LIB}
)

target_link_libraries(test ${OPENCL_LIB})
target_link_libraries(cllib ${OPENCL_LIB})
target_link_libraries(mlxlib mlx Xext X11 m z)

target_link_libraries(RayTracer ${OPENCL_LIB} ${MLX_LIB})
