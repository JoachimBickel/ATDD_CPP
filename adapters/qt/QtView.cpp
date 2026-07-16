#include "QtView.h"

#include "InfoPanel.h"
#include "ViewportWidget.h"

namespace viewer::qt {

QtView::QtView(ViewportWidget& viewport, InfoPanel& infoPanel)
    : viewport_(viewport), infoPanel_(infoPanel)
{
}

void QtView::showModel(const viewer::geometry::Mesh& mesh)
{
    viewport_.setMesh(mesh);
}

void QtView::showModelInfo(const viewer::ports::ModelInfo& info)
{
    infoPanel_.showInfo(info);
}

void QtView::showCamera(const viewer::ports::CameraState& camera)
{
    viewport_.setCamera(camera);
}

}  // namespace viewer::qt