#pragma once

#include <algorithm>

#include <viewer/geometry/BoundingBox.h>
#include <viewer/geometry/Mesh.h>

namespace viewer::analysis {

// Axis-aligned bounding box of a mesh's vertices. An empty mesh yields a
// zero-sized box at the origin.
inline viewer::geometry::BoundingBox boundingBox(const viewer::geometry::Mesh& mesh)
{
    viewer::geometry::BoundingBox box;

    const auto& vertices = mesh.vertices();
    if (vertices.empty()) {
        return box;
    }

    box.min = vertices.front();
    box.max = vertices.front();
    for (const auto& vertex : vertices) {
        box.min.x = std::min(box.min.x, vertex.x);
        box.min.y = std::min(box.min.y, vertex.y);
        box.min.z = std::min(box.min.z, vertex.z);
        box.max.x = std::max(box.max.x, vertex.x);
        box.max.y = std::max(box.max.y, vertex.y);
        box.max.z = std::max(box.max.z, vertex.z);
    }

    return box;
}

}  // namespace viewer::analysis
