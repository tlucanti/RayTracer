
#ifndef RTX_EXCEPTION
# define RTX_EXCEPTION

# include <common.hpp>
# include <color.hpp>
# include <string>
# include <stdexcept>
# include <sstream>

RTX_NAMESPACE_BEGIN

struct Exception : public std::runtime_error
{
    Exception(const std::string &msg) NOEXCEPT
        : std::runtime_error(msg.c_str())
    {}
};

inline void Info(const std::string &parent, const std::string &message)
{
    std::cout << "[INFO]"_B << ": "_W << P[parent] << message << rtx::Color::reset << std::endl;
}

inline void Ok(const std::string &parent, const std::string &message)
{
    std::cout << "[ OK ]"_G << ": "_W << P[parent] << message << rtx::Color::reset << std::endl;
}

inline void Warning(const std::string &parent, const std::string &message)
{
    std::cout << "[WARN]"_Y << ": "_W << P[parent] << message << rtx::Color::reset << std::endl;
}

inline void Error(const std::string &parent, const std::string &message)
{
    std::cout << "[FAIL]"_R << ": "_W << P[parent] << message << rtx::Color::reset << std::endl;
}

RTX_NAMESPACE_END

#endif /* RTX_EXCEPTION */