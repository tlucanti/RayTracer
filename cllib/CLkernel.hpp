
#ifndef CLLIB_KERNEL_HPP
# define CLLIB_KERNEL_HPP

# include <string>
# include <utility>

# include "defs.hpp"
# include "CLexception.hpp"
# include "CLprogram.hpp"
# include "CLarray.hpp"

CLLIB_NAMESPACE_BEGIN

class CLkernel
{
public:
    CLkernel(const CLprogram &program, const std::string &name)
        : kernel(), argc(program.__get_argc())
    {
        cl_int  error;

        kernel = clCreateKernel(program.__get_program(), name.c_str(), &error);
        if (error)
            throw CLexception(error);
    }

    template <class... type>
    void set_args(const CLarray<type> &... arg)
    {
        clSetKernelArg(kernel, i, sizeof(cl_mem), arg.get_buffer());
    }

    WUR const cl_kernel &__get_kernel() const
    {
        return kernel;

    }

private:
    cl_kernel   kernel;
    int         argc;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_KERNEL_HPP */