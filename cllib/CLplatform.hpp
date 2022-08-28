
#ifndef CLLIB_PLATFORM_HPP
# define CLLIB_PLATFORM_HPP
# include "defs.hpp"
# include "CLexception.hpp"

CLLIB_NAMESPACE_BEGIN

class CLplatform
{
public:
    CLplatform(cl_uint num_entries=1)
        : platform_id(), num_platforms()
    {
        cl_int  error;

        error = clGetPlatformIDs(num_entries, &platform_id, &num_platforms);
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    WUR cl_platform_id get_platform_id() const
    {
        return platform_id;
    }

    WUR cl_uint get_num_platforms() const
    {
        return num_platforms;
    }

private:
    cl_platform_id  platform_id;
    cl_uint         num_platforms;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_PLATFORM_HPP */
