#pragma once

#include <viewer/analysis/BoundingBox.h>
#include <viewer/geometry/Mesh.h>
#include <viewer/geometry/Vec3.h>
#include <viewer/ports/CameraState.h>

namespace viewer::app {

// Positions the camera to frame a model: it targets the bounding-box center and
// sits back along +Z by the box diagonal (a simple, fov-independent fit), Y up.
inline ports::CameraState frameModel(const geometry::Mesh& mesh)
{
    const geometry::BoundingBox bounds = analysis::boundingBox(mesh);
    const geometry::Vec3 center = bounds.center();
    const double distance = bounds.diagonal();

    ports::CameraState camera;
    camera.target = center;
    camera.eye = center + geometry::Vec3{0.0, 0.0, distance};
    camera.up = geometry::Vec3{0.0, 1.0, 0.0};
    return camera;
}

}  // namespace viewer::app
