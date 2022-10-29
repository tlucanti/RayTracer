
#ifndef LINALG_HPP
# define LINALG_HPP

# include <common.hpp>

FLOAT3 operator +(const FLOAT3 &a, const FLOAT3 &b);
FLOAT3 operator -(const FLOAT3 &a);
FLOAT3 operator -(const FLOAT3 &a, const FLOAT3 &b);
FLOAT3 operator *(const FLOAT3 &a, FLOAT t);
FLOAT3 operator *(const FLOAT3 &a, const FLOAT3 &b);
FLOAT3 &operator +=(FLOAT3 &a, const FLOAT3 &b);
FLOAT3 &operator *=(FLOAT3 &a, FLOAT b);

COMPLEX operator *(FLOAT a, const COMPLEX &b);
COMPLEX operator +(const COMPLEX &a, const COMPLEX &b);
COMPLEX operator -(const COMPLEX &a, const COMPLEX &b);
COMPLEX operator -(const COMPLEX &a);
COMPLEX to_complex(const cl_int2 &a);

cl_int2 operator -(const cl_int2 &a, const cl_int2 &b);

RTX_NAMESPACE_BEGIN

namespace linalg
{
    FLOAT3 cross(const FLOAT3 &a, const FLOAT3 &b);
    FLOAT det(const FLOAT3 &v1, const FLOAT3 &v2, const FLOAT3 &v3);
    void inv(const FLOAT3 matr[3], FLOAT3 dest[3]);
    FLOAT dot(const FLOAT3 &v1, const FLOAT3 &v2);
    FLOAT dot(const COMPLEX &v1, const COMPLEX &v2);
    int dot(const cl_int2 &v1, const cl_int2 &v2);
    FLOAT length(const FLOAT3 &v);
    FLOAT length(const COMPLEX &v);
    FLOAT3 normalize(const FLOAT3 &v);
    COMPLEX normalize(const COMPLEX &v);
    FLOAT3 fmin(FLOAT3 x, FLOAT y);
    FLOAT3 fmin(FLOAT3 x, FLOAT3 y);
    FLOAT3 fmax(FLOAT3 x, FLOAT y);
    FLOAT3 fmax(FLOAT3 x, FLOAT3 y);
    FLOAT3 fabs(FLOAT3 x);

    void normalize_ref(FLOAT3 &vec);
    void compute_angles(const FLOAT3 &direction, FLOAT &alpha, FLOAT &theta);
    void compute_angles_gamma(const FLOAT3 &direction, FLOAT3 &atg);
    void compute_matrix(FLOAT3 rotate_matrix[3], FLOAT alpha, FLOAT theta);
    void compute_matrix_gamma_abg(FLOAT3 rotate_matrix[3], FLOAT3 atg);
    void compute_matrix_gamma_gba(FLOAT3 rotate_matrix[3], FLOAT3 atg);
    void set_rotation_matrix(FLOAT3 *matrix, const FLOAT3 &v1, const FLOAT3 &v2);
    void add_rotated_vec(FLOAT3 &vec, const FLOAT3 matrix[3], FLOAT3 dir);
}

RTX_NAMESPACE_END

std::ostream &operator <<(std::ostream &out, const FLOAT3 &p);
std::ostream &operator <<(std::ostream &out, const COMPLEX &p);

#endif /* STRUCT_HPP */
