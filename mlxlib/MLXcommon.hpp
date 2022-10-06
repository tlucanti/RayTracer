
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
    const int white =   0xFFFFFF;
    const int black =   0x000000;
    const int red =     0xFF0000;
    const int green =   0x00FF00;
    const int blue =    0x0000FF;
    const int cyan =    0x00FFFF;
    const int magenta = 0xFF00FF;
    const int yellow =  0xFFFF00;
    const int purple =  magenta;
}

MLXLIB_NAMESPACE_END

#endif /* MLXLIB_DEFS_HPP */