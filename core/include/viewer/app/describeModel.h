#pragma once

#include <cstddef>
#include <string>

#include <viewer/analysis/BoundingBox.h>
#include <viewer/geometry/Mesh.h>
#include <viewer/ports/ModelInfo.h>

namespace viewer::app {

// The file name without directory or extension. Both '/' and '\' count as
// separators on every platform, unlike std::filesystem on Linux.
inline std::string fileStem(const std::string& path)
{
    const std::size_t separator = path.find_last_of("/\\");
    const std::string fileName =
        separator == std::string::npos ? path : path.substr(separator + 1);
    const std::size_t dot = fileName.find_last_of('.');
    return dot == std::string::npos ? fileName : fileName.substr(0, dot);
}

// Presentation mapping: turns a mesh, and the path it was opened from, into the
// view-facing summary shown in the info panel. Grows as more metrics are
// surfaced; kept out of ViewerService so that use case stays pure orchestration.
inline ports::ModelInfo describeModel(const geometry::Mesh& mesh, const std::string& path)
{
    ports::ModelInfo info;
    info.name = mesh.name().empty() ? fileStem(path) : mesh.name();
    info.vertexCount = mesh.vertexCount();
    info.triangleCount = mesh.triangleCount();
    info.bounds = analysis::boundingBox(mesh);
    return info;
}

}  // namespace viewer::app
