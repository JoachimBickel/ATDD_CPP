#include <QApplication>
#include <QSurfaceFormat>

#include <viewer/app/ViewerService.h>

#include "FileModelSource.h"
#include "MainWindow.h"
#include "QtView.h"

int main(int argc, char* argv[])
{
    // The viewport's shaders need a modern context; request it before any
    // window (and thus GL context) is created.
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    viewer::qt::MainWindow window;
    viewer::qt::FileModelSource source;
    viewer::qt::QtView view{window.viewport(), window.infoPanel()};
    viewer::app::ViewerService service{source, view};

    window.setOpenHandler([&service](const std::string& path) { service.openModel(path); });

    window.resize(1024, 720);
    window.show();
    return app.exec();
}