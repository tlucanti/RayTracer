
INCLUDE			=	include

LIB_SRC_DIR		=	libsrc
LIB_JSON_DIR	=	json
LIB_MLX_DIR		=	mlxlib
MLX_LINUX_DIR	=	mlxlinux
MLX_OSX_DIR		=	mlxosx

CMAKE_ENV		= __ENV_CXX_OPT='-D__MAKE'


all:
	mkdir -p build
	${CMAKE_ENV} cmake -S . -B build
	${CMAKE_ENV} cmake --build build --target RayTracer
	mv build/RayTracer .

install:
	@printf		"[TAR]\t${LIB_SRC_DIR}/${LIB_JSON_DIR}.tar.gz \n"
	@tar		-xf ${LIB_SRC_DIR}/${LIB_JSON_DIR}.tar.gz
	@printf		"[TAR]\t${LIB_SRC_DIR}/${LIB_JSON_DIR}.tar.gz \n"
	@tar		-C ${LIB_MLX_DIR} -xf ${LIB_SRC_DIR}/${MLX_OSX_DIR}.tar.gz
	@printf		"[TAR]\t${LIB_SRC_DIR}/${MLX_OSX_DIR}.tar.gz \n"
	@tar		-C ${LIB_MLX_DIR} -xf ${LIB_SRC_DIR}/${MLX_LINUX_DIR}.tar.gz
	@printf		"[LN]\t${LIB_JSON_DIR}/single_include/nlohmann/json.hpp \n"
	@ln			-s ../${LIB_JSON_DIR}/single_include/nlohmann/json.hpp ${INCLUDE}/json

