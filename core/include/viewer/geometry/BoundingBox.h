#pragma once

#include <cmath>

#include <viewer/geometry/Vec3.h>

namespace viewer::geometry {

// Axis-aligned bounding box: the minimum and maximum corners of an extent.
struct BoundingBox {
    Vec3 min;
    Vec3 max;

    Vec3 center() const
    {
        return Vec3{
            (min.x + max.x) / 2.0,
            (min.y + max.y) / 2.0,
            (min.z + max.z) / 2.0};
    }

    double diagonal() const
    {
        const double dx = max.x - min.x;
        const double dy = max.y - min.y;
        const double dz = max.z - min.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
};

}  // namespace viewer::geometry
