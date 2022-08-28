
#ifndef CLLIB_UTILS_HPP
# define CLLIB_UTILS_HPP

CLLIB_NAMESPACE_BEGIN

# include <regex>
# include <string>

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
            __str = std::regex_replace(str, std::regex(from), to);
            return *this;
        }

        const std::string &str() const
        {
            return __str;
        }

    private:
        std::string __str;
    };
}

CLLIB_NAMESPACE_END

#endif /* CLLIB_UTILS_HPP */
