/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GPUKernel.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 19:53:06 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/09 21:33:07 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/GPUKernel.hpp"
#include "../inc/GPUArray.hpp"
#include "../inc/File.hpp"

bool 				tlucanti::GPUKernel::init_done = false;
bool 				tlucanti::GPUKernel::destroy_done = false;

cl_platform_id		tlucanti::GPUKernel::platform_id;
cl_device_id		tlucanti::GPUKernel::dev_id;
cl_uint				tlucanti::GPUKernel::dev_num;
cl_uint				tlucanti::GPUKernel::platform_num;

cl_context			tlucanti::GPUKernel::context;
cl_command_queue	tlucanti::GPUKernel::command_queue;

tlucanti::GPUKernel::GPUKernel(char operation)
		: size(0)
{
	std::string code;
	const char *func;
	switch (operation)
	{
		case '+':
			code = File("../cl/add.cl").text();
			func = "add";
			break;
		case '-':
			code = File("../cl/sub.cl").text();
			func = "sub";
			break;
		case '*':
			code = File("../cl/mul.cl").text();
			func = "mul";
			break;
		case '/':
			code = File("../cl/div.cl").text();
			func = "div";
			break;
		default:
			throw GPUArrayException("kernel", "unknown kernel opertaoion", operation);
	}
	init();
	constructor(code.c_str(), code.size(), func	);	
}

tlucanti::GPUKernel::GPUKernel(const std::string &program_code, const std::string &func)
		: size(0)
{
	init();
	constructor(program_code.c_str(), program_code.size(), func.c_str());
}

void
tlucanti::GPUKernel::constructor(const char *program_code, size_t program_size, const char *func)
{
	cl_int	cl_errno;

	size = program_size;
	program = clCreateProgramWithSource(context, 1, &program_code, &size, &cl_errno);
	if (cl_errno)
		throw GPUArrayException("kernel init", "cannot create program", cl_errno);
	if ((cl_errno = clBuildProgram(program, 1, &dev_id, nullptr, nullptr, nullptr)))
		throw GPUArrayException("kernel init", "cannot build program", cl_errno);
	kernel = clCreateKernel(program, func, &cl_errno);
	if (cl_errno)
		throw GPUArrayException("kernel init", "cannot create kernel", cl_errno);
}

void
tlucanti::GPUKernel::set_operands(const GPUArray &first, const GPUArray &second, const GPUArray &result)
{
	cl_int cl_errno;

	if (first.size() != second.size() or second.size() != result.size())
		throw GPUArrayException("kernel", "array size mismatch");
	if ((cl_errno = clSetKernelArg(kernel, 0, sizeof(cl_mem), &first._mem_obj)))
		throw GPUArrayException("kernel", "cannot set first sum argument", cl_errno);
	if ((cl_errno = clSetKernelArg(kernel, 1, sizeof(cl_mem), &second._mem_obj)))
		throw GPUArrayException("kernel", "cannot set second sum argument", cl_errno);
	if ((cl_errno = clSetKernelArg(kernel, 2, sizeof(cl_mem), &result._mem_obj)))
		throw GPUArrayException("kernel", "cannot set sum result argument", cl_errno);
	size = result.size();
}

void
tlucanti::GPUKernel::do_operation() const
{
	const size_t	&range = size;
	size_t			work_size;
	cl_int 			cl_errno;

	work_size = 1;
	cl_errno = clEnqueueNDRangeKernel(command_queue, kernel, 1,
		nullptr, &range, &work_size, 0, nullptr, nullptr);
	if (cl_errno)
		throw GPUArrayException("gpuarray", "cannot do gpu operation", cl_errno);
}

tlucanti::GPUKernel::~GPUKernel() noexcept(false)
{
	cl_int cl_errno;
	if ((cl_errno = clReleaseKernel(kernel)))
		throw GPUArrayException("gpuarray", "cannot destroy kernel", cl_errno);
	if ((cl_errno = clReleaseProgram(program)))
		throw GPUArrayException("gpuarray", "cannot destroy kernel program", cl_errno);
}

void
tlucanti::GPUKernel::destroy()
{
	if (destroy_done)
		return ;
	cl_int cl_errno;
	if ((cl_errno = clFlush(command_queue)))
		throw GPUArrayException("gpuarray", "cannot flush command queue", cl_errno);
	if ((cl_errno = clFinish(command_queue)))
		throw GPUArrayException("gpuarray", "cannot finish", cl_errno);
	if ((cl_errno = clReleaseCommandQueue(command_queue)))
		throw GPUArrayException("gpuarray", "cannot destroy command queue", cl_errno);
	if ((cl_errno = clReleaseContext(context)))
		throw GPUArrayException("gpuarray", "cannot destroy kernel context", cl_errno);
	destroy_done = true;
}

void
tlucanti::GPUKernel::init()
{
	if (init_done)
		return ;
	cl_int	cl_errno;
	cl_errno = 0;
	cl_errno = clGetPlatformIDs(1, &platform_id, &platform_num);
	if (cl_errno)
		throw GPUArrayException("init error", "cannot get platform id", cl_errno);
	cl_errno = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &dev_id, &dev_num);
	if (cl_errno)
		throw GPUArrayException("init error", "cannot get devices id", cl_errno);

	context = clCreateContext(nullptr, 1, &dev_id, nullptr, nullptr, &cl_errno);
	if (cl_errno)
		throw GPUArrayException("init error", "cannot create context", cl_errno);
	command_queue = clCreateCommandQueue(context, dev_id, 0, &cl_errno);
	if (cl_errno)
		throw GPUArrayException("init error", "cannot create command queue", cl_errno);
	init_done = true;
}
