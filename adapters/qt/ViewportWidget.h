#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>

#include <viewer/geometry/Mesh.h>
#include <viewer/ports/CameraState.h>

namespace viewer::qt {

// The 3D viewport: renders the mesh pushed by the core, flat-shaded, with the
// view/projection built from the core's CameraState via Qt's matrix functions
// (lookAt/perspective stay in the adapter by design). Interaction in stage 3.
class ViewportWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit ViewportWidget(QWidget* parent = nullptr);
    ~ViewportWidget() override;

    void setMesh(const viewer::geometry::Mesh& mesh);
    void setCamera(const viewer::ports::CameraState& camera);

protected:
    void initializeGL() override;
    void paintGL() override;

private:
    void uploadMesh();

    viewer::geometry::Mesh mesh_;
    viewer::ports::CameraState camera_;

    QOpenGLShaderProgram program_;
    QOpenGLBuffer vertexBuffer_{QOpenGLBuffer::VertexBuffer};
    QOpenGLVertexArrayObject vao_;
    int uploadedVertexCount_ = 0;
    bool meshDirty_ = false;
};

}  // namespace viewer::qt