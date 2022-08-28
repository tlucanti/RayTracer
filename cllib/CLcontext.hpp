
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

        context = clCreateContext(properties, num_devices, &device.device_id, callback, user_data, &error);
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    WUR cl_context get_context() const
    {
        return context;
    }

private:
    cl_context  context;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_CONTEXT_HPP */
