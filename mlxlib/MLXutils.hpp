
#ifndef MLXLIB_UTILS_HPP
# define MLXLIB_UTILS_HPP

namespace __utils
{
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

#endif /* MLXLIB_UTILS_HPP */
