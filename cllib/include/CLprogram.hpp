
#ifndef CLLIB_PROGRAM_HPP
# define CLLIB_PROGRAM_HPP

# include "CLcommon.hpp"
# include "CLutils.hpp"
# include "CLexception.hpp"
# include "CLcontext.hpp"
# include "CLdevice.hpp"

# include <string>
# include <fstream>

CLLIB_NAMESPACE_BEGIN

class CLprogram : public __utils::__noncopymovable<>
{
public:
    CLprogram(
        const CLbuiltinprog &program,
        const CLcontext &context
    ) : CLprogram(program.get_programd_arg_num(), program.get_program_source(), program.get_program_name(), context) {}

    CLprogram(
        int argc,
        std::ifstream stream,
        const std::string &program_name,
        const CLcontext &context
    ) : CLprogram(argc, __utils::__get_stream_content(stream), program_name, context)
    {}

    CLprogram(
        int argc,
        const std::string &program_code,
        const std::string &program_name,
        const CLcontext &context
    ) :
        builder(), argc(argc), program(), name(program_name)
    {
        cl_int  error;
        const char  *code_ptr = program_code.c_str();

        program = clCreateProgramWithSource(context.__get_context(), 1, &code_ptr, nullptr, &error);
        if (error)
            throw CLexception(error);
    }

    ~CLprogram() THROW
    {
        cl_int error;

        error = clReleaseProgram(program);
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    void compile(
        const CLdevice &device,
        bool verbose=false,
        const char *options=nullptr,
        cl_uint num_devices=1,
        void (CL_CALLBACK *notify)(cl_program, void *)=nullptr,
        void *user_data=nullptr
    )
    {
        cl_int  error;

        builder.program = program;
        builder.device = device.__get_device();
        error = clBuildProgram(program, num_devices, &device.__get_device(), options, notify, user_data);
        if (error == CL_BUILD_PROGRAM_FAILURE and verbose)
            std::cout << builder.get_program_log() << std::endl;
        if (error != CL_SUCCESS)
            throw CLexception(error);
    }

    WUR std::string get_program_name() const
    {
        return name;
    }

# ifdef CL_PROGRAM_REFERENCE_COUNT
    WUR UNUSED cl_uint get_program_reference_count() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_PROGRAM_REFERENCE_COUNT, sizeof(cl_uint)));
    }
# endif

# ifdef CL_PROGRAM_CONTEXT
    WUR UNUSED cl_context get_program_context() const
    {
        return _get_struct_data<cl_context>(CL_PROGRAM_CONTEXT);
    }
# endif

# ifdef CL_PROGRAM_NUM_DEVICES
    WUR UNUSED cl_uint get_program_num_devices() const
    {
        return static_cast<cl_uint>(_get_numeric_data(CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint)));
    }
# endif

# ifdef CL_PROGRAM_DEVICES
    WUR UNUSED std::vector<cl_device_id> get_program_devices() const
    {
        cl_int error;
        std::vector<cl_device_id> ret(get_program_num_devices());

        error = clGetProgramInfo(
            program,
            CL_PROGRAM_DEVICES,
            sizeof(cl_device_id) * ret.size(),
            ret.data(),
            nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return ret;
    }
# endif

# ifdef CL_PROGRAM_SOURCE
    WUR UNUSED std::string get_program_source() const
    {
        return _get_string_data(CL_PROGRAM_SOURCE);
    }
# endif

# ifdef CL_PROGRAM_IL
    WUR UNUSED void *get_program_il() const
    {
        return reinterpret_cast<void *>(_get_numeric_data(CL_PROGRAM_IL, sizeof(void *)));
    }
# endif

# ifdef CL_PROGRAM_BINARY_SIZES
    WUR UNUSED std::vector<size_t> get_program_binary_sizes() const
    {
        cl_int error;
        std::vector<size_t> ret(get_program_num_devices());

        error = clGetProgramInfo(
                program,
                CL_PROGRAM_BINARY_SIZES,
                sizeof(size_t) * ret.size(),
                ret.data(),
                nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return ret;
    }
# endif

# ifdef CL_PROGRAM_BINARIES
    WUR UNUSED std::vector<char *> get_program_binaries() const
    {
        cl_int error;
        std::vector<char *> ret(get_program_num_devices());

        error = clGetProgramInfo(
            program,
            CL_PROGRAM_BINARIES,
            sizeof(size_t) * ret.size(),
            ret.data(),
            nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return ret;
    }
# endif

# ifdef CL_PROGRAM_NUM_KERNELS
    WUR UNUSED size_t get_program_num_kernels() const
    {
        return static_cast<size_t>(_get_numeric_data(CL_PROGRAM_NUM_KERNELS, sizeof(size_t)));
    }
# endif

# ifdef CL_PROGRAM_KERNEL_NAMES
    WUR UNUSED std::string get_program_kernel_names() const
    {
        return _get_string_data(CL_PROGRAM_KERNEL_NAMES);
    }
#endif

    WUR const cl_program &__get_program() const
    {
        return program;
    }

    WUR int __get_argc() const
    {
        return argc;
    }

    CLprogram()=delete;

private:

    WUR unsigned long long _get_numeric_data(cl_program_info type, size_t value_size) const
    {
        cl_int              error;
        unsigned long long  info;

        error = clGetProgramInfo(
            program,
            type,
            value_size,
            &info,
            nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return info;
    }

    template <class struct_T>
    WUR struct_T _get_struct_data(cl_program_info type) const
    {
        cl_int      error;
        struct_T    info;

        error = clGetProgramInfo(
                program,
                type,
                sizeof(struct_T),
                &info,
                nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return info;
    }

    WUR std::string _get_string_data(cl_program_info type) const
    {
        cl_int      error;
        size_t      info_size;
        std::string info;

        error = clGetProgramInfo(
                program,
                type,
                0,
                nullptr,
                &info_size
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        info.resize(info_size - 1);
        error = clGetProgramInfo(
                program,
                type,
                info_size,
                info.data(),
                nullptr
        );
        if (error != CL_SUCCESS)
            throw CLexception(error);

        return info;
    }

    private:
        class __CLbuilder
        {
        private:
            __CLbuilder()
                : program(), device()
            {}

        public:
            __CLbuilder(cl_program program, cl_device_id device)
                : program(program), device(device)
            {}

# ifdef CL_PROGRAM_BUILD_STATUS
            WUR UNUSED cl_build_status get_program_build_status()
            {
                return static_cast<cl_build_status>(_get_numeric_data(CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status)));
            }
# endif

# ifdef CL_PROGRAM_BUILD_OPTIONS
            WUR UNUSED std::string get_program_build_options()
            {
                return _get_string_data(CL_PROGRAM_BUILD_OPTIONS);
            }
# endif

# ifdef CL_PROGRAM_BUILD_LOG
            WUR UNUSED std::string get_program_log()
            {
                return _get_string_data(CL_PROGRAM_BUILD_LOG);
            }
# endif

# ifdef CL_PROGRAM_BINARY_TYPE
            WUR UNUSED cl_program_binary_type get_program_binary_type()
            {
                return static_cast<cl_program_binary_type>(_get_numeric_data(CL_PROGRAM_BINARY_TYPE, sizeof(cl_program_binary_type)));
            }
# endif

# ifdef CL_PROGRAM_BUILD_GLOBAL_VARIABLE_TOTAL_SIZE
            WUR UNUSED size_t get_program_build_global_variable_total_size()
            {
                return static_cast<size_t>(_get_numeric_data(CL_PROGRAM_BUILD_GLOBAL_VARIABLE_TOTAL_SIZE, sizeof(size_t)));
            }
# endif

        private:
            WUR UNUSED unsigned long long _get_numeric_data(cl_program_build_info type, size_t value_size) const
            {
                cl_int              error;
                unsigned long long  info;

                if (program == nullptr || device == nullptr)
                    throw std::runtime_error("builder info can be obtained only after compile");
                error = clGetProgramBuildInfo(
                        program,
                        device,
                        type,
                        value_size,
                        &info,
                        nullptr
                );
                if (error != CL_SUCCESS)
                    throw CLexception(error);

                return info;
            }

            WUR std::string _get_string_data(cl_program_info type) const
            {
                cl_int      error;
                size_t      info_size;
                std::string info;

                if (program == nullptr || device == nullptr)
                    throw std::runtime_error("builder info can be obtained only after compile");
                error = clGetProgramBuildInfo(
                        program,
                        device,
                        type,
                        0,
                        nullptr,
                        &info_size
                );
                if (error != CL_SUCCESS)
                    throw CLexception(error);

                info.resize(info_size - 1);
                error = clGetProgramBuildInfo(
                        program,
                        device,
                        type,
                        info_size,
                        info.data(),
                        nullptr
                );
                if (error != CL_SUCCESS)
                    throw CLexception(error);

                return info;
            }

            cl_program      program;
            cl_device_id    device;

            friend class CLprogram;
        }; /* __CLbuilder */

public:
    __CLbuilder builder;

private:
    cl_program  program;
    int         argc;
    std::string name;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_PROGRAM_HPP */
