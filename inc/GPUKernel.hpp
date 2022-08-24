/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GPUKernel.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlucanti <tlucanti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/08 20:05:54 by tlucanti          #+#    #+#             */
/*   Updated: 2022/03/09 23:36:41 by tlucanti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GPUKERNEL_HPP
# define GPUKERNEL_HPP

# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else /* not __APPLE__ */
#  include <CL/cl.h>
# endif /* __APPLE__ */

# include <GPUArrayException.hpp>
# include <File.hpp>
# include <GPUContext.hpp>

namespace tlucanti
{
    template<char operation>
    class GPUKernel
            {
    public:
        GPUKernel()
                : size(0) {
            static_assert(operation == '+' or operation == '-' or
                          operation == '*' or operation == '/',
                          "unknown kernel operation, available operations are: '+', '-', '*', '/");

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
                    break;
            }
            _init(code.c_str(), code.size(), func);
        }

        GPUKernel(const std::string &program_code, const std::string &func)
                : size(0) {
            _init(program_code.c_str(), program_code.size(), func.c_str());
        }

        ~GPUKernel() noexcept(false) {
            cl_int cl_errno;
            if ((cl_errno = clReleaseKernel(kernel)))
                throw GPUArrayException("gpuarray", "cannot destroy kernel", cl_errno);
            if ((cl_errno = clReleaseProgram(program)))
                throw GPUArrayException("gpuarray", "cannot destroy kernel program", cl_errno);
        }


        template <class GPUArrayIn1, class GPUArrayIn2, class GPUarrayOut>
        void set_operands(const GPUArrayIn1 &first, const GPUArrayIn2 &second, const GPUarrayOut &result) {
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

        void do_operation() const {
            const size_t &range = size;
            size_t work_size;
            cl_int cl_errno;

            work_size = 1;
            cl_errno = clEnqueueNDRangeKernel(GPUContext::command_queue, kernel, 1,
                                              nullptr, &range, &work_size, 0, nullptr, nullptr);
            if (cl_errno)
                throw GPUArrayException("gpuarray", "cannot do gpu operation", cl_errno);
        }


    private:
        void _init(const char *program_code, size_t program_size, const char *func)
        {
            cl_int cl_errno;

            GPUContext::init();
            size = program_size;
            program = clCreateProgramWithSource(GPUContext::context, 1, &program_code, &size, &cl_errno);
            if (cl_errno)
                throw GPUArrayException("kernel init", "cannot create program", cl_errno);
            if ((cl_errno = clBuildProgram(program, 1, &GPUContext::dev_id, nullptr, nullptr, nullptr)))
                throw GPUArrayException("kernel init", "cannot build program", cl_errno);
            kernel = clCreateKernel(program, func, &cl_errno);
            if (cl_errno)
                throw GPUArrayException("kernel init", "cannot create kernel", cl_errno);
        }


        std::size_t size;

        cl_program program;
        cl_kernel kernel;
    };
}

#endif /* GPUKERNEL_HPP */
