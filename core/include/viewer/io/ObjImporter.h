#pragma once

#include <string>

#include <viewer/geometry/Mesh.h>

namespace viewer::io {

// Parses Wavefront OBJ text into a Mesh. Pure core logic: works on in-memory
// text, so it is fully testable without touching the filesystem.
class ObjImporter {
public:
    viewer::geometry::Mesh parse(const std::string& text) const;
};

}  // namespace viewer::io
