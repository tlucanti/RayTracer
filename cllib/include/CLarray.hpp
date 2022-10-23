
#ifndef CLLIB_CLARRAY_HPP
# define CLLIB_CLARRAY_HPP

# include "CLcommon.hpp"
# include "CLexception.hpp"
# include "CLcontext.hpp"
# include "CLqueue.hpp"

# include <vector>

CLLIB_NAMESPACE_BEGIN

const cl_mem_flags read_write_array = CL_MEM_READ_WRITE;
const cl_mem_flags read_only_array = CL_MEM_READ_ONLY;
const cl_mem_flags write_only_array = CL_MEM_WRITE_ONLY;

// =============================================================================
// ---------------------------------- CLarray ----------------------------------
template <class value_type, cl_mem_flags flag=read_write_array>
class CLarray : public __utils::__noncopyble<value_type>
{
public:
// -----------------------------------------------------------------------------
    CLarray<value_type>(
        size_t size,
        const CLcontext &context,
        void *host_ptr=nullptr
    ) THROW
     : buffer(), buff_size(size)
    /**
        \brief sized constructor
        \details constructor allocates empty block of GPU memory of size `size`

        \param size size of allocating block
        \param context OpenCL context where buffer will be visible
        \param host_ptr

        \time O(1)
        \memory O(1)

        \GPUtime O(1)
        \GPUmemory O(size)
    */
    {
        static_assert(flag == read_write_array or flag == read_only_array
            or flag == write_only_array, "invalid array flag, valid values are "
            "read_write_array, read_only_array, write_only_array");

        cl_int  error = CL_SUCCESS;

//        if (size == 0)
//            buffer = nullptr;
//        else
            buffer = clCreateBuffer(
                context.__get_context(),
                flag,
                size * sizeof(value_type),
                host_ptr,
                &error
            );
        if (error != CL_SUCCESS)
            throw CLexception(error);
        if (__VERBOSE)
            std::cout << "created array " << this
                << " (" << buffer << ")" << std::endl;
    }

// -----------------------------------------------------------------------------
    CLarray(
        const std::vector<value_type> &vec,
        const CLcontext &context,
        const CLqueue &queue,
        bool block=true,
        void *host_ptr=nullptr
    ) THROW
     : buffer(), buff_size(vec.size())
    /**
        \brief valued constructor
        \details constructor allocaes block GPU memory and fills it with
            contents of `vec`

        \param vec vector with data to be filled
        \param context OpenCL context where buffer will be visible
        \param queue OpenCL queue where filling operation will be enqueued
        \param block blocking operation flag, if set to true, function will
            block until all data will be filled
        \param host_ptr

        \time O(1)/O(vec.size())
        \memory O(1)

        \GPUtime O(vec.size())
        \GPUmemory O(vec.size())
    */
    {
        static_assert(flag == read_write_array or flag == read_only_array
            or flag == write_only_array, "invalid array flag, valid values are "
            "read_write_array, read_only_array, write_only_array");

        cl_int  error = CL_SUCCESS;

        if (buff_size == 0)
            buffer = nullptr;
        else
            buffer = clCreateBuffer(
                context.__get_context(),
                flag,
                buff_size * sizeof(value_type),
                host_ptr,
                &error
            );
        if (error != CL_SUCCESS)
            throw CLexception(error);
        fill(vec, queue, block);
        if (__VERBOSE)
            std::cout << "created array " << this
                << " (" << buffer << ")" << std::endl;
    }

// -----------------------------------------------------------------------------
    ~CLarray() THROW
    /**
        \brief destructor
        \detailed destructor deallocates block of GPU memory (if no other
            CLarray is referencing it)

        \time O(1)
        \memory O(1)

        \GPUtime O(1)
        \GPUmemory O(1)
    */
    {
        _destroy();
    }

// -----------------------------------------------------------------------------
    WUR size_t size() const NOEXCEPT
    /**
        \brief size
        \detailed return size of allocated GPU memory block
    */
    {
        return buff_size;
    }

// -----------------------------------------------------------------------------
    void memset(
        const value_type &val,
        const CLqueue &queue,
        size_t offset=0,
        cl_uint num_events_in_wait_list=0,
        const cl_event *event_wait_list=nullptr,
        cl_event *event=nullptr
    ) THROW
        /**
        \brief fill GPU buffer with set value
        \detailed function fills GPU memory block, with values of `val`,
            overriding old data.

        \param val value to be filled with
        \param queue OpenCL queue where filling operation will be enqueued
        \param offset the offset in bytes in the buffer object to read from
        \param event_wait_list that need to be completed before executing this
            command
        \param num_events_in_wait_list number of events in `event_wait_list`
        \param event returns event id of current event, that can be used in the
            future to identify it

        \time O(this->size())
        \memory O(1)

        \GPUtime O(this->size())
        \GPUmemory O(1)
    */
    {
        cl_int  error = CL_SUCCESS;

        if (offset > buff_size)
            throw std::runtime_error("vector size more than buffer size");
        error = clEnqueueFillBuffer(
                queue.__get_queue(),
                buffer,
                &val,
                sizeof(value_type),
                offset,
                buff_size * sizeof(value_type),
                num_events_in_wait_list,
                event_wait_list,
                event
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

// -----------------------------------------------------------------------------
    void fill(
        const std::vector<value_type> &vec,
        const CLqueue &queue,
        bool block=true,
        size_t offset=0,
        cl_uint num_events_in_wait_list=0,
        const cl_event *event_wait_list=nullptr,
        cl_event *event=nullptr
    ) THROW
    /**
        \brief fill GPU buffer
        \detailed function fills GPU memory block with new data from `vec`,
            overriding old data.

        \param vec vector with data
        \param queue OpenCL queue where filling operation will be enqueued
        \param block blocking operation flag, if set to true, function will
            block until all data will be filled
        \param offset the offset in bytes in the buffer object to read from
        \param event_wait_list that need to be completed before executing this
            command
        \param num_events_in_wait_list number of events in `event_wait_list`
        \param event returns event id of current event, that can be used in the
            future to identify it

        \time O(vec.size())
        \memory O(1)

        \GPUtime O(vec.size())
        \GPUmemory O(1)
    */
    {
        cl_int  error = CL_SUCCESS;

        if (vec.size() - offset > buff_size)
            throw std::runtime_error("vector size more than buffer size");
        if (vec.empty())
            return ;
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

// -----------------------------------------------------------------------------
    std::vector<value_type> dump(
        const CLqueue &queue,
        bool block=true,
        size_t size=0,
        size_t offset=0,
        cl_uint num_events_in_wait_list=0,
        cl_event *event_wait_list=nullptr,
        cl_event *event=nullptr
    ) const THROW
    /**
        \brief dump GPU buffer
        \detailed move data from GPU memory block to CPU
        \param queue OpenCL queue where dumping operation will be enqueued
        \param block blocking operation flag, if set to true, function will
            block until all data will be filled
        \param size number of bytes to be dumped (size of whole array by
            default)
        \param offset offset of bytes in GPU memory block to read from
        \param event_wait_list that need to be completed before executing this
            command
        \param num_events_in_wait_list number of events in `event_wait_list`
        \param event returns event id of current event, that can be used in the
            future to identify it

        \time O(size)
        \memory O(size)

        \GPUtime O(size)
        \GPUmemory O(1)
    */
    {
        if (size == 0)
            size = buff_size - offset;
        std::vector<value_type> ret(size);
        dump(
            ret,
            queue,
            block,
            size,
            offset,
            num_events_in_wait_list,
            event_wait_list,
            event
        );
        return ret;
    }

// -----------------------------------------------------------------------------
    void dump(
        std::vector<value_type> &out,
        const CLqueue &queue,
        bool block=true,
        size_t size=0,
        size_t offset=0,
        cl_uint num_events_in_wait_list=0,
        cl_event *event_wait_list=nullptr,
        cl_event *event=nullptr
    ) const THROW
    /**
        \brief dump GPU buffer
        \detailed same as previous `dump` method, except dumping in provided
            array `out`, without creating new vector
    */
    {
        dump(
            out.data(),
            queue,
            block,
            size,
            offset,
            num_events_in_wait_list,
            event_wait_list,
            event
        );
    }

// -----------------------------------------------------------------------------
    void dump(
        value_type *out,
        const CLqueue &queue,
        bool block=true,
        size_t size=0,
        size_t offset=0,
        cl_uint num_events_in_wait_list=0,
        cl_event *event_wait_list=nullptr,
        cl_event *event=nullptr
    ) const THROW
    /**
        \brief dump GPU buffer
        \detailed same as previos `dump` method, except dumping to provided
            pointer `out`, rather than vector
    */
    {
        cl_int  error;

        if (size == 0)
            size = buff_size;
        if (size > buff_size)
            throw std::runtime_error("vector size more than buffer size");
        error = clEnqueueReadBuffer(
            queue.__get_queue(),
            buffer,
            static_cast<cl_bool>(block),
            offset,
            size * sizeof(value_type),
            out,
            num_events_in_wait_list,
            event_wait_list,
            event
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

// -----------------------------------------------------------------------------
    WUR const cl_mem &data() const NOEXCEPT
    /**
        \brief cl_mem object
        \detailed returns raw cl_mem pointer of this CLarray

        \time O(1)
        \memory O(1)

        \GPUtime O(1)
        \GPUmemory O(1)
    */
    {
        return buffer;
    }

// -----------------------------------------------------------------------------
    CLarray() NOEXCEPT
        : buffer(nullptr), buff_size(0)
    /**
        \bruef default contsructor
        \detailed empty constructor, do not allocating GPU memory

        \time O(1)
        \memory O(1)

        \GPUtime O(1)
        \GPUmemory O(1)
    */
    {}

// -----------------------------------------------------------------------------
    CLarray &operator =(CLarray &&mv) THROW
    /**
        \brief move operator
        \detailed copy reference to GPU memory block to other object, do not
            creating new GPU buffer
        \param mv source CLarray object

        \time O(1)
        \memory O(1)

        \GPUtime O(1)
        \GPUmemory O(1)
    */
    {
        if (this == &mv)
            return *this;
        _destroy();
        buffer = std::move(mv.buffer);
        buff_size = mv.buff_size;
        mv.buffer = nullptr;
        mv.buff_size = 0;
        return *this;
    }

// -----------------------------------------------------------------------------
    CLarray(CLarray &&cpy) THROW
        : buffer(nullptr), buff_size(0)
    /**
        \brief move operator
        \detailed see `operator =`
    */
    {
        *this = std::move(cpy);
    }

// =============================================================================
// -------------------------------- array info ---------------------------------
# ifdef CL_MEM_TYPE
    WUR UNUSED cl_mem_object_type get_mem_type() const THROW
    {
        return static_cast<cl_mem_object_type>(_get_numeric_data(
            CL_MEM_TYPE, sizeof(cl_mem_object_type)));
    }
# endif

// -----------------------------------------------------------------------------
# ifdef CL_MEM_FLAGS
    WUR UNUSED cl_mem_flags get_mem_flags() const THROW
    {
        return static_cast<cl_mem_flags>(_get_numeric_data(
            CL_MEM_FLAGS, sizeof(cl_mem_flags)));
    }
# endif

// -----------------------------------------------------------------------------
# ifdef CL_MEM_SIZE
    WUR UNUSED size_t get_mem_size() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(
            CL_MEM_SIZE, sizeof(size_t)));
    }
# endif

// -----------------------------------------------------------------------------
# ifdef CL_MEM_HOST_PTR
    WUR UNUSED void *get_mem_host_ptr() const THROW
    {
        return reinterpret_cast<void *>(_get_numeric_data(
            CL_MEM_HOST_PTR, sizeof(void *)));
    }
# endif

// -----------------------------------------------------------------------------
# ifdef CL_MEM_MAP_COUNT
    WUR UNUSED cl_uint get_mem_map_count() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(
            CL_MEM_MAP_COUNT, sizeof(cl_uint)));
    }
# endif

// -----------------------------------------------------------------------------
# ifdef CL_MEM_REFERENCE_COUNT
    WUR UNUSED cl_uint get_mem_reference_count() const THROW
    {
        return static_cast<cl_uint>(_get_numeric_data(
            CL_MEM_REFERENCE_COUNT, sizeof(cl_uint)));
    }
# endif

// -----------------------------------------------------------------------------
# ifdef CL_MEM_CONTEXT
    WUR UNUSED cl_context get_mem_context() const THROW
    {
        return static_cast<cl_context>(_get_numeric_data(
            CL_MEM_CONTEXT, sizeof(cl_context)));
    }
# endif

// -----------------------------------------------------------------------------
# ifdef CL_MEM_ASSOCIATED_MEMOBJECT
    WUR UNUSED cl_mem get_mem_associated_memobject() const THROW
    {
        return static_cast<cl_mem>(_get_numeric_data(
            CL_MEM_ASSOCIATED_MEMOBJECT, sizeof(cl_mem)));
    }
# endif

// -----------------------------------------------------------------------------
# ifdef CL_MEM_OFFSET
    WUR UNUSED size_t get_mem_offset() const THROW
    {
        return static_cast<size_t>(_get_numeric_data(
            CL_MEM_OFFSET, sizeof(size_t)));
    }
# endif

// =============================================================================
// ------------------------------ private methods ------------------------------
private:
    WUR unsigned long long _get_numeric_data(
        cl_mem_info type,
        size_t value_size
    ) const THROW
    /**
        \brief numeric info getter
        \defailed get requested GPU memory buffer info
        
        \param type CL_MEM_* macro value
        \param size of returned number
    */
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

// -----------------------------------------------------------------------------
    void _destroy() THROW
    /**
        \brief destructor routine
        \detailed deallocates GPU memory block
    */
    {
        cl_int  error;

        if (buffer != nullptr)
        {
            if (__VERBOSE)
                std::cout << "released array " << this
                    << " (" << buffer << ')' << std::endl;
            error = clReleaseMemObject(buffer);
            if (error != CL_SUCCESS)
                throw CLexception(error);
        }
    }

// =============================================================================
// ------------------------------ private members ------------------------------
private:
    cl_mem      buffer;
    size_t      buff_size;

    friend class CLkernel;

}; /* CLarray */

CLLIB_NAMESPACE_END

#endif /* CLLIB_CLARRAY_HPP */
