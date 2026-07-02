#pragma once

#include <viewer/analysis/BoundingBox.h>
#include <viewer/geometry/Mesh.h>
#include <viewer/ports/CameraState.h>

namespace viewer::app {

// Positions the camera to frame a model: the target is the bounding-box centre.
// eye and up are left at their defaults for now and driven by later cycles.
inline viewer::ports::CameraState frameModel(const viewer::geometry::Mesh& mesh)
{
    const viewer::geometry::BoundingBox bounds = viewer::analysis::boundingBox(mesh);

    viewer::ports::CameraState camera;
    camera.target.x = (bounds.min.x + bounds.max.x) / 2.0;
    camera.target.y = (bounds.min.y + bounds.max.y) / 2.0;
    camera.target.z = (bounds.min.z + bounds.max.z) / 2.0;
    return camera;
}

}  // namespace viewer::app
