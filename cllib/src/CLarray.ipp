

// -----------------------------------------------------------------------------
CLarray(
    size_t size,
    const CLcontext &context,
    void *host_ptr=nullptr
) THROW :
    buffer(), buff_size(size), ref_cnt(new int(1))
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
    bool block=true
    void *host_ptr=nullptr
) THROW :
    buffer(), buff_size(vec.size()), ref_cnt(new int(1))
/**
    \brief valued constructor
    \details constructor allocaes block GPU memory and fills it with contents
        of `vec`

    \param vec vector with data to be filled
    \param context OpenCL context where buffer will be visible
    \param queue OpenCL queue where filling operation will be enqueued
    \param block blocking operation flag, if set to true, function will block
        until all data will be filled
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
    */
    {
        _destroy();
    }

// -----------------------------------------------------------------------------
    size_t size() const NOEXCEPT
    /*
        return size of allocated GPU memory block
    */
    {
        return buff_size;
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
    )
    /*
        function fills GPU memory block with new data from `vec`, overriding old
        data.
    */
    {
        cl_int  error = CL_SUCCESS;

        if (vec.size() > buff_size)
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
        dump(out.data(), queue, block, size, offset, num_events_in_wait_list, event_wait_list, event);
    }

    void dump(
        value_type *out,
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

    WUR const cl_mem &data() const NOEXCEPT
    {
        return buffer;
    }

    CLarray() NOEXCEPT
        : buffer(nullptr), buff_size(0), ref_cnt(nullptr)
    {}

    CLarray &operator =(const CLarray &cpy)
    {
        if (this == &cpy or cpy.ref_cnt == nullptr)
            return *this;
        _destroy();
        buffer = cpy.buffer;
        buff_size = cpy.buff_size;
        ref_cnt = cpy.ref_cnt;
        ++*ref_cnt;
        return *this;
    }

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

    void _destroy()
    {
        cl_int  error;

        if (ref_cnt != nullptr and --*ref_cnt == 0)
        {
            delete ref_cnt;
            std::cout << "released array " << this << " (" << buffer << ')' << std::endl;
            error = clReleaseMemObject(buffer);
            if (error != CL_SUCCESS)
                throw CLexception(error);
        }
    }

    cl_mem      buffer;
    size_t      buff_size;
    int         *ref_cnt;

    friend class CLkernel;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_GPUARRAY_HPP */
