#pragma once

#include <viewer/ports/View.h>

namespace viewer::qt {

class MainWindow;

// Qt implementation of the View outbound port: routes what the core wants
// shown to the viewport, the info panel, and the window title. Translation
// only — no logic.
class QtView : public ports::View {
public:
    explicit QtView(MainWindow& window);

    void showModel(const geometry::Mesh& mesh) override;
    void showModelInfo(const ports::ModelInfo& info) override;
    void showCamera(const ports::CameraState& camera) override;

private:
    MainWindow& window_;
};

}  // namespace viewer::qt
