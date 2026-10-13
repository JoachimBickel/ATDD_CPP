#include <QApplication>
#include <QSurfaceFormat>

#include <viewer/app/ViewerService.h>

#include "FileModelSource.h"
#include "MainWindow.h"
#include "QtView.h"
#include "ViewportWidget.h"

int main(int argc, char* argv[])
{
    // Only the depth buffer is requested explicitly, before any window (and
    // thus GL context) is created. The viewport's GLSL 3.30 shaders compile in
    // the driver's default context, whereas pinning a 3.3 core profile made
    // context creation fail with EGL_BAD_MATCH on NVIDIA under Wayland.
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    viewer::qt::MainWindow window;
    viewer::qt::FileModelSource source;
    viewer::qt::QtView view{window};
    viewer::app::ViewerService service{source, view};

    window.setOpenHandler([&service](const std::string& path) { service.openModel(path); });
    window.viewport().setOrbitHandler([&service](const double yaw) { service.orbit(yaw); });
    window.viewport().setZoomHandler([&service](const double factor) { service.zoom(factor); });

    window.resize(1024, 720);
    window.show();
    return QApplication::exec();
}
