#include "QtView.h"

#include "InfoPanel.h"
#include "ViewportWidget.h"

namespace viewer::qt {

QtView::QtView(ViewportWidget& viewport, InfoPanel& infoPanel)
    : viewport_(viewport), infoPanel_(infoPanel)
{
}

void QtView::showModel(const geometry::Mesh& mesh)
{
    viewport_.setMesh(mesh);
}

void QtView::showModelInfo(const ports::ModelInfo& info)
{
    infoPanel_.showInfo(info);
}

void QtView::showCamera(const ports::CameraState& camera)
{
    viewport_.setCamera(camera);
}

}  // namespace viewer::qt