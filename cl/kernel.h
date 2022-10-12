
#ifndef CL_KERNEL_H
# define CL_KERNEL_H

# ifndef __CPP
#  define CPP_UNUSED
#  define CPP_INLINE
#  define ASSIGN_FLOAT3(__x, __y, __z) (FLOAT3)(__x, __y, __z)
#  define ASSIGN_COMPLEX(__real, __imag) (COMPLEX)(__real, __imag)
#  define ASSIGN_SCENE(...) (scene_t){__VA_ARGS__}
#  define cstatic_cast(__type, __var) ((__type)(__var))
# else
#  include <linalg.hpp>
#  define __constant
#  define __global
#  define __kernel
#  define CPP_UNUSED __attribute__((unused))
#  define CPP_INLINE inline
#  define NULLPTR nullptr
#  define ASSIGN_FLOAT3(__x, __y, __z) {{__x, __y, __z}}
#  define ASSIGN_COMPLEX(__real, __imag) {{__real, __imag}}
#  define ASSIGN_SCENE(...) {__VA_ARGS__}
#  define cstatic_cast(__type, __var) static_cast<__type>(__var)

CPP_INLINE
int32_t get_global_id(uint32_t)
{
	return {};
}

using rtx::linalg::length;
using rtx::linalg::normalize;
using rtx::linalg::dot;
using rtx::linalg::cross;

using std::isnan;
using std::isinf;

# endif/* __CPP */

#endif /* CL_KERNEL_H */
