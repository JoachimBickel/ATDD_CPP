#include <viewer/io/ObjImporter.h>

#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

#include <viewer/geometry/Mesh.h>
#include <viewer/geometry/Vec3.h>

namespace viewer::io {
namespace {

geometry::Vec3 parseVertex(std::istringstream& lineStream)
{
    geometry::Vec3 vertex;
    lineStream >> vertex.x >> vertex.y >> vertex.z;
    return vertex;
}

// Resolves a face line's corners to 0-based vertex indices, accepting the
// "v", "v/vt", "v/vt/vn" and "v//vn" token forms.
std::vector<std::size_t> parseFaceCorners(std::istringstream& lineStream)
{
    std::vector<std::size_t> corners;
    std::string token;
    while (lineStream >> token) {
        const std::size_t slash = token.find('/');
        const std::string vertexRef =
            slash == std::string::npos ? token : token.substr(0, slash);
        // OBJ indices are 1-based; the mesh stores them 0-based.
        corners.push_back(static_cast<std::size_t>(std::stoul(vertexRef)) - 1);
    }
    return corners;
}

}  // namespace

// Deliberately an instance method (not static): parse becomes a virtual on a
// MeshImporter port once more formats (e.g. STL) arrive.
// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
geometry::Mesh ObjImporter::parse(const std::string& text) const
{
    geometry::Mesh mesh;

    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string kind;
        lineStream >> kind;

        if (kind == "v") {
            mesh.addVertex(parseVertex(lineStream));
        } else if (kind == "f") {
            const std::vector<std::size_t> corners = parseFaceCorners(lineStream);
            if (corners.size() >= 3) {
                mesh.addTriangle({corners[0], corners[1], corners[2]});
            }
        }
    }

    return mesh;
}

}  // namespace viewer::io
