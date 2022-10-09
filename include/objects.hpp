
# include <objects.h>
# include <ostream>

static inline constexpr FLOAT _flt0(FLOAT q) { return fabs(q) < EPS ? 0 : q; }
static inline std::string _clprt(const FLOAT3 &c)
{
    return (std::stringstream() << c.x << ':' << c.y << ':' << c.z).str();
}

// -----------------------------------------------------------------------------
inline
light_t ambient_t(FLOAT intensity, const FLOAT3 &color)
{
    return light_t(AMBIENT, intensity, color, {{0., 0., 0.}});
}

// -----------------------------------------------------------------------------
inline
light_t direct_t(FLOAT intensity, const FLOAT3 &color, const FLOAT3 &direction)
{
    rtx::linalg::normalize(direction);
    return light_t(DIRECT, intensity, color, direction);
}

// -----------------------------------------------------------------------------
inline
light_t point_t(FLOAT intensity, const FLOAT3 &color, const FLOAT3 &position)
{
    return light_t(POINT, intensity, color, position);
}

// -----------------------------------------------------------------------------
inline
std::ostream &operator <<(std::ostream &out, const camera_t &cam)
{
    out << "camera[pos: " << cam.position << ", dir: " << cam.direction << "]";
    return out;
}

// -----------------------------------------------------------------------------
inline
std::ostream &operator <<(std::ostream &out, const sphere_t &sp)
{
    out << "sphere[pos: " << sp.position << ", R: " << _flt0(sp.radius)
        << ", color: " << _clprt(sp.color) << ", S:R[" << sp.specular
        << ':' << _flt0(sp.reflective) << ']';
    return out;
}

// -----------------------------------------------------------------------------
inline
std::ostream &operator <<(std::ostream &out, const plane_t &pl)
{
    out << "plane[point: " << pl.point << ", normal: " << pl.normal
        << ", color: " << _clprt(pl.color) << ", S:R["
        << pl.specular << ':' << _flt0(pl.reflective) << "] ]";
    return out;
}

// -----------------------------------------------------------------------------
inline
std::ostream &operator <<(std::ostream &out, const light_t &li)
{
    switch (li.type)
    {
        case AMBIENT: out << "ambient_light["; break ;
        case DIRECT: out << "direct_light[dir: " << li.direction << ", "; break ;
        case POINT: out << "point_light[pos: " << li.position << ", "; break ;
    }
    out << "color: " << _clprt(li.color) << ']';
    return out;
}

