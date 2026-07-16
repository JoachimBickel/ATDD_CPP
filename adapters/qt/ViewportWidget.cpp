#include "ViewportWidget.h"

namespace viewer::qt {

ViewportWidget::ViewportWidget(QWidget* parent) : QOpenGLWidget(parent) {}

void ViewportWidget::setMesh(const viewer::geometry::Mesh& mesh)
{
    mesh_ = mesh;
    update();
}

void ViewportWidget::setCamera(const viewer::ports::CameraState& camera)
{
    camera_ = camera;
    update();
}

void ViewportWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.16f, 0.17f, 0.20f, 1.0f);
}

void ViewportWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

}  // namespace viewer::qt