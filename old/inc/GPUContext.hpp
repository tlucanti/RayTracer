/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GPUContext.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 23:21:40 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/09 23:31:26 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPUCONTEXT_HPP
# define GPUCONTEXT_HPP

# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else /* not __APPLE__ */
#  include <CL/cl.h>
# endif /* __APPLE__ */


namespace tlucanti
{
	class GPUContext
	{
	public:
		GPUContext() = delete;

	// private:
		static void init();
		static void destroy();

		static bool					init_done;
		static bool					destroy_done;

		static cl_platform_id		platform_id;
		static cl_device_id			dev_id;
		static cl_uint				dev_num;
		static cl_uint				platform_num;

		static cl_context			context;
		static cl_command_queue		command_queue;
	};
}

#endif /* GPUCONTEXT_HPP */
