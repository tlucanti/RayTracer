
#ifndef CLLIB_UTILS_HPP
# define CLLIB_UTILS_HPP

# include <regex>
# include <string>

CLLIB_NAMESPACE_BEGIN

namespace __utils
{
    class __replacer
    {
    public:
        __replacer(const std::string &str)
            : __str(str)
        {}

        __replacer &replace(const std::string &from, const std::string &to)
        {
            __str = std::regex_replace(__str, std::regex(from), to);
            return *this;
        }

        WUR const std::string &str() const
        {
            return __str;
        }

    private:
        std::string __str;
    };
}

CLLIB_NAMESPACE_END

#endif /* CLLIB_UTILS_HPP */
