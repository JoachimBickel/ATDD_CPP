#pragma once

#include <viewer/geometry/Vec3.h>

namespace viewer::ports {

// View-facing camera pose. The adapter feeds these three vectors to its
// framework's look-at (and a projection with the viewport aspect) to render;
// no matrix math lives in the core. Part of the View port's data contract.
struct CameraState {
    viewer::geometry::Vec3 eye;
    viewer::geometry::Vec3 target;
    viewer::geometry::Vec3 up;
};

}  // namespace viewer::ports
