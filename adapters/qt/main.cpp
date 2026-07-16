#include <QApplication>

#include <viewer/app/ViewerService.h>

#include "FileModelSource.h"
#include "MainWindow.h"
#include "QtView.h"

int main(int argc, char* argv[])
{
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