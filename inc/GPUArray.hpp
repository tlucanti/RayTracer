/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GPUArray.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 20:05:46 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/08 20:49:25 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPUARRAY_HPP
# define GPUARRAY_HPP

#ifdef __APPLE__
# include <OpenCL/opencl.h>
#else /* not __APPLE__ */
# include <CL/cl.h>
#endif /* __APPLE__ */

# include <vector>
# include "GPUArrayException.hpp"

namespace tlucanti
{
	class GPUKernel;
	class GPUArray
	{
	public:
		typedef int value_type;
		GPUArray(std::vector<value_type> &data, const std::string_view &flag="rw");
		void dump(std::vector<int> &vec) const;
		__WUR std::size_t size() const;
		~GPUArray() noexcept(false);

		friend GPUKernel;

	private:
		cl_mem		_mem_obj;
		std::size_t _size;
	};
}

#endif /* GPUARRAY_HPP */
