#pragma once

#include <viewer/geometry/Vec3.h>

namespace viewer::geometry {

// Axis-aligned bounding box: the minimum and maximum corners of an extent.
struct BoundingBox {
    Vec3 min;
    Vec3 max;
};

}  // namespace viewer::geometry
