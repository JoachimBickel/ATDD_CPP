#pragma once

#include <viewer/ports/CameraState.h>

namespace viewer::app {

// Moves the eye along the eye->target line by the given factor (< 1 zooms in,
// > 1 zooms out). Target and up are unchanged.
inline viewer::ports::CameraState zoomCamera(const viewer::ports::CameraState& camera,
                                             double factor)
{
    viewer::ports::CameraState zoomed = camera;
    zoomed.eye.x = camera.target.x + (camera.eye.x - camera.target.x) * factor;
    zoomed.eye.y = camera.target.y + (camera.eye.y - camera.target.y) * factor;
    zoomed.eye.z = camera.target.z + (camera.eye.z - camera.target.z) * factor;
    return zoomed;
}

}  // namespace viewer::app
