#include <viewer/io/ObjImporter.h>

#include <cstddef>
#include <sstream>
#include <string>

#include <viewer/geometry/Vec3.h>

namespace viewer::io {

viewer::geometry::Mesh ObjImporter::parse(const std::string& text) const
{
    viewer::geometry::Mesh mesh;

    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line)) {
        std::istringstream lineStream(line);
        std::string kind;
        lineStream >> kind;

        if (kind == "v") {
            viewer::geometry::Vec3 vertex;
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            mesh.addVertex(vertex);
        } else if (kind == "f") {
            std::size_t a = 0;
            std::size_t b = 0;
            std::size_t c = 0;
            lineStream >> a >> b >> c;
            // OBJ indices are 1-based; the mesh stores them 0-based.
            mesh.addTriangle({a - 1, b - 1, c - 1});
        }
    }

    return mesh;
}

}  // namespace viewer::io
