
#ifndef CLLIB_DEFS_HPP
# define CLLIB_DEFS_HPP

# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else /* not __APPLE__ */
#  include <CL/cl.h>
# endif /* __APPLE__ */

# define CLIB_NAMESPACE cllib
# define CLLIB_NAMESPACE_BEGIN namespace CLIB_NAMESPACE {
# define CLLIB_NAMESPACE_END }

# define CPP	    1
# define CPP98		__cplusplus >= 199711L
# define CPP11		__cplusplus >= 201103L
# define CPP14		__cplusplus >= 201402L
# define CPP17		__cplusplus >= 201703L
# define CPP20		__cplusplus >= 202002L
# define CPP23		__cplusplus >= 202003L

# ifndef WUR
#  define WUR [[nodiscard]]
# endif /* WUR */

# ifndef UNUSED
#  define UNUSED [[maybe_unused]]
# endif /* UNUSED */

#endif /* CLLIB_DEFS_HPP */
