
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

    template <class ...>
    struct __noncopyble
    {
        __noncopyble()=default;
        __noncopyble(const __noncopyble &)=delete;
        __noncopyble &operator =(const __noncopyble &)=delete;
    };

    template <class ...>
    struct __nonmovable
    {
        __nonmovable()=default;
        __nonmovable(const __nonmovable &&)=delete;
        __nonmovable &&operator =(const __nonmovable &&)=delete;
    };

    template <class ...T>
    struct __noncopymovable : public __noncopyble<T...>, public __nonmovable<T...>
    {
        __noncopymovable()=default;
    };
}

CLLIB_NAMESPACE_END

#endif /* CLLIB_UTILS_HPP */
