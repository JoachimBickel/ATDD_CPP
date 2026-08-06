#pragma once

#include <functional>
#include <string>

#include <QMainWindow>

namespace viewer::qt {

class InfoPanel;
class ViewportWidget;

// The application shell: viewport in the center, info panel docked right,
// File -> Open. Forwards the chosen path to the open handler wired in main().
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

    ViewportWidget& viewport() { return *viewport_; }
    InfoPanel& infoPanel() { return *infoPanel_; }

    void setOpenHandler(std::function<void(const std::string&)> handler);

private:
    void chooseAndOpenModel();

    ViewportWidget* viewport_;
    InfoPanel* infoPanel_;
    std::function<void(const std::string&)> openHandler_;
};

}  // namespace viewer::qt
