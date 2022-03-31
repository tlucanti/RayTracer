/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GPUArray.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 20:05:46 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/09 23:37:38 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPUARRAY_HPP
# define GPUARRAY_HPP

# ifdef __APPLE__

#  include <OpenCL/opencl.h>

# else /* not __APPLE__ */
#  include <CL/cl.h>
# endif /* __APPLE__ */

# include <vector>
# include "GPUContext.hpp"
# include "GPUArrayException.hpp"

namespace tlucanti {
    template<char o>
    class GPUKernel;

    template<typename value_type, int flags = 'rw'>
    class GPUArray {
    public:
        GPUArray(std::vector<value_type> &data) {
            _init(data);
        }

        GPUArray(size_t vec_size) {
            _init(std::vector<value_type>(vec_size));
        }

        ~GPUArray() noexcept(false) {
            cl_int cl_errno;
            if ((cl_errno = clReleaseMemObject(_mem_obj)))
                throw GPUArrayException("gpuarray", "cannot destroy gpu array", cl_errno);
        }

        void dump(std::vector<int> &vec) const {
            vec.resize(_size);
            clEnqueueReadBuffer(GPUContext::command_queue, _mem_obj, CL_TRUE, 0,
                                _size * sizeof(int), vec.data(), 0, nullptr, nullptr);
        }

        __WUR inline std::size_t size() const { return _size; };

    private:
        void _init(const std::vector<value_type> &v) {
            cl_int cl_errno;
            cl_mem_flags cl_flags;
            if (flags == 'r')
                cl_flags = CL_MEM_READ_ONLY;
            else if (flags == 'w')
                cl_flags = CL_MEM_WRITE_ONLY;
            else if (flags == 'rw')
                cl_flags = CL_MEM_READ_WRITE;
            else
                throw GPUArrayException("gpuarray", "unknown array flag", flags);
            _size = v.size();
            _mem_obj = clCreateBuffer(GPUContext::context, cl_flags, _size * sizeof(cl_int), nullptr, &cl_errno);
            if (cl_errno)
                throw GPUArrayException("gpuarray", "cannot allocate gpu array", cl_errno);
            cl_errno = clEnqueueWriteBuffer(GPUContext::command_queue, _mem_obj,
                                            CL_TRUE, 0, _size * sizeof(value_type), v.data(), 0, nullptr, nullptr);
            if (cl_errno)
                throw GPUArrayException("gpuarray", "cannot initialize gpu array", cl_errno);
        }

    public: // remove this
        cl_mem _mem_obj;
        std::size_t _size;
    };
}

#endif /* GPUARRAY_HPP */
