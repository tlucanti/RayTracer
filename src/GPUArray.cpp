/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GPUArray.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 20:48:14 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/08 20:50:05 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/GPUArray.hpp"
#include "../inc/GPUKernel.hpp"

tlucanti::GPUArray::GPUArray(std::vector<value_type> &data, const std::string_view &flag)
{
	cl_int cl_errno;
	cl_mem_flags cl_flags;
	if (flag == "r")
		cl_flags = CL_MEM_READ_ONLY;
	else if (flag == "w")
		cl_flags = CL_MEM_WRITE_ONLY;
	else if (flag == "rw")
		cl_flags = CL_MEM_READ_WRITE;
	else
		throw GPUArrayException("gpuarray", "unknown array flag", flag);
	_size = data.size();
	_mem_obj = clCreateBuffer(GPUKernel::context, cl_flags, _size * sizeof(cl_int), nullptr, &cl_errno);
	if (cl_errno)
		throw GPUArrayException("gpuarray", "cannot allocate gpu array", cl_errno);
	cl_errno = clEnqueueWriteBuffer(GPUKernel::command_queue, _mem_obj,
		CL_TRUE, 0, _size * sizeof(value_type), data.data(), 0, nullptr, nullptr);
	if (cl_errno)
		throw GPUArrayException("gpuarray", "cannot initialize gpu array", cl_errno);
}

void
tlucanti::GPUArray::dump(std::vector<int> &vec) const
{
	vec.resize(_size);
	clEnqueueReadBuffer(GPUKernel::command_queue, _mem_obj, CL_TRUE, 0,
		_size * sizeof(int), vec.data(), 0, nullptr, nullptr);
}

__WUR
std::size_t
tlucanti::GPUArray::size() const
{
	return _size;
}

tlucanti::GPUArray::~GPUArray() noexcept(false)
{
	cl_int cl_errno;
	if ((cl_errno = clReleaseMemObject(_mem_obj)))
		throw GPUArrayException("gpuarray", "cannot destroy gpu array", cl_errno);
}
