#pragma once

#include <cmath>

#include <viewer/geometry/Vec3.h>
#include <viewer/ports/CameraState.h>

namespace viewer::app {

// Rotates the eye around the target about the world-up (Y) axis by yaw radians
// (a turntable orbit). Target and up are unchanged.
inline viewer::ports::CameraState orbitCamera(const viewer::ports::CameraState& camera,
                                              double yaw)
{
    const viewer::geometry::Vec3 offset = camera.eye - camera.target;
    const double cosYaw = std::cos(yaw);
    const double sinYaw = std::sin(yaw);

    const viewer::geometry::Vec3 rotated{
        offset.x * cosYaw + offset.z * sinYaw,
        offset.y,
        -offset.x * sinYaw + offset.z * cosYaw};

    viewer::ports::CameraState orbited = camera;
    orbited.eye = camera.target + rotated;
    return orbited;
}

}  // namespace viewer::app
