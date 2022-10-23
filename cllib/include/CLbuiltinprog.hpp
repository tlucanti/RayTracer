
#ifndef CLLIB_BUILTINPROG
# define CLLIB_BUILTINPROG

# include "CLcommon.hpp"

# include <string>

CLLIB_NAMESPACE_BEGIN

class CLbuiltinprog : __utils::__noncopymovable<>
{
public:
    CLbuiltinprog(unsigned int arg_count, const std::string &program_name, const std::string &program_code) NOEXCEPT
        : program(program_code), argc(arg_count), name(program_name)
    {}

    WUR UNUSED const std::string &get_program_source() const NOEXCEPT
    {
        return program;
    }

    WUR UNUSED unsigned int get_programd_arg_num() const NOEXCEPT
    {
        return argc;
    }

    WUR std::string get_program_name() const NOEXCEPT
    {
        return name;
    }

    CLbuiltinprog() = delete;

private:
    std::string     program;
    unsigned int    argc;
    std::string     name;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_BUILTINPROG */