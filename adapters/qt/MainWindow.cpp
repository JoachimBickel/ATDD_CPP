#include "MainWindow.h"

#include <utility>

#include <QDockWidget>
#include <QFileDialog>
#include <QMenuBar>

#include "InfoPanel.h"
#include "ViewportWidget.h"

namespace viewer::qt {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      viewport_(new ViewportWidget(this)),
      infoPanel_(new InfoPanel(this))
{
    setWindowTitle(tr("3D Viewer"));
    setCentralWidget(viewport_);

    auto* dock = new QDockWidget(tr("Model Info"), this);
    dock->setWidget(infoPanel_);
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::RightDockWidgetArea, dock);

    auto* fileMenu = menuBar()->addMenu(tr("&File"));
    auto* openAction = fileMenu->addAction(tr("&Open…"));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, &MainWindow::chooseAndOpenModel);
}

void MainWindow::setOpenHandler(std::function<void(const std::string&)> handler)
{
    openHandler_ = std::move(handler);
}

void MainWindow::chooseAndOpenModel()
{
    const QString path = QFileDialog::getOpenFileName(
        this, tr("Open Model"), QString(), tr("Wavefront OBJ (*.obj)"));
    if (!path.isEmpty() && openHandler_) {
        openHandler_(path.toStdString());
    }
}

}  // namespace viewer::qt
