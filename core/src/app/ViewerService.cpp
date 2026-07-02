#include <viewer/app/ViewerService.h>

#include <string>

#include <viewer/analysis/BoundingBox.h>
#include <viewer/ports/ModelInfo.h>
#include <viewer/ports/ModelSource.h>
#include <viewer/ports/View.h>

namespace viewer::app {

ViewerService::ViewerService(viewer::ports::ModelSource& source, viewer::ports::View& view)
    : source_(source), view_(view)
{
}

void ViewerService::openModel(const std::string& path)
{
    const std::string content = source_.read(path);
    model_ = importer_.parse(content);
    view_.showModel(model_);

    viewer::ports::ModelInfo info;
    info.vertexCount = model_.vertexCount();
    info.triangleCount = model_.triangleCount();
    info.bounds = viewer::analysis::boundingBox(model_);
    view_.showModelInfo(info);
}

}  // namespace viewer::app
