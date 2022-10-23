
#ifndef UTILS_HPP
# define UTILS_HPP

# include <common.hpp>
# include <string>

inline std::string operator ""_s(const char *str, UNUSED unsigned long length)
{
    return {str};
}

RTX_NAMESPACE_BEGIN

template <uintmax_t mod=10000000001191, uintmax_t prime=257>
inline constexpr uintmax_t _poly_hash(const char *str, size_t size, size_t idx)
{
    if (size == 0)
        return 0;
    else if (size == 1)
        return static_cast<uintmax_t>(str[0]);
    else if (idx < size)
        return (
                       static_cast<uintmax_t>(str[idx])
                       + _poly_hash<mod, prime>(str, size, idx + 1) * prime % mod
               ) % mod;
    return static_cast<uintmax_t>(str[idx]);
}


inline uintmax_t hash(const std::string &str)
{
    return _poly_hash(str.c_str(), str.size(), 0);
}

RTX_NAMESPACE_END

inline constexpr uintmax_t operator ""_hash(const char *str, size_t len)
{
    return rtx::_poly_hash(str, len, 0);
}

#endif /* UTILS_HPP */
