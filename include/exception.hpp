
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
        : std::runtime_error(rtx::Red["[FATAL]: "] + rtx::DarkRed[msg])
    {}
};

inline void __Msg(const std::string &status, const std::string &parent, const std::string &message)
{
    std::cout << status << ": "_W << P[parent] << ": "_W << message << rtx::Color::reset << std::endl;
}

inline void Info(const std::string &parent, const std::string &message)
{
    __Msg("[INFO]"_B, parent, message);
}

inline void Ok(const std::string &parent, const std::string &message)
{
    __Msg("[ OK ]"_G, parent, message);
}

inline void Warning(const std::string &parent, const std::string &message)
{
    __Msg("[WARN]"_Y, parent, message);
}

inline void Error(const std::string &parent, const std::string &message)
{
    __Msg("[FAIL]"_R, parent, message);
}

RTX_NAMESPACE_END

#endif /* RTX_EXCEPTION */