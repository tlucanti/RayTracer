/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GPUKernel.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 20:05:54 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/08 20:51:58 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPUKERNEL_HPP
# define GPUKERNEL_HPP

#ifdef __APPLE__
# include <OpenCL/opencl.h>
#else /* not __APPLE__ */
# include <CL/cl.h>
#endif /* __APPLE__ */

# include "GPUArrayException.hpp"
//# include "GPUArray.hpp"

namespace tlucanti
{
	class GPUArray;

	class GPUKernel
	{
	public:
		GPUKernel(const std::string &program_code);
		void set_operands(const GPUArray &first, const GPUArray &second, const GPUArray &result);
		void do_operation() const;
		~GPUKernel() noexcept(false);
		static void init();

		std::size_t 	size;

		// static members
		static bool 				init_done;
		static cl_platform_id		platform_id;
		static cl_device_id			dev_id;
		static cl_uint				dev_num;
		static cl_uint				platform_num;

		static cl_context			context;
		static cl_command_queue		command_queue;

		static cl_program			program;
		static cl_kernel			kernel;
	};
}

#endif /* GPUKERNEL_HPP */
