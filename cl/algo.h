
#ifndef CL_ALGO_H
# define CL_ALGO_H

#define SWAP(a, b) do {	\
    FLOAT ___c = a;     \
    (a) = b;            \
    (b) = ___c;         \
} while (false)

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT3 rotate_vector(FLOAT3 vec, __constant const FLOAT3 *matrix)
{
    return ASSIGN_FLOAT3(
        dot(matrix[0], vec),
        dot(matrix[1], vec),
        dot(matrix[2], vec)
    );
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT3 rotate_vector_nonconst(FLOAT3 vec, const FLOAT3 *matrix)
{
    return ASSIGN_FLOAT3(
        dot(matrix[0], vec),
        dot(matrix[1], vec),
        dot(matrix[2], vec)
    );
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT3 reflect_ray(FLOAT3 ray, FLOAT3 normal)
{
    return normal * (2 * dot(ray, normal)) - ray;
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT3 refract_ray(FLOAT3 ray, FLOAT3 normal, FLOAT n12)
{
//    (void)normal;
//    (void)n12;
//    return -ray;
    FLOAT cos_a = dot(normal, ray);
    FLOAT sin_b = n12 * sqrt(1. - cos_a * cos_a);
    FLOAT cos_b = sqrt(1. - sin_b * sin_b);

    FLOAT3 V_n = normal * cos_a;
    FLOAT3 V_p = ray - V_n;

    FLOAT3 R_n = -normal * cos_b;
    FLOAT3 R_p = -V_p * sin_b;

    return R_n + R_p;
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT poly3(FLOAT x, FLOAT a, FLOAT b, FLOAT c, FLOAT d)
/*
	computes ax^3 + bx^2 + cx + d
*/
{
    FLOAT fx = a;
    fx = fx * x + b;
    fx = fx * x + c;
    fx = fx * x + d;
    return fx;
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT newton_cubic_solve(FLOAT x, FLOAT a, FLOAT b, FLOAT c, FLOAT d)
/*
	computes root of cubic equasion ax^3 + bx^2 + cx + d using Newton method,
	starting of `x` point
*/
{
    const FLOAT h = 1e-3;
    const FLOAT h2 = 2. * h;
    const int iter_num = 10;

    for (int i=0; i < iter_num; ++i)
    {
        FLOAT fx = poly3(x, a, b, c, d);
        FLOAT dx = (poly3(x + h, a, b, c, d) - poly3(x - h, a, b, c, d)) / h2;
        x = x - fx / dx;
    }
    return x;
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT cardano_solve(FLOAT b, FLOAT c, FLOAT d)									// TODO: maybe use here FLOAT4
/*
    finds any real root of cubic equation x^3 + bx^2 + cx + d
*/
{
    FLOAT b2 = b * b;

    FLOAT p = (3.*c - b2)/3.;
    FLOAT q = (2.*b2*b - 9.*b*c + 27.*d) / 27.;

    FLOAT Q = p * p * p / 27. + q * q / 4.;
    if (Q < 0.)
    {
        return INFINITY;
        FLOAT discriminant = sqrt(b2 - 3.*c);
        FLOAT x1 = (-b + discriminant) / 3.;
        FLOAT x2 = (-b - discriminant) / 3.;
        return newton_cubic_solve((x1 + x2) / 2., 1., b, c, d);
    }
    Q = sqrt(Q);
    q = -q/2;
    FLOAT alpha = cbrt(q + Q);
    FLOAT beta = cbrt(q - Q);

    FLOAT x = alpha + beta;
    return x - b / 3.;
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT cubic_solve_iterative(FLOAT b, FLOAT c, FLOAT d)
/*
    finds any real root of cubic equation x^3 + bx^2 + cx + d
*/
{
    FLOAT discriminant = b*b - 3.*c;
    if (discriminant < EPS)
        return newton_cubic_solve(0., 1., b, c, d);
    discriminant = sqrt(discriminant);
    FLOAT x1 = (-b + discriminant) / 3.;
    FLOAT x2 = (-b - discriminant) / 3.;
    return newton_cubic_solve((x1 + x2) / 2., 1., b, c, d);
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT cardano_solve_fast(FLOAT b, FLOAT c, FLOAT d)
/*
    finds any real root of cubic equation x^3 + bx^2 + cx + d
*/
{
    FLOAT Q = (3.*c - b*b) / 9.;
    FLOAT R = (9.*b*c - 27.*d - 2.*b*b*b) / 54.;

    Q = Q * Q * Q;
    FLOAT S = cbrt(R + sqrt(Q + R*R));
    FLOAT T = cbrt(R - sqrt(Q + R*R));

    return S + T - b/3.;
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline static
FLOAT _ferrari_solve_quadratic(FLOAT p, FLOAT q)
/*
	helper function for ferrari method
*/
{
    FLOAT x = INFINITY;
    FLOAT discriminant = p*p - 8.*q;
    if (discriminant > EPS)
    {
        discriminant = sqrt(discriminant);
        FLOAT x1 = (-p + discriminant) / 4.;
        FLOAT x2 = (-p - discriminant) / 4.;

        x = fmin(x1, x2);
        if (x < EPS)
            x = fmax(x1, x2);
        if (x < EPS)
            x = INFINITY;
    }
    return x;
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT ferrari_solve(FLOAT b, FLOAT c, FLOAT d, FLOAT e)
/*
    finds minimal positive root of quartic equation x^4 + bx^3 + cx^2 + dx + e
*/
{
    FLOAT y2 = -c;
    FLOAT y1 = b*d - 4.*e;
    FLOAT y0 = 4.*c*e - b*b*e - d*d;

//    FLOAT y = cubic_solve(y2, y1, y0);
    FLOAT y = cardano_solve(y2, y1, y0);

    FLOAT p_sqrt = sqrt(b*b - 4.*c + 4.*y);
    FLOAT q_sqrt = sqrt(y*y - 4.*e);

    FLOAT p = b + p_sqrt;
    FLOAT q = y - q_sqrt;
    FLOAT x1 = _ferrari_solve_quadratic(p, q);

    p = b - p_sqrt;
    q = y + q_sqrt;
    FLOAT x2 = _ferrari_solve_quadratic(p, q);

    FLOAT ret = fmin(x1, x2);
    if (ret < 0)
        ret = fmax(x1, x2);
    return ret;
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT depressed_quartic_solve(FLOAT b, FLOAT c, FLOAT d, FLOAT e)
/*
    finds minimal positive root of quartic equation x^4 + bx^3 + cx^2 + dx + e
*/
{
    FLOAT b2 = b*b;
    FLOAT b3 = b2*b;

    // solving x^4 + bx^3 + cx^2 + dx + e
    FLOAT A = -3.*b2/8. + c;
    FLOAT B = b3/8. - b*c/2. + d;
    FLOAT C = -3.*b3*b/256. + c*b2/16. - b*d/4. + e;
    FLOAT u = -b / 4.;

    A /= 2;
    B /= 2;
    C /= 2;

    // solving x^3 + Ax^2 + Bx + C
    FLOAT y = cardano_solve(-A, -2.*C, A*C - B*B/4.);

    FLOAT s1 = sqrt(2.*y - A);
    FLOAT s2 = 2.*B / s1;
    // x1 = (-s1 + sqrt(-2y - a + s2)) / 2 + u
    // x2 = (-s1 - sqrt(-2y - a + s2)) / 2 + u
    // x3 = (+s1 + sqrt(-2y - a - s2)) / 2 + u
    // x4 = (+s1 - sqrt(-2y - a - s2)) / 2 + u

    FLOAT x1 = (-s1 + sqrt(-2.*y - A + s2) / 2.) + u;
    FLOAT x2 = (-s1 - sqrt(-2.*y - A + s2) / 2.) + u;
    FLOAT x3 = (+s1 + sqrt(-2.*y - A - s2) / 2.) + u;
    FLOAT x4 = (+s1 - sqrt(-2.*y - A - s2) / 2.) + u;
    if (isnan(x1) || x1 < EPS)
        x1 = INFINITY;
    if (isnan(x2) || x2 < EPS)
        x2 = INFINITY;
    if (isnan(x3) || x3 < EPS)
        x3 = INFINITY;
    if (isnan(x4) || x4 < EPS)
        x4 = INFINITY;
    return fmin(fmin(x1, x2), fmin(x3, x4));
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
COMPLEX cmul(COMPLEX a, COMPLEX b)
/*
	computes multiplication of complex numbers
*/
{
    return ASSIGN_COMPLEX(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
COMPLEX cdiv(COMPLEX a, COMPLEX b)
/*
	computes division of complex numbers
*/
{
    FLOAT rlen = 1. / dot(b, b);
    return ASSIGN_COMPLEX(
        dot(a, b) * rlen,
        (a.y * b.x - a.x * b.y) * rlen
    );
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
COMPLEX csqrt(COMPLEX a)
/*
	computes square root of complex numer
*/
{
    FLOAT len_half = length(a) / 2.;
    FLOAT ysign = copysign(1., a.y);
    FLOAT x_half = a.x / 2.;

    return ASSIGN_COMPLEX(
        sqrt(len_half + x_half),
        ysign * sqrt(len_half - x_half)
    );
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT rcbrt(FLOAT a)
/*
	computes algebraic (real) cubic root of numer
*/
{
    return copysign(1., a) * cbrt(fabs(a));
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
void cubic_complex_solve(FLOAT b, FLOAT c, FLOAT d, COMPLEX *y1, COMPLEX *y2)
/*
	finds two solutions of cubic equasion x^3 + bx^2 + cx + d
	if equation has only real roots - returns absolute maximal two values
	else returns only two complex roots
*/
{
    FLOAT b2 = b * b;
    FLOAT b3 = b2 * b;

    FLOAT f = c - b2/3.;
    FLOAT g = 2.*b3/27. - b*c/3. + d;
    FLOAT h = g*g/4. + f*f*f / 27.;

    if (h <= 0)
    {
        FLOAT i = sqrt(g*g/4. - h);
        FLOAT j = cbrt(i);
        FLOAT k = acos(-g / (2.*i));

        FLOAT L = -j;
        FLOAT M = cos(k/3.);
        FLOAT N = sqrt(3.) * sin(k/3.);
        FLOAT P = -b/3.;

        FLOAT x1 = 2 * j * M + P;
        FLOAT x2 = L * (M + N) + P;
        FLOAT x3 = L * (M - N) + P;

        if (fabs(x1) > fabs(x3))
            SWAP(x1, x3);
        if (fabs(x1) > fabs(x2))
            SWAP(x1, x2);

        *y1 = ASSIGN_COMPLEX(x2, 0.);
        *y2 = ASSIGN_COMPLEX(x3, 0.);
    } else {
        h = sqrt(h);
        FLOAT R = -g/2. + h;
        FLOAT T = -g/2. - h;
        FLOAT S = rcbrt(R);
        FLOAT U = rcbrt(T);

        FLOAT sqrt3_2 = sqrt(3.) / 2.;
        FLOAT REAL = (S + U)/-2. - b/3.;
        FLOAT IMAG = (S - U) * sqrt3_2;
        *y1 = ASSIGN_COMPLEX(REAL, IMAG);
        *y2 = ASSIGN_COMPLEX(REAL, -IMAG);
    }
}

// -----------------------------------------------------------------------------
CPP_UNUSED inline
FLOAT quartic_complex_solve(FLOAT b, FLOAT c, FLOAT d, FLOAT e)
/*
    finds minimal positive root of quartic equation x^4 + bx^3 + cx^2 + dx + e
*/
{
    // solving x^4 + bx^3 + cx^2 + dx + e

    FLOAT b2 = b * b;
    FLOAT b3 = b2 * b;
    FLOAT b4 = b3 * b;

    FLOAT f = c - 3.*b2/8.;
    FLOAT g = d + b3/8. - b*c/2.;
    FLOAT h = e - 3.*b4/256. + b2*c/16. - b*d/4.;

    FLOAT B = f/2.;
    FLOAT C = (f*f - 4.*h) / 16.;
    FLOAT D = -g*g / 64.;
    // solving x^3 + Bx^2 + Cx + D

    COMPLEX p, q;
    cubic_complex_solve(B, C, D, &p, &q);
    p = csqrt(p);
    q = csqrt(q);

    COMPLEX r = cdiv(ASSIGN_COMPLEX(-g, 0.), cmul(8. * p, q));
    COMPLEX s = ASSIGN_COMPLEX(b / 4., 0.);

    COMPLEX x1c = p + q + r - s;
    COMPLEX x2c = p - q - r - s;
    COMPLEX x3c = -p + q - r - s;
    COMPLEX x4c = -p - q + r - s;

    FLOAT x1 = x1c.x;
    FLOAT x2 = x2c.x;
    FLOAT x3 = x3c.x;
    FLOAT x4 = x4c.x;
    if (fabs(x1c.y) > EPS || x1c.x < EPS)
        x1 = INFINITY;
    if (fabs(x2c.y) > EPS || x2c.x < EPS)
        x2 = INFINITY;
    if (fabs(x3c.y) > EPS || x3c.x < EPS)
        x3 = INFINITY;
    if (fabs(x4c.y) > EPS || x4c.x < EPS)
        x4 = INFINITY;
    return fmin(fmin(x1, x2), fmin(x3, x4));
}

#endif /* CL_ALGO_H */
