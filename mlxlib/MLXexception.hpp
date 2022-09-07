
#ifndef MLXLIB_EXCEPTION_HPP
# define MLXLIB_EXCEPTION_HPP

# include "MLXdefs.hpp"

# include <stdexcept>
# include <string>

MLXLIB_NAMESPACE_BEGIN

class MLXexception : public std::runtime_error
{
public:
    MLXexception(const std::string &message)
        : std::runtime_error(message)
    {}
};

MLXLIB_NAMESPACE_END

#endif /* MLXLIB_EXCEPTION_HPP */
