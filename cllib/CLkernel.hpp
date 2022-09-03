
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
    void set_args(const CLarray<type> &...arg)
    {
        static_assert(argc == sizeof...(arg), "argument count mismatch with kernel");
        _set_args_req(arg...);
    }

    WUR const cl_kernel &__get_kernel() const
    {
        return kernel;
    }

private:
    template <size_t I=0, class... type>
    typename std::enable_if<I == sizeof...(type), void>::type _set_args_rec(const CLarray<type> &...arg)
    {}

    template <size_t I=0, class... type>
    typename std::enable_if<I < sizeof...(type), void>::type _set_args_rec(const CLarray<type> &...arg)
    {
        clSetKernelArg(kernel, I, sizeof(cl_mem), std::get<I>(arg...).get_buffer());
        _set_args_rec<I + 1, type...>(arg...);
    }

    cl_kernel   kernel;
    int         argc;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_KERNEL_HPP */