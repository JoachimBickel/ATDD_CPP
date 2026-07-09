#pragma once

namespace viewer::geometry {

// A point or direction in 3D space. Plain value type — no Qt, no GPU concerns.
struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

inline Vec3 operator+(const Vec3& a, const Vec3& b)
{
    return Vec3{a.x + b.x, a.y + b.y, a.z + b.z};
}

inline Vec3 operator-(const Vec3& a, const Vec3& b)
{
    return Vec3{a.x - b.x, a.y - b.y, a.z - b.z};
}

inline Vec3 operator*(const Vec3& v, double scalar)
{
    return Vec3{v.x * scalar, v.y * scalar, v.z * scalar};
}

}  // namespace viewer::geometry
