#pragma once

#include <functional>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QPoint>

#include <viewer/geometry/Mesh.h>
#include <viewer/ports/CameraState.h>

namespace viewer::qt {

// The 3D viewport: renders the mesh pushed by the core, flat-shaded, with the
// view/projection built from the core's CameraState via Qt's matrix functions
// (lookAt/perspective stay in the adapter by design). Mouse input is only
// translated here — drag/wheel become orbit/zoom commands on the core.
class ViewportWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    explicit ViewportWidget(QWidget* parent = nullptr);
    ~ViewportWidget() override;

    void setMesh(const viewer::geometry::Mesh& mesh);
    void setCamera(const viewer::ports::CameraState& camera);

    void setOrbitHandler(std::function<void(double)> handler);
    void setZoomHandler(std::function<void(double)> handler);

protected:
    void initializeGL() override;
    void paintGL() override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    void uploadMesh();

    viewer::geometry::Mesh mesh_;
    viewer::ports::CameraState camera_;

    QOpenGLShaderProgram program_;
    QOpenGLBuffer vertexBuffer_{QOpenGLBuffer::VertexBuffer};
    QOpenGLVertexArrayObject vao_;
    int uploadedVertexCount_ = 0;
    bool meshDirty_ = false;

    QPoint lastMousePos_;
    std::function<void(double)> orbitHandler_;
    std::function<void(double)> zoomHandler_;
};

}  // namespace viewer::qt