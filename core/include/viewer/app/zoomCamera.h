#pragma once

#include <viewer/geometry/Vec3.h>
#include <viewer/ports/CameraState.h>

namespace viewer::app {

// Moves the eye along the eye->target line by the given factor (< 1 zooms in,
// > 1 zooms out). Target and up are unchanged.
inline viewer::ports::CameraState zoomCamera(const viewer::ports::CameraState& camera,
                                             double factor)
{
    viewer::ports::CameraState zoomed = camera;
    zoomed.eye = camera.target + (camera.eye - camera.target) * factor;
    return zoomed;
}

}  // namespace viewer::app
