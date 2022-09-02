
#ifndef CLLIB_HPP
# define CLLIB_HPP

# include "defs.hpp"
# include "CLutils.hpp"
# include "CLsources.hpp"
# include "CLexception.hpp"
//
# include "CLplatform.hpp"
# include "CLdevice.hpp"
# include "CLcontext.hpp"
# include "CLqueue.hpp"
# include "CLprogram.hpp"
# include "CLkernel.hpp"
# include "CLarray.hpp"

# include <vector>

CLLIB_NAMESPACE_BEGIN

WUR std::vector<CLplatform> get_platforms(cl_uint num_platforms=-1)
{
    cl_int  error;

    error = clGetPlatformIDs(
        num_platforms,
        nullptr,
        &num_platforms
    );
    if (error != CL_SUCCESS)
        throw CLexception(error);

    std::vector<cl_platform_id> platforms(num_platforms);
    error = clGetPlatformIDs(
        num_platforms,
        platforms.data(),
        &num_platforms
    );
    if (error != CL_SUCCESS)
        throw CLexception(error);

    std::vector<CLplatform> ret(num_platforms, CLplatform());
    for (cl_uint i=0; i < num_platforms; ++i)
        ret.at(i)._set_platform_id(platforms.at(i));

    return ret;
}

CLLIB_NAMESPACE_END

#endif /* CLLIB_HPP */
