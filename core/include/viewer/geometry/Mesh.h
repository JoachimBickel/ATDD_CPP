#pragma once

#include <cstddef>
#include <vector>

#include <viewer/geometry/Vec3.h>

namespace viewer::geometry {

// Three vertex indices forming a triangular face.
struct Triangle {
    std::size_t v0 = 0;
    std::size_t v1 = 0;
    std::size_t v2 = 0;
};

// Indexed triangle mesh: a list of vertices and the triangles referencing them.
class Mesh {
public:
    void addVertex(const Vec3& vertex) { vertices_.push_back(vertex); }
    void addTriangle(const Triangle& triangle) { triangles_.push_back(triangle); }

    [[nodiscard]] std::size_t vertexCount() const { return vertices_.size(); }
    [[nodiscard]] std::size_t triangleCount() const { return triangles_.size(); }

    [[nodiscard]] const std::vector<Vec3>& vertices() const { return vertices_; }
    [[nodiscard]] const std::vector<Triangle>& triangles() const { return triangles_; }

private:
    std::vector<Vec3> vertices_;
    std::vector<Triangle> triangles_;
};

}  // namespace viewer::geometry
