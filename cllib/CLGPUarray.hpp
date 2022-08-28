
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
        const CLcontext &context,
        const std::vector<value_type> &vec,
        cl_mem_flags flag=read_write_array,
        void *host_ptr=nullptr
    ) :
        buffer()
    {
        static_assert(IS_INTEGRAL(value_type), "array type can only be integral");
        cl_int  error;

        buffer = clCreateBuffer(context.get_context(), flag, vec.size() * sizeof(value_type), host_ptr, &error);
        if (error)
            throw CLexception(error);
        fill(vec);
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

        buffer = clCreateBuffer(context.get_context(), flag, size * sizeof(value_type), host_ptr, &error);
        if (error)
            throw CLexception(error);
    }

    void fill(const CLqueue &queue, const std::vector<value_type> &vec, bool block, size_t offset=0, cl_uint event=0)
    {
        static_assert(IS_INTEGRAL(value_type), "array type can only be integral");
        cl_int  error;

        error = clEnqueueWriteBuffer(queue.get_queue(), buffer, static_cast<cl_bool>(block), offset, vec.size() * sizeof(value_type), vec.data(), )
    }

private:
    cl_mem  buffer;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_GPUARRAY_HPP */