
#ifndef MLXLIB_DEFS_HPP
# define MLXLIB_DEFS_HPP

# include <mlx.h>

# define MLXLIB_NAMESPACE mlxlib
# define MLXLIB_NAMESPACE_BEGIN namespace MLXLIB_NAMESPACE {
# define MLXLIB_NAMESPACE_END }

# ifndef NORET
#  define NORET [[noreturn]]
# endif /* NORET */

# ifndef WUR
#  define WUR [[nodiscard]]
# endif /* NODISCARD */

# ifndef NOEXCEPT
#  define NOEXCEPT noexcept
# endif /* NOEXCEPT */

# ifndef INLINE
#  define INLINE inline
# endif /* INLINE */

MLXLIB_NAMESPACE_BEGIN

namespace color
{
    inline constexpr unsigned int white =   0xFFFFFF;
    inline constexpr unsigned int black =   0x000000;
    inline constexpr unsigned int red =     0xFF0000;
    inline constexpr unsigned int green =   0x00FF00;
    inline constexpr unsigned int blue =    0x0000FF;
    inline constexpr unsigned int cyan =    0x00FFFF;
    inline constexpr unsigned int magenta = 0xFF00FF;
    inline constexpr unsigned int yellow =  0xFFFF00;
    inline constexpr unsigned int purple =  magenta;
}

MLXLIB_NAMESPACE_END

#endif /* MLXLIB_DEFS_HPP */
