
LIB_DIR = /usr/lib/x86_64-linux-gnu
OPENCL = OpenCL

all:
	g++ -Wall -Wextra -std=c++2a test.cpp -L${LIB_DIR} -l${OPENCL}

