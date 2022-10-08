
#ifdef CL_KERNEL_H
# define CL_KERNEL_H

# define UNUSED  __attribute__((unused))
# ifndef NULL
#  define NULL    0
# endif /* NULL */
# define CPP_UNUSED
# define CPP_INLINE
# define EPS 1e-4
# define PACKED    __attribute__((__packed__))
# define ALIGNED8  __attribute__((__aligned__(8)))
# define ALIGNED16 __attribute__((__aligned__(16)))

typedef double FLOAT;
typedef double4 FLOAT4;
typedef double3 FLOAT3;
typedef double2 FLOAT2;
typedef FLOAT2 COMPLEX;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned long int uint64_t;
typedef long int int64_t;

#endif /* CL_KERNEL_H */
