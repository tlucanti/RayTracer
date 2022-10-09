
# include <objects.h>
# include <ostream>

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
