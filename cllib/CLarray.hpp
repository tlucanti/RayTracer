
#ifndef CLLIB_GPUARRAY_HPP
# define CLLIB_GPUARRAY_HPP

# include <vector>

# include "defs.hpp"
# include "CLexception.hpp"
# include "CLcontext.hpp"
# include "CLqueue.hpp"

CLLIB_NAMESPACE_BEGIN

const cl_mem_flags read_write_array = CL_MEM_READ_WRITE;
const cl_mem_flags read_only_array = CL_MEM_READ_ONLY;
const cl_mem_flags write_only_array = CL_MEM_WRITE_ONLY;

template <class value_type, cl_mem_flags flag=read_write_array>
class CLarray /* : public __utils::__noncopymovable<value_type> */
{
public:
    CLarray(
        size_t size,
        const CLcontext &context,
        const CLqueue &queue,
        void *host_ptr=nullptr
    ) :
        buffer(), buff_size(size), ref_cnt(1)
    {
        static_assert(IS_ARITHMETIC(value_type), "array type can only be arithmetic");
        cl_int  error;

        buffer = clCreateBuffer(context.__get_context(), flag, size * sizeof(value_type), host_ptr, &error);
        if (error)
            throw CLexception(error);
        std::cout << "created array " << this << " (" << buffer << ")" << std::endl;
    }

    CLarray(
        const std::vector<value_type> &vec,
        const CLcontext &context,
        const CLqueue &queue,
        void *host_ptr=nullptr
    ) :
        buffer(), buff_size(vec.size()), ref_cnt(1)
    {
        static_assert(IS_ARITHMETIC(value_type), "array type can only be arithmetic");
        cl_int  error;

        clCreateImage2D()
        buffer = clCreateBuffer(context.__get_context(), flag, buff_size * sizeof(value_type), host_ptr, &error);
        if (error)
            throw CLexception(error);
        fill(vec, queue);
        std::cout << "created array " << this << " (" << buffer << ")" << std::endl;
    }

    ~CLarray() THROW
    {
        cl_int  error;

        --ref_cnt;
        if (ref_cnt == 0)
        {
            std::cout << "released array " << this << " (" << buffer << ')' << std::endl;
            error = clReleaseMemObject(buffer);
            if (error != CL_SUCCESS)
                throw CLexception(error);
        }
    }

    size_t size()
    {
        return buff_size;
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

        if (vec.size() > buff_size)
            throw std::runtime_error("vector size more than buffer size");
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

    std::vector<value_type> dump(
        const CLqueue &queue,
        bool block=true,
        size_t size=0,
        size_t offset=0,
        cl_uint num_events_in_wait_list=0,
        cl_event *event_wait_list=nullptr,
        cl_event *event=nullptr
    )
    {
        if (size == 0)
            size = buff_size - offset;
        std::vector<value_type> ret(size);
        dump(ret, queue, block, size, offset, num_events_in_wait_list, event_wait_list, event);
        return ret;
    }

    void dump(
        std::vector<value_type> &out,
        const CLqueue &queue,
        bool block=true,
        size_t size=0,
        size_t offset=0,
        cl_uint num_events_in_wait_list=0,
        cl_event *event_wait_list=nullptr,
        cl_event *event=nullptr
        )
    {
        cl_int  error;

        if (size == 0)
            size = out.size();
        if (size > buff_size)
            throw std::runtime_error("vector size more than buffer size");
        error = clEnqueueReadBuffer(
            queue.__get_queue(),
            buffer,
            static_cast<cl_bool>(block),
            offset,
            size * sizeof(value_type),
            out.data(),
            num_events_in_wait_list,
            event_wait_list,
            event
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

# ifdef CL_MEM_TYPE
    WUR UNUSED cl_mem_object_type get_mem_type() const
    {
        return static_cast<cl_mem_object_type>(_get_numeric_data(CL_MEM_TYPE, sizeof(cl_mem_object_type)));
    }
# endif

# ifdef CL_MEM_FLAGS
    WUR UNUSED cl_mem_flags get_mem_flags() const
    {
        return static_cast<cl_mem_flags>(_get_numeric_data(CL_MEM_FLAGS, sizeof(cl_mem_flags)));
    }
# endif

# ifdef CL_MEM_SIZE
    WUR UNUSED size_t get_mem_size() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_MEM_SIZE, sizeof(size_t)));
    }
# endif

# ifdef CL_MEM_HOST_PTR
    WUR UNUSED void *get_mem_host_ptr() const
    {
        return reinterpret_cast<void *>(_get_numeric_data(CL_MEM_HOST_PTR, sizeof(void *)));
    }
# endif

# ifdef CL_MEM_MAP_COUNT
    WUR UNUSED cl_uint get_mem_map_count() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_MEM_MAP_COUNT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_MEM_REFERENCE_COUNT
    WUR UNUSED cl_uint get_mem_reference_count() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_MEM_REFERENCE_COUNT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_MEM_CONTEXT
    WUR UNUSED cl_context get_mem_context() const
    {
        return static_cast<cl_context>(_get_numeric_data(CL_MEM_CONTEXT, sizeof(cl_context)));
    }
# endif

# ifdef CL_MEM_ASSOCIATED_MEMOBJECT
    WUR UNUSED cl_mem get_mem_associated_memobject() const
    {
        return static_cast<cl_mem>(_get_numeric_data(CL_MEM_ASSOCIATED_MEMOBJECT, sizeof(cl_mem)));
    }
# endif

# ifdef CL_MEM_OFFSET
    WUR UNUSED size_t get_mem_offset() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_MEM_OFFSET, sizeof(size_t)));
    }
# endif

    WUR const cl_mem &__get_buffer() const
    {
        return buffer;
    }

    CLarray()=delete;

private:

    CLarray(const CLarray &cpy)
        : buffer(cpy.buffer), ref_cnt(cpy.ref_cnt + 1)
    {}

    WUR unsigned long long _get_numeric_data(cl_mem_info type, size_t value_size) const
    {
        cl_int              error;
        unsigned long long  info;

        error = clGetMemObjectInfo(
                buffer,
                type,
                value_size,
                &info,
                nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return info;
    }

    cl_mem          buffer;
    size_t          buff_size;
    unsigned int    ref_cnt;

    friend class CLkernel;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_GPUARRAY_HPP */