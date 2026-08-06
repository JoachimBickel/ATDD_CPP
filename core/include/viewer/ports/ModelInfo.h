#pragma once

#include <cstddef>

#include <viewer/geometry/BoundingBox.h>

namespace viewer::ports {

// View-facing summary of the loaded model, shown in the info panel. Part of the
// View port's data contract, so it lives here rather than in the app layer.
struct ModelInfo {
    std::size_t vertexCount = 0;
    std::size_t triangleCount = 0;
    geometry::BoundingBox bounds;
};

}  // namespace viewer::ports
