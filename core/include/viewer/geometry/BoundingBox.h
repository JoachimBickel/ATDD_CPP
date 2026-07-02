#pragma once

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
};

}  // namespace viewer::geometry
