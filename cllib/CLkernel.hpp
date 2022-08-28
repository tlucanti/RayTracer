
#ifndef CLLIB_KERNEL_HPP
# define CLLIB_KERNEL_HPP

# include <string>

# include "defs.hpp"
# include "CLexception.hpp"
# include "CLprogram.hpp"

CLLIB_NAMESPACE_BEGIN

class CLkernel
{
public:
    CLkernel(const CLprogram &program, const std::string &name)
        : kernel()
    {
        cl_int  error;

        kernel = clCreateKernel(program.get_program(), name.c_str(), &error);
        if (error)
            throw CLexception(error);
    }

    WUR cl_kernel get_kernel() const
    {
        return kernel;
    }

private:
    cl_kernel   kernel;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_KERNEL_HPP */