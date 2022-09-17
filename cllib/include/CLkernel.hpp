
#ifndef CLLIB_KERNEL_HPP
# define CLLIB_KERNEL_HPP

# include "CLcommon.hpp"
# include "CLexception.hpp"
# include "CLprogram.hpp"
# include "CLarray.hpp"

# include <string>
# include <utility>

CLLIB_NAMESPACE_BEGIN

class CLkernel : public __utils::__noncopymovable<>
{
public:
    CLkernel(
        const CLprogram &program,
        size_t global_work_size,
        size_t local_work_size=0,
        size_t global_work_offset=0
    ) :
        kernel(),
        argc(program.__get_argc()),
        argi(0)
    {
        cl_int  error;

        set_global_work_size(global_work_size);
        set_local_work_size(local_work_size);
        set_global_work_offset(global_work_offset);

        kernel = clCreateKernel(program.__get_program(), program.get_program_name().c_str(), &error);
        if (error)
            throw CLexception(error);
    }

    CLkernel(
        const CLprogram &program,
        const std::vector<size_t> &global_work_size,
        const std::vector<size_t> &local_work_size=std::vector<size_t>(),
        const std::vector<size_t> &global_work_offset=std::vector<size_t>()
    ) :
        kernel(),
        argc(program.__get_argc()),
        argi(0),
        local_work_size_ptr(local_work_size.data()),
        global_work_offset_ptr(global_work_offset.data())
    {
        cl_int  error;

        set_global_work_size(global_work_size);
        set_local_work_size(local_work_size);
        set_global_work_offset(global_work_offset);

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

    void set_global_work_size(const std::vector<size_t> &global_work_size)
    {
        global_work_size_vector = global_work_size;
    }

    void set_global_work_size(size_t size)
    {
        global_work_size_vector = std::vector<size_t>(1, size);
    }

    void set_local_work_size(const std::vector<size_t> &local_work_size)
    {
        if (local_work_size.empty())
        {
            local_work_size_ptr = nullptr;
            return ;
        }
        if (local_work_size.size() != global_work_size_vector.size())
            throw std::runtime_error("local_work_size and global_work_size dimensions mismatch");
        local_work_size_vector = local_work_size;
        local_work_size_ptr = local_work_size_vector.data();
    }

    void set_local_work_size(size_t size)
    {
        if (size == 0)
        {
            local_work_size_ptr = nullptr;
            return ;
        }
        if (global_work_size_vector.size() != 1)
            throw std::runtime_error("for multidimensional kernel offset vector expected");
        local_work_size_vector = std::vector<size_t>(1, size);
        local_work_size_ptr = local_work_size_vector.data();
    }

    void set_global_work_offset(const std::vector<size_t> &global_work_offset)
    {
        if (global_work_offset.empty())
        {
            global_work_offset_ptr = nullptr;
            return ;
        }
        if (global_work_offset.size() != global_work_size_vector.size())
            throw std::runtime_error("global_work_offset and global_work_size dimensions mismatch");
        global_work_offset_vector = global_work_offset;
        global_work_offset_ptr = global_work_offset_vector.data();
    }

    void set_global_work_offset(size_t size)
    {
        if (size == 0)
        {
            global_work_offset_ptr = nullptr;
            return ;
        }
        if (global_work_size_vector.size() != 1)
            throw std::runtime_error("for multidimensional kernel offset vector expected");
        global_work_offset_vector = std::vector<size_t>(1, size);
        global_work_offset_ptr = global_work_offset_vector.data();
    }

    void reset_args()
    {
        argi = 0;
    }

    template <class type, cl_mem_flags flag>
    void set_next_arg(const CLarray<type, flag> &array)
    {
        cl_int  error;

        error = clSetKernelArg(kernel, argi++, sizeof(cl_mem), &array.data());
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    template <class type>
    void set_next_arg(const type &arg)
    {
        cl_int  error;

        error = clSetKernelArg(kernel, argi++, sizeof(type), &arg);
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
        bool block=true,
        cl_uint num_events_in_wait_list=0,
        cl_event *event_wait_list=nullptr,
        cl_event *event=nullptr
    )
    {
        cl_int  error;

        error = clEnqueueNDRangeKernel(
            queue.__get_queue(),
            kernel,
            global_work_size_vector.size(),
            global_work_offset_ptr,
            global_work_size_vector.data(),
            local_work_size_ptr,
            num_events_in_wait_list,
            event_wait_list,
            event
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);
        if (block)
            queue.finish();
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
    int         argi;

    std::vector<size_t>     global_work_size_vector;
    std::vector<size_t>     local_work_size_vector;
    std::vector<size_t>     global_work_offset_vector;

    const size_t            *local_work_size_ptr;
    const size_t            *global_work_offset_ptr;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_KERNEL_HPP */