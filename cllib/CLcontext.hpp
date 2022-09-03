
#ifndef CLLIB_CONTEXT_HPP
# define CLLIB_CONTEXT_HPP

# include "defs.hpp"
# include "CLdevice.hpp"
# include "CLexception.hpp"

CLLIB_NAMESPACE_BEGIN

class CLcontext
{
public:
    CLcontext(
        const CLdevice &device,
        cl_uint num_devices=1,
        cl_context_properties *properties=nullptr,
        void (CL_CALLBACK * callback)(const char *, const void *, size_t, void *)=nullptr,
        void *user_data=nullptr
    ) :
        context()
    {
        cl_int  error;

        context = clCreateContext(properties, num_devices, &device.__get_device(), callback, user_data, &error);
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    WUR UNUSED cl_uint get_context_reference_count() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_CONTEXT_REFERENCE_COUNT, sizeof(cl_uint)));
    }

    WUR UNUSED cl_uint get_context_num_devices() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_CONTEXT_NUM_DEVICES, sizeof(cl_uint)));
    }

    WUR UNUSED std::vector<cl_device_id> get_context_devices() const
    {
        cl_int  error;
        std::vector<cl_device_id> ret(get_context_num_devices());
        error = clGetContextInfo(
            context,
            CL_CONTEXT_DEVICES,
            ret.size() * sizeof(cl_device_id),
            ret.data(),
            nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);
        return ret;
    }

    UNUSED void get_device_context_properties() {}

    WUR const cl_context &__get_context() const
    {
        return context;
    }

    ~CLcontext()
    {
        clReleaseContext(context);
    }

private:
    WUR unsigned long long _get_numeric_data(cl_context_info type, size_t value_size) const
    {
        cl_int              error;
        unsigned long long  info;

        error = clGetContextInfo(
                context,
                type,
                value_size,
                &info,
                nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return info;
    }

    cl_context  context;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_CONTEXT_HPP */
