#include "QtView.h"

#include <QString>

#include "InfoPanel.h"
#include "MainWindow.h"
#include "ViewportWidget.h"

namespace viewer::qt {

QtView::QtView(MainWindow& window) : window_(window) {}

void QtView::showModel(const geometry::Mesh& mesh)
{
    window_.viewport().setMesh(mesh);
}

void QtView::showModelInfo(const ports::ModelInfo& info)
{
    window_.infoPanel().showInfo(info);
    window_.setWindowTitle(
        QStringLiteral("%1 — %2").arg(QString::fromStdString(info.name), MainWindow::tr("3D Viewer")));
}

void QtView::showCamera(const ports::CameraState& camera)
{
    window_.viewport().setCamera(camera);
}

}  // namespace viewer::qt
