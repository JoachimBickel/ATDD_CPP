#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <viewer/geometry/Mesh.h>
#include <viewer/ports/CameraState.h>

namespace viewer::qt {

// The 3D viewport. Stage 1: a cleared surface that stores the mesh and camera
// pushed by the core; actual rendering arrives in stage 2, interaction in 3.
class ViewportWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit ViewportWidget(QWidget* parent = nullptr);

    void setMesh(const viewer::geometry::Mesh& mesh);
    void setCamera(const viewer::ports::CameraState& camera);

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    viewer::geometry::Mesh mesh_;
    viewer::ports::CameraState camera_;
};

}  // namespace viewer::qt