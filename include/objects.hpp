
# include <objects.h>
# include <ostream>

struct empty_string
{
    friend std::ostream &operator <<(std::ostream &out, const empty_string &)
    {
        return out;
    }
};

static inline constexpr FLOAT _fabs(FLOAT q) { return q > 0 ? q : -q; }

static inline constexpr FLOAT _flt0(FLOAT q) { return _fabs(q) < EPS ? 0 : q; }

static inline empty_string _clprt(const FLOAT3 &c, std::ostream &out)
{
    out << c.x << ':' << c.y << ':' << c.z;
    return {};
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
    return light_t(DIRECT, intensity, color, direction);
}

// -----------------------------------------------------------------------------
inline
light_t point_t(FLOAT intensity, const FLOAT3 &color, const FLOAT3 &position)
{
    return light_t(POINT, intensity, color, position);
}

// -----------------------------------------------------------------------------
template <class obj_type>
empty_string _SRRTE(const obj_type &obj, std::ostream &out)
{
    out << "S:R:R:T:E[" << obj.specular
        << ':' << _flt0(obj.reflective)
        << ':' << _flt0(obj.refractive)
        << ':' << _flt0(obj.transparency)
        << ':' << _flt0(obj.emission)
        << ']';
    return {};
}

// -----------------------------------------------------------------------------
template <class obj_type>
empty_string _SRRT(const obj_type &obj, std::ostream &out)
{
    out << "S:R:R:T[" << obj.specular
    << ':' << _flt0(obj.reflective)
    << ':' << _flt0(obj.refractive)
    << ':' << _flt0(obj.transparency)
    << ']';
    return {};
}

// -----------------------------------------------------------------------------
inline
std::ostream &operator <<(std::ostream &out, const sphere_t &sp)
{
    out << "sphere[pos: " << sp.position << ", R: " << _flt0(sp.radius)
        << ", color: " << _clprt(sp.color, out) << ", "
        << _SRRTE(sp, out) << " ]";
    return out;
}

// -----------------------------------------------------------------------------
inline
std::ostream &operator <<(std::ostream &out, const plane_t &pl)
{
    out << "plane[point: " << pl.position << ", normal: " << pl.normal
        << ", color: " << _clprt(pl.color, out) << ", "
        << _SRRT(pl, out) << " ]";
    return out;
}

// -----------------------------------------------------------------------------
inline
std::ostream &operator <<(std::ostream &out, const triangle_t &tr)
{
    out << "triangle[points: " << tr.a << ", " << tr.b << ", " << tr.c
        << ", color: " << _clprt(tr.color, out) << ", "
        << _SRRTE(tr, out) << " ]";
    return out;
}

// -----------------------------------------------------------------------------
inline
std::ostream &operator <<(std::ostream &out, const cone_t &cn)
{
    out << "cone[pos: " << cn.position << ", dir: " << cn.direction << ", W:G["
        << cn.width << ':' << cn.gamma << "], color: "
        << _clprt(cn.color, out) << ", " << _SRRT(cn, out)
        << " ]";
    return out;
}

// -----------------------------------------------------------------------------
inline
std::ostream &operator <<(std::ostream &out, const cylinder_t &cn)
{
    out << "cylinder[pos: " << cn.position << ", dir: " << cn.direction
        << ", R:H[" << cn.radius << ':' << cn.height << "], color: "
        << _clprt(cn.color, out) << ", " << _SRRT(cn, out)
        << " ]";
    return out;
}

// -----------------------------------------------------------------------------
inline
std::ostream &operator <<(std::ostream &out, const torus_t &to)
{
    out << "torus[pos: " << to.position << ", normal: " << to.normal << "r:R["
        << to.r << ':' << to.R << "], color: " << _clprt(to.color, out)
        << ", " << _SRRT(to, out) << " ]";
    return out;
}

// -----------------------------------------------------------------------------
inline
std::ostream &operator <<(std::ostream &out, const box_t box)
{
    out << "box[pos: " << box.position << ", sides: " << box.sides <<
        ", color: " << _clprt(box.color, out)
        << ", " << _SRRT(box, out) << " ]";
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
    out << "color: " << _clprt(li.color, out) << "]";
    return out;
}

// -----------------------------------------------------------------------------
inline
std::ostream &operator <<(std::ostream &out, const camera_t &cam)
{
    out << "camera[pos: " << cam.position << ", dir: " << cam.direction << "]";
    return out;
}
