#pragma once

#include <viewer/geometry/Vec3.h>
#include <viewer/ports/CameraState.h>

namespace viewer::app {

// Moves the eye along the eye->target line by the given factor (< 1 zooms in,
// > 1 zooms out). Target and up are unchanged.
inline ports::CameraState zoomCamera(const ports::CameraState& camera, const double factor)
{
    ports::CameraState zoomed = camera;
    zoomed.eye = camera.target + (camera.eye - camera.target) * factor;
    return zoomed;
}

}  // namespace viewer::app
