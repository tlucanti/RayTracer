
#ifndef CLLIB_PROGRAM_HPP
# define CLLIB_PROGRAM_HPP

# include <string>

# include "defs.hpp"
# include "CLexception.hpp"
# include "CLcontext.hpp"
# include "CLdevice.hpp"

CLLIB_NAMESPACE_BEGIN

class CLprogram
{
public:
    CLprogram(
        const CLbuiltinprog &program,
        const CLcontext &context
    ) : CLprogram(program.__get_argc(), program.__get_code(), context) {}

    CLprogram(
        int argc,
        const std::string &kernel_code,
        const CLcontext &context
    ) :
        argc(argc), program()
    {
        cl_int  error;
        const char  *code_ptr = kernel_code.c_str();

        program = clCreateProgramWithSource(context.__get_context(), 1, &code_ptr, nullptr, &error);
        if (error)
            throw CLexception(error);
    }

    void compile(
        const CLdevice &device,
        const char *options=nullptr,
        cl_uint num_devices=1,
        void (CL_CALLBACK *notify)(cl_program, void *)=nullptr,
        void *user_data=nullptr
    )
    {
        cl_int  error;

        error = clBuildProgram(program, num_devices, &device.__get_device(), options, notify, user_data);
        if (error)
            throw CLexception(error);
    }

    WUR const cl_program &__get_program() const
    {
        return program;
    }

    WUR int __get_argc() const
    {
        return argc;
    }

private:
    cl_program  program;
    int         argc;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_PROGRAM_HPP */
