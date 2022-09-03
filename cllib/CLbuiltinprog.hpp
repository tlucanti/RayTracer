
#ifndef CLLIB_BUILTINPROG
# define CLLIB_BUILTINPROG

# include "defs.hpp"

# include <string>

CLLIB_NAMESPACE_BEGIN

class CLbuiltinprog
{
public:
    CLbuiltinprog(int arg_count, const std::string &program_code)
        : argc(arg_count), program(program_code)
    {}

    WUR const std::string &__get_code() const
    {
        return program;
    }

    WUR int __get_argc() const
    {
        return argc;
    }

private:
    const std::string &program;
    int argc;
};

CLLIB_NAMESPACE_END

#endif /* CLLIB_BUILTINPROG */