
#ifndef CLLIB_DEVICE_HPP
# define CLLIB_DEVICE_HPP

# include "defs.hpp"
# include "CLplatform.hpp"
# include "CLexception.hpp"

CLLIB_NAMESPACE_BEGIN

cl_device_type any_type = CL_DEVICE_TYPE_DEFAULT;
cl_device_type gpu_type = CL_DEVICE_TYPE_GPU;
cl_device_type cpu_type = CL_DEVICE_TYPE_CPU;

class CLdevice
{
public:
    CLdevice(
        const CLplatform &platform,
        cl_device_type type=any_type,
        cl_uint num_entries=1
    ) :
        device_id(),
        num_devices()
    {
        cl_int  error;

        error = clGetDeviceIDs(platform.get_platform_id(), type, num_entries, &device_id, &num_devices);
        if (error)
            throw CLexception(error);
    }

    WUR cl_device_id get_device_id() const
    {
        return device_id;
    }

    WUR cl_uint get_num_devices() const
    {
        return num_devices;
    }

private:
    friend class CLcontext;
    friend class CLprogram;

    cl_device_id    device_id;
    cl_uint         num_devices;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_DEVICE_HPP */
