
#ifndef CLLIB_PLATFORM_HPP
# define CLLIB_PLATFORM_HPP

# include "defs.hpp"
# include "CLexception.hpp"
# include "CLdevice.hpp"

# include <string>
# include <vector>

CLLIB_NAMESPACE_BEGIN

class CLplatform /*: public __utils::__noncopymovable<>*/
{
public:
    WUR std::vector<CLdevice> get_devices(
        cl_device_type type=CLdevice::any_type,
        cl_uint num_entries=-1
    ) const
    {
        cl_int      error;
        cl_uint     max_num;

        std::vector<cl_device_id> devices;
        while (devices.size() < num_entries)
        {
            devices.push_back(cl_device_id());
            error = clGetDeviceIDs(
                platform_id,
                type,
                devices.size(),
                devices.data(),
                &max_num
            );
            if (error != CL_SUCCESS)
                throw CLexception(error);
            if (max_num < devices.size())
            {
                devices.pop_back();
                break ;
            }
        }

        std::vector<CLdevice> ret;
        ret.reserve(devices.size());
        for (auto &device : devices)
            ret.emplace_back(CLdevice(device));

        return ret;
    }

# ifdef CL_PLATFORM_PROFILE
    WUR UNUSED std::string get_platform_profile() const
    {
        return _get_string_data(CL_PLATFORM_PROFILE);
    }
# endif

# ifdef CL_PLATFORM_VERSION
    WUR UNUSED std::string get_platform_version() const
    {
        return _get_string_data(CL_PLATFORM_VERSION);
    }
# endif

# ifdef CL_PLATFORM_NAME
    WUR UNUSED std::string get_platform_name() const
    {
        return _get_string_data(CL_PLATFORM_NAME);
    }
# endif

# ifdef CL_PLATFORM_VENDOR
    WUR UNUSED std::string get_platform_vendor() const
    {
        return _get_string_data(CL_PLATFORM_VENDOR);
    }
# endif

# ifdef CL_PLATFORM_EXTENSIONS
    WUR UNUSED std::string get_platform_extensions() const
    {
        return _get_string_data(CL_PLATFORM_EXTENSIONS);
    }
# endif

# ifdef CL_PLATFORM_HOST_TIMER_RESOLUTION
    WUR UNUSED cl_ulong get_platform_timer_resolution() const
    {
        return _get_ulong_data(CL_PLATFORM_HOST_TIMER_RESOLUTION);
    }
# endif

#ifdef CL_PLATFORM_ICD_SUFFIX_KHR
    WUR UNUSED std::string get_platform_icd_suffix_khr() const
    {
        return _get_string_data(CL_PLATFORM_ICD_SUFFIX_KHR);
    }
#endif /* CL_PLATFORM_ICD_SUFFIX_KHR */

//    CLplatform(const CLplatform &cpy) = default;

    WUR const cl_platform_id &__get_platform() const
    {
        return platform_id;
    }

    CLplatform()=delete;

private:
    void _set_platform_id(cl_platform_id id)
    {
        platform_id = id;
    }

    WUR std::string _get_string_data(cl_platform_info type) const
    {
        cl_int      error;
        size_t      info_size;
        std::string info;

        error = clGetPlatformInfo(
            platform_id,
            type,
            0,
            nullptr,
            &info_size
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        info.resize(info_size - 1);
        error = clGetPlatformInfo(
            platform_id,
            type,
            info_size,
            info.data(),
            nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return info;
    }

    WUR cl_ulong _get_ulong_data(cl_platform_info type) const
    {
        cl_int      error;
        cl_ulong    info;

        error = clGetPlatformInfo(
                platform_id,
                type,
                sizeof(cl_ulong),
                &info,
                nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return info;
    }

private:
    CLplatform(cl_platform_id platform)
        : platform_id(platform)
    {}

    cl_platform_id  platform_id;

    friend std::vector<CLplatform> get_platforms(cl_uint);
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_PLATFORM_HPP */
