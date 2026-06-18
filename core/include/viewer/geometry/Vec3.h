#pragma once

namespace viewer::geometry {

// A point or direction in 3D space. Plain value type — no Qt, no GPU concerns.
struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

}  // namespace viewer::geometry
