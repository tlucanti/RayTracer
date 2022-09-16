
#ifndef CLLIB_BUILTINPROG
# define CLLIB_BUILTINPROG

# include "CLdefs.hpp"

# include <string>

CLLIB_NAMESPACE_BEGIN

class CLbuiltinprog
{
public:
    CLbuiltinprog(int arg_count, const std::string &program_name, const std::string &program_code)
        : argc(arg_count), program(program_code), name(program_name)
    {}

    WUR UNUSED const std::string &get_program_source() const
    {
        return program;
    }

    WUR UNUSED int get_programd_arg_num() const
    {
        return argc;
    }

    WUR std::string get_program_name() const
    {
        return name;
    }

    CLbuiltinprog()=delete;

private:
    std::string     program;
    int             argc;
    std::string     name;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_BUILTINPROG */