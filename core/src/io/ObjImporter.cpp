#include <viewer/io/ObjImporter.h>

#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

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
            std::vector<std::size_t> corners;
            std::string token;
            while (lineStream >> token) {
                // A face vertex is "v", "v/vt", "v/vt/vn" or "v//vn"; keep only v.
                const std::size_t slash = token.find('/');
                const std::string vertexRef =
                    (slash == std::string::npos) ? token : token.substr(0, slash);
                // OBJ indices are 1-based; the mesh stores them 0-based.
                corners.push_back(static_cast<std::size_t>(std::stoul(vertexRef)) - 1);
            }
            if (corners.size() >= 3) {
                mesh.addTriangle({corners[0], corners[1], corners[2]});
            }
        }
    }

    return mesh;
}

}  // namespace viewer::io
