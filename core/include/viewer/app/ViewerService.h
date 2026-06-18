#pragma once

#include <string>

#include <viewer/geometry/Mesh.h>
#include <viewer/io/ObjImporter.h>

namespace viewer::ports {
class ModelSource;
class View;
}  // namespace viewer::ports

namespace viewer::app {

// Inbound port / use case: the actions the UI can drive. Reads a model via the
// ModelSource port, parses it, and presents it through the View port.
class ViewerService {
public:
    ViewerService(viewer::ports::ModelSource& source, viewer::ports::View& view);

    void openModel(const std::string& path);

private:
    viewer::ports::ModelSource& source_;
    viewer::ports::View& view_;
    viewer::io::ObjImporter importer_;
    viewer::geometry::Mesh model_;
};

}  // namespace viewer::app
