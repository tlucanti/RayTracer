
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

RTX_NAMESPACE_BEGIN

namespace linalg
{
    FLOAT3 cross(const FLOAT3 &a, const FLOAT3 &b);
    FLOAT det(const FLOAT3 &v1, const FLOAT3 &v2, const FLOAT3 &v3);
    FLOAT dot(const FLOAT3 &v1, const FLOAT3 &v2);
    void normalize_ref(FLOAT3 &vec);
    void compute_angles(const FLOAT3 &direction, FLOAT &alpha, FLOAT &theta);
    void compute_angles_gamma(const FLOAT3 &direction, FLOAT3 &atg);
    void compute_matrix(FLOAT3 rotate_matrix[3], FLOAT alpha, FLOAT theta);
    void compute_matrix_gamma(FLOAT3 rotate_matrix[3], FLOAT3 atg);
    void set_rotation_matrix(FLOAT3 *matrix, const FLOAT3 &v1, const FLOAT3 &v2);
    void add_rotated_vec(FLOAT3 &vec, const FLOAT3 matrix[3], FLOAT3 dir);
}

RTX_NAMESPACE_END

std::ostream &operator <<(std::ostream &out, const FLOAT3 &p);

#endif /* STRUCT_HPP */
