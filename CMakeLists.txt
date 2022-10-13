cmake_minimum_required(VERSION 3.21)
project(RayTracer)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_FLAGS "-D__DEBUG -D__CMAKE -D__CPP -D__VERBOSE -DCL_TARGET_OPENCL_VERSION=210 -O0 -g3")
#set(CMAKE_CXX_FLAGS "-O0")
set(CMAKE_CXX_COMPILER "clang++")

if (APPLE)
	message("macos")
	include_directories(mlxlib/mlxosx)
	link_directories(mlxlib/mlxosx)
	find_library(OPENGL_LIB OpenGL)
	find_library(APPKIT_LIB AppKit)
	set(MLX_LIB mlx z pthread ${OPENGL_LIB} ${APPKIT_LIB})
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Wfloat-equal -Wshadow -Wcast-qual -Wconversion -Wzero-as-null-pointer-constant -Wextra-semi -Wsign-conversion")
else()
	message("linux")
	include_directories(mlxlib/mlxlinux)
	link_directories(mlxlib/mlxlinux)
	set(MLX_LIB mlx Xext X11 m z pthread)
#	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Wduplicated-branches -Wduplicated-cond -Wfloat-equal -Wshadow=compatible-local -Wcast-qual -Wconversion -Wzero-as-null-pointer-constant -Wextra-semi -Wsign-conversion -Wlogical-op")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wctor-dtor-privacy -Wnon-virtual-dtor -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Wfloat-equal -Wcast-qual -Wconversion -Wzero-as-null-pointer-constant -Wextra-semi -Wsign-conversion")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic-errors")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-zero-variadic-macro-arguments -Wno-gnu-anonymous-struct")
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
