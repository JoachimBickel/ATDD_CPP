#include <viewer/app/ViewerService.h>

#include <string>

#include <viewer/app/describeModel.h>
#include <viewer/app/frameModel.h>
#include <viewer/app/orbitCamera.h>
#include <viewer/app/zoomCamera.h>
#include <viewer/ports/ModelSource.h>
#include <viewer/ports/View.h>

namespace viewer::app {

ViewerService::ViewerService(ports::ModelSource& source, ports::View& view)
    : source_(source), view_(view)
{
}

void ViewerService::openModel(const std::string& path)
{
    const std::string content = source_.read(path);
    model_ = importer_.parse(content);

    camera_ = frameModel(model_);

    view_.showModel(model_);
    view_.showModelInfo(describeModel(model_, path));
    view_.showCamera(camera_);
}

void ViewerService::zoom(const double factor)
{
    camera_ = zoomCamera(camera_, factor);
    view_.showCamera(camera_);
}

void ViewerService::orbit(const double yaw, const double pitch)
{
    camera_ = orbitCamera(camera_, yaw, pitch);
    view_.showCamera(camera_);
}

}  // namespace viewer::app
