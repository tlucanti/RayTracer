
#ifndef CLLIB_GPUARRAY_HPP
# define CLLIB_GPUARRAY_HPP

# include <vector>

# include "defs.hpp"
# include "CLexception.hpp"
# include "CLcontext.hpp"
# include "CLqueue.hpp"

CLLIB_NAMESPACE_BEGIN

cl_mem_flags read_write_array = CL_MEM_READ_WRITE;
cl_mem_flags read_only_array = CL_MEM_READ_ONLY;
cl_mem_flags write_only_array = CL_MEM_WRITE_ONLY;

template <class value_type>
class CLarray
{
public:
    CLarray(
        size_t size,
        const CLcontext &context,
        const CLqueue &queue,
        cl_mem_flags flag=read_write_array,
        void *host_ptr=nullptr
    ) : CLarray(
            std::vector<value_type>(size),
            context,
            queue,
            flag,
            host_ptr
        )
    {}

    CLarray(
        const std::vector<value_type> &vec,
        const CLcontext &context,
        const CLqueue &queue,
        cl_mem_flags flag=read_write_array,
        void *host_ptr=nullptr
    ) :
        buffer()
    {
        static_assert(IS_ARITHMETIC(value_type), "array type can only be integral");
        cl_int  error;

        buffer = clCreateBuffer(context.__get_context(), flag, vec.size() * sizeof(value_type), host_ptr, &error);
        if (error)
            throw CLexception(error);
        fill(vec, queue);
    }

    CLarray(
        const CLcontext &context,
        size_t size,
        cl_mem_flags flag=read_write_array,
        void *host_ptr=nullptr
    ) :
        buffer()
    {
        static_assert(IS_INTEGRAL(value_type), "array type can only be integral");
        cl_int  error;

        buffer = clCreateBuffer(context.__get_context(), flag, size * sizeof(value_type), host_ptr, &error);
        if (error)
            throw CLexception(error);
    }

    void fill(
        const std::vector<value_type> &vec,
        const CLqueue &queue,
        bool block=true,
        size_t offset=0,
        cl_uint num_events_in_wait_list=0,
        const cl_event *event_wait_list=nullptr,
        cl_event *event=nullptr
    )
    {
        static_assert(IS_ARITHMETIC(value_type), "array type can only be integral");
        cl_int  error;

        error = clEnqueueWriteBuffer(
            queue.__get_queue(),
            buffer,
            static_cast<cl_bool>(block),
            offset,
            vec.size() * sizeof(value_type),
            vec.data(),
            num_events_in_wait_list,
            event_wait_list,
            event
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    WUR const cl_mem &__get_buffer() const
    {
        return buffer;
    }

private:
    cl_mem  buffer;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_GPUARRAY_HPP */