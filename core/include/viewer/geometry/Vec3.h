#pragma once

#include <cmath>

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

inline Vec3 operator*(const Vec3& v, const double scalar)
{
    return Vec3{v.x * scalar, v.y * scalar, v.z * scalar};
}

inline double dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3& a, const Vec3& b)
{
    return Vec3{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

inline double length(const Vec3& v)
{
    return std::sqrt(dot(v, v));
}

inline Vec3 normalized(const Vec3& v)
{
    return v * (1.0 / length(v));
}

}  // namespace viewer::geometry
