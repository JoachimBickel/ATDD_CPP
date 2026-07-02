#include <viewer/app/ViewerService.h>

#include <string>

#include <viewer/app/describeModel.h>
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
    view_.showModelInfo(describeModel(model_));
}

}  // namespace viewer::app
