/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GPUContext.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 23:16:11 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/09 23:31:23 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/GPUContext.hpp"
#include <GPUArrayException.hpp>
#include <iostream>

bool				tlucanti::GPUContext::init_done = false;
bool				tlucanti::GPUContext::destroy_done = false;
cl_platform_id		tlucanti::GPUContext::platform_id;
cl_device_id		tlucanti::GPUContext::dev_id;
cl_uint				tlucanti::GPUContext::dev_num;
cl_uint				tlucanti::GPUContext::platform_num;

cl_context			tlucanti::GPUContext::context;
cl_command_queue	tlucanti::GPUContext::command_queue;

void
tlucanti::GPUContext::init()
{
	if (init_done)
		return ;
	cl_int	cl_errno = 0;

	cl_errno = clGetPlatformIDs(1, &platform_id, &platform_num);
	if (cl_errno)
		throw GPUArrayException("init error", "cannot get platform id", cl_errno);
    std::cout << "platform_id: " << platform_id << ", platform_num: " << platform_num << std::endl;
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


void
tlucanti::GPUContext::destroy()
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
