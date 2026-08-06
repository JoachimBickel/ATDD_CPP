#pragma once

#include <viewer/analysis/BoundingBox.h>
#include <viewer/geometry/Mesh.h>
#include <viewer/ports/ModelInfo.h>

namespace viewer::app {

// Presentation mapping: turns a mesh into the view-facing summary shown in the
// info panel. Grows as more metrics are surfaced; kept out of ViewerService so
// that use case stays pure orchestration.
inline ports::ModelInfo describeModel(const geometry::Mesh& mesh)
{
    ports::ModelInfo info;
    info.vertexCount = mesh.vertexCount();
    info.triangleCount = mesh.triangleCount();
    info.bounds = analysis::boundingBox(mesh);
    return info;
}

}  // namespace viewer::app
