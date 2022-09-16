
#ifndef CLLIB_GPUARRAY_HPP
# define CLLIB_GPUARRAY_HPP

# include "CLdefs.hpp"
# include "CLexception.hpp"
# include "CLcontext.hpp"
# include "CLqueue.hpp"

# include <vector>

CLLIB_NAMESPACE_BEGIN

const cl_mem_flags read_write_array = CL_MEM_READ_WRITE;
const cl_mem_flags read_only_array = CL_MEM_READ_ONLY;
const cl_mem_flags write_only_array = CL_MEM_WRITE_ONLY;

template <class value_type, cl_mem_flags flag=read_write_array>
class CLarray : public __utils::__noncopymovable<value_type>
{
public:
    CLarray(
        size_t size,
        const CLcontext &context,
        const CLqueue &queue,
        void *host_ptr=nullptr
    ) THROW;

    CLarray(
        const std::vector<value_type> &vec,
        const CLcontext &context,
        const CLqueue &queue,
        void *host_ptr=nullptr
    ) THROW;

    ~CLarray() THROW;

    size_t size() const NOEXCEPT;

    void fill(
        const std::vector<value_type> &vec,
        const CLqueue &queue,
        bool block=true,
        size_t offset=0,
        cl_uint num_events_in_wait_list=0,
        const cl_event *event_wait_list=nullptr,
        cl_event *event=nullptr
    ) THROW;

    #include "CLarray.ipp";
}; /* CLarray */

