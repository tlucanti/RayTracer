
#ifndef CLLIB_KERNEL_HPP
# define CLLIB_KERNEL_HPP

# include <string>

# include "defs.hpp"
# include "CLexception.hpp"
# include "CLprogram.hpp"
# include "CLarray.hpp"

CLLIB_NAMESPACE_BEGIN

class CLkernel
{
public:
    CLkernel(const sources::CLbuiltinprog &program)
        : CLkernel(program.code(), program,argc(), program.name())
    {}

    CLkernel(const CLprogram &program, int argc, const std::string &name)
        : kernel()
    {
        cl_int  error;

        kernel = clCreateKernel(program.__get_program(), name.c_str(), &error);
        if (error)
            throw CLexception(error);
    }

    template <class... types>
    void set_args(const types &... args)
    {
        va_
    }

    WUR const cl_kernel &__get_kernel() const
    {
        return kernel;
    }

private:
    cl_kernel   kernel;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_KERNEL_HPP */