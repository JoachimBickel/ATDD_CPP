#pragma once

#include <cmath>

#include <viewer/geometry/Vec3.h>
#include <viewer/ports/CameraState.h>

namespace viewer::app {

// Rotates v by angle radians about the unit-length axis (Rodrigues' formula),
// counter-clockwise when looking down the axis towards the origin.
inline geometry::Vec3 rotateAbout(
    const geometry::Vec3& v, const geometry::Vec3& axis, const double angle)
{
    const double cosAngle = std::cos(angle);
    const double sinAngle = std::sin(angle);
    return v * cosAngle + geometry::cross(axis, v) * sinAngle
        + axis * (geometry::dot(axis, v) * (1.0 - cosAngle));
}

// Turntable orbit around the target: yaw turns the eye about the world's
// vertical axis, then pitch tilts it about the camera's sideways axis
// (positive pitch raises the eye). Up turns and tilts along with the eye, so
// it stays at right angles to the view and the orbit can pass over the top;
// turning about the world's vertical keeps the sideways axis level, so the
// view never rolls. While upside down the turn is reversed, so the model still
// follows the mouse; exactly overhead counts as upright. The target is
// unchanged.
inline ports::CameraState orbitCamera(
    const ports::CameraState& camera, const double yaw, const double pitch)
{
    const geometry::Vec3 worldUp{0.0, 1.0, 0.0};
    const double turn = camera.up.y < 0.0 ? -yaw : yaw;
    geometry::Vec3 offset = rotateAbout(camera.eye - camera.target, worldUp, turn);
    geometry::Vec3 up = rotateAbout(camera.up, worldUp, turn);

    const geometry::Vec3 sideways = geometry::normalized(geometry::cross(offset, up));
    offset = rotateAbout(offset, sideways, pitch);
    up = rotateAbout(up, sideways, pitch);

    ports::CameraState orbited = camera;
    orbited.eye = camera.target + offset;
    orbited.up = up;
    return orbited;
}

}  // namespace viewer::app
