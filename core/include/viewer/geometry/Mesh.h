#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include <viewer/geometry/Vec3.h>

namespace viewer::geometry {

// Three vertex indices forming a triangular face.
struct Triangle {
    std::size_t v0 = 0;
    std::size_t v1 = 0;
    std::size_t v2 = 0;
};

// Indexed triangle mesh: a list of vertices and the triangles referencing them,
// plus the object name the source file gave it (empty if it gave none).
class Mesh {
public:
    void addVertex(const Vec3& vertex) { vertices_.push_back(vertex); }
    void addTriangle(const Triangle& triangle) { triangles_.push_back(triangle); }
    void setName(std::string name) { name_ = std::move(name); }

    [[nodiscard]] std::size_t vertexCount() const { return vertices_.size(); }
    [[nodiscard]] std::size_t triangleCount() const { return triangles_.size(); }

    [[nodiscard]] const std::vector<Vec3>& vertices() const { return vertices_; }
    [[nodiscard]] const std::vector<Triangle>& triangles() const { return triangles_; }
    [[nodiscard]] const std::string& name() const { return name_; }

private:
    std::string name_;
    std::vector<Vec3> vertices_;
    std::vector<Triangle> triangles_;
};

}  // namespace viewer::geometry
