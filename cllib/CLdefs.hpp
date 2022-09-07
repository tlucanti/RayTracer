
#ifndef CLLIB_DEFS_HPP
# define CLLIB_DEFS_HPP

# ifdef __APPLE__
#  include <OpenCL/opencl.h>
# else /* not __APPLE__ */
#  include <CL/cl.h>
# endif /* __APPLE__ */

# ifdef __APPLE__
#  define clCreateCommandQueueWithProperties    clCreateCommandQueueWithPropertiesAPPLE
//#  define cl_command_queue_properties           cl_queue_properties_APPLE
# endif /* __APPLE__ */

# include <type_traits>

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

# ifndef THROW
#  define THROW noexcept(false)
# endif

# define IS_INTEGRAL(__type) std::is_integral<__type>::value
# define IS_FLOATING(__type) std::is_floating_point<__type>::value
# define IS_ARITHMETIC(__type) std::is_arithmetic<__type>::value


#endif /* CLLIB_DEFS_HPP */
