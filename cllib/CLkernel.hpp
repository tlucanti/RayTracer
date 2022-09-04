
#ifndef CLLIB_KERNEL_HPP
# define CLLIB_KERNEL_HPP

# include <string>
# include <utility>

# include "defs.hpp"
# include "CLexception.hpp"
# include "CLprogram.hpp"
# include "CLarray.hpp"

CLLIB_NAMESPACE_BEGIN

class CLkernel : public __utils::__noncopymovable<>
{
public:
    CLkernel(
        const CLprogram &program,
        size_t *global_work_size,
        cl_uint dimentions=1,
        size_t *local_work_size=nullptr,
        size_t *global_work_offset=nullptr
    ) :
        kernel(),
        argc(program.__get_argc()),
        work_dim(1),
        global_work_offset(global_work_offset),
        global_work_size(global_work_size),
        local_work_size(local_work_size)
    {
        cl_int  error;

        kernel = clCreateKernel(program.__get_program(), program.get_program_name().c_str(), &error);
        if (error)
            throw CLexception(error);
    }

    ~CLkernel() THROW
    {
        cl_int  error;

        error = clReleaseKernel(kernel);
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    void set_work_dimentios(cl_uint dim)
    {
        work_dim = dim;
    }

    void set_global_work_offset(size_t *offset)
    {
        global_work_offset = offset;
    }

    void set_global_work_size(size_t *size)
    {
        global_work_size = size;
    }

    void set_local_work_size(size_t *size)
    {
        local_work_size = size;
    }

    template <class type>
    void set_arg(int argi, const CLarray<type> &array)
    {
        cl_int  error;

        error = clSetKernelArg(kernel, argi, sizeof(cl_mem), &array.__get_buffer());
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    template <class type>
    void set_arg(unsigned int argi, const type &arg)
    {
        cl_int  error;

        error = clSetKernelArg(kernel, argi, sizeof(type), &arg);
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }
//    template <class... type>
//    void set_args(const type &...arg)
//    {
//        if (argc != sizeof...(arg))
//            throw std::runtime_error("argument count mismatch with kernel");
//        _set_args_req(std::tuple<type...>(arg...));
//    }

    void run(
        const CLqueue &queue,
        cl_uint num_events_in_wait_list=0,
        cl_event *event_wait_list=nullptr,
        cl_event *event=nullptr
    )
    {
        cl_int  error;

        error = clEnqueueNDRangeKernel(
            queue.__get_queue(),
            kernel,
            work_dim,
            global_work_offset,
            global_work_size,
            local_work_size,
            num_events_in_wait_list,
            event_wait_list,
            event
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    WUR const cl_kernel &__get_kernel() const
    {
        return kernel;
    }

    CLkernel()=delete;

private:
//    template <size_t argi=0, class ...type>
//    typename std::enable_if<argi == sizeof...(type), void>::type
//    _set_args_req(const std::tuple<type...> &arg)
//    { return {}; }
//
//    template <size_t argi=0, class ...type>
//    typename std::enable_if<argi < sizeof...(type), void>::type
//    _set_args_req(const std::tuple<type...> &arg)
//    {
//        cl_int  error;
//
//        error = clSetKernelArg(kernel, argi, sizeof(cl_mem), &arg.__get_buffer());
//        if (error != CL_SUCCESS)
//            throw CLexception(error);
//        _set_args_req<argi + 1, type...>(arg);
//    }

//    template <class... type>
//    void _set_args_rec(const CLarray<type> &...arg)
//    {
//        cl_int  error;
//        int     i=0;
//
//        for (const auto &a : {arg...})
//        {
//            error = clSetKernelArg(kernel, i++, sizeof(cl_mem), &a.__get_buffer());
//            if (error != CL_SUCCESS)
//                throw CLexception(error);
//        }
//    }

    cl_kernel   kernel;
    int         argc;

    cl_uint     work_dim;
    size_t      *global_work_offset;
    size_t      *global_work_size;
    size_t      *local_work_size;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_KERNEL_HPP */