#pragma once

#include <string>

#include <viewer/geometry/Mesh.h>
#include <viewer/io/ObjImporter.h>
#include <viewer/ports/CameraState.h>

namespace viewer::ports {
class ModelSource;
class View;
}  // namespace viewer::ports

namespace viewer::app {

// Inbound port / use case: the actions the UI can drive. Reads a model via the
// ModelSource port, parses it, and presents it through the View port. Holds the
// current camera, framed on open and mutated by interaction commands.
class ViewerService {
public:
    ViewerService(ports::ModelSource& source, ports::View& view);

    void openModel(const std::string& path);
    void zoom(double factor);
    void orbit(double yaw);

private:
    ports::ModelSource& source_;
    ports::View& view_;
    io::ObjImporter importer_;
    geometry::Mesh model_;
    ports::CameraState camera_;
};

}  // namespace viewer::app
