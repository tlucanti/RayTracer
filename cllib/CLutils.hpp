
#ifndef CLLIB_UTILS_HPP
# define CLLIB_UTILS_HPP

# include <regex>
# include <string>

namespace __utils
{
# ifndef __REPLACER
#  define __REPLACER
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
# endif /* __REPLACER */

# ifndef __NONCOPYBLE
#  define __NONCOPYBLE
    template <class ...>
    struct __noncopyble
    {
        __noncopyble()=default;
        __noncopyble(const __noncopyble &)=delete;
        __noncopyble &operator =(const __noncopyble &)=delete;
    };
# endif /* __NONCOPYBLE */

# ifndef __NONMOVABLE
#  define __NONMOVABLE
    template <class ...>
    struct __nonmovable
    {
        __nonmovable()=default;
        __nonmovable(const __nonmovable &&)=delete;
        __nonmovable &&operator =(const __nonmovable &&)=delete;
    };
# endif /* __NONMOVABLE */

# ifndef __NONCOPYMOVABLE
#  define __NONCOPYMOVABLE
    template <class ...T>
    struct __noncopymovable : public __noncopyble<T...>, public __nonmovable<T...>
    {
        __noncopymovable()=default;
    };
# endif /* __NONCOPYMOVABLE */

} /* namespace __utils */

#endif /* CLLIB_UTILS_HPP */
