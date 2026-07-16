#include "ViewportWidget.h"

#include <cmath>
#include <utility>
#include <vector>

#include <QMatrix4x4>
#include <QMouseEvent>
#include <QVector3D>
#include <QWheelEvent>

#include <viewer/geometry/Vec3.h>

namespace viewer::qt {
namespace {

const char* kVertexShader = R"(
    #version 330 core
    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;
    uniform mat4 mvp;
    out vec3 fragNormal;
    void main() {
        fragNormal = normal;
        gl_Position = mvp * vec4(position, 1.0);
    }
)";

// Headlight shading: brightness follows the angle between the face normal and
// the view direction, so form is readable from any camera position.
const char* kFragmentShader = R"(
    #version 330 core
    in vec3 fragNormal;
    uniform vec3 viewDirection;
    out vec4 fragColor;
    void main() {
        float lit = abs(dot(normalize(fragNormal), viewDirection));
        vec3 base = vec3(0.55, 0.65, 0.80);
        fragColor = vec4(base * (0.25 + 0.75 * lit), 1.0);
    }
)";

QVector3D toQt(const viewer::geometry::Vec3& v)
{
    return QVector3D(static_cast<float>(v.x), static_cast<float>(v.y), static_cast<float>(v.z));
}

}  // namespace

ViewportWidget::ViewportWidget(QWidget* parent) : QOpenGLWidget(parent) {}

ViewportWidget::~ViewportWidget()
{
    // GL resources need the context current when they are destroyed.
    makeCurrent();
    vertexBuffer_.destroy();
    vao_.destroy();
    doneCurrent();
}

void ViewportWidget::setMesh(const viewer::geometry::Mesh& mesh)
{
    mesh_ = mesh;
    meshDirty_ = true;
    update();
}

void ViewportWidget::setCamera(const viewer::ports::CameraState& camera)
{
    camera_ = camera;
    update();
}

void ViewportWidget::setOrbitHandler(std::function<void(double)> handler)
{
    orbitHandler_ = std::move(handler);
}

void ViewportWidget::setZoomHandler(std::function<void(double)> handler)
{
    zoomHandler_ = std::move(handler);
}

void ViewportWidget::mousePressEvent(QMouseEvent* event)
{
    lastMousePos_ = event->pos();
}

void ViewportWidget::mouseMoveEvent(QMouseEvent* event)
{
    const QPoint delta = event->pos() - lastMousePos_;
    lastMousePos_ = event->pos();

    if ((event->buttons() & Qt::LeftButton) && orbitHandler_) {
        // Translation only: pixels -> radians; the camera logic lives in the core.
        // Negative: dragging right turns the model's right side away from you.
        constexpr double radiansPerPixel = -0.005;
        orbitHandler_(delta.x() * radiansPerPixel);
    }
}

void ViewportWidget::wheelEvent(QWheelEvent* event)
{
    if (zoomHandler_) {
        // One 15-degree wheel notch (angleDelta 120) scales the distance by 0.9.
        const double notches = event->angleDelta().y() / 120.0;
        zoomHandler_(std::pow(0.9, notches));
    }
}

void ViewportWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.16f, 0.17f, 0.20f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    program_.addShaderFromSourceCode(QOpenGLShader::Vertex, kVertexShader);
    program_.addShaderFromSourceCode(QOpenGLShader::Fragment, kFragmentShader);
    program_.link();

    vao_.create();
    vertexBuffer_.create();
}

// De-index the mesh into a triangle soup with per-face (flat) normals:
// position xyz + normal xyz per vertex. Flat shading keeps the adapter simple;
// per-vertex normals in the core are on the workshop backlog.
void ViewportWidget::uploadMesh()
{
    const auto& vertices = mesh_.vertices();
    std::vector<float> data;
    data.reserve(mesh_.triangleCount() * 3 * 6);

    for (const auto& triangle : mesh_.triangles()) {
        const QVector3D a = toQt(vertices[triangle.v0]);
        const QVector3D b = toQt(vertices[triangle.v1]);
        const QVector3D c = toQt(vertices[triangle.v2]);
        const QVector3D normal = QVector3D::normal(a, b, c);

        for (const QVector3D& corner : {a, b, c}) {
            data.push_back(corner.x());
            data.push_back(corner.y());
            data.push_back(corner.z());
            data.push_back(normal.x());
            data.push_back(normal.y());
            data.push_back(normal.z());
        }
    }

    vao_.bind();
    vertexBuffer_.bind();
    vertexBuffer_.allocate(data.data(), static_cast<int>(data.size() * sizeof(float)));

    program_.bind();
    program_.enableAttributeArray(0);
    program_.setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
    program_.enableAttributeArray(1);
    program_.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
    program_.release();

    vertexBuffer_.release();
    vao_.release();

    uploadedVertexCount_ = static_cast<int>(mesh_.triangleCount() * 3);
    meshDirty_ = false;
}

void ViewportWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (meshDirty_) {
        uploadMesh();
    }
    if (uploadedVertexCount_ == 0) {
        return;
    }

    const QVector3D eye = toQt(camera_.eye);
    const QVector3D target = toQt(camera_.target);
    const QVector3D up = toQt(camera_.up);

    // The adapter's one piece of camera work: turn the core's CameraState into
    // matrices using the framework's own, well-tested functions.
    const float distance = (eye - target).length();
    const float aspect =
        height() > 0 ? static_cast<float>(width()) / static_cast<float>(height()) : 1.0f;

    QMatrix4x4 view;
    view.lookAt(eye, target, up);
    QMatrix4x4 projection;
    projection.perspective(45.0f, aspect, distance * 0.01f, distance * 100.0f);

    program_.bind();
    program_.setUniformValue("mvp", projection * view);
    program_.setUniformValue("viewDirection", (target - eye).normalized());

    vao_.bind();
    glDrawArrays(GL_TRIANGLES, 0, uploadedVertexCount_);
    vao_.release();
    program_.release();
}

}  // namespace viewer::qt