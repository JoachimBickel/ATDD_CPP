#pragma once

#include <viewer/ports/View.h>

namespace viewer::qt {

class InfoPanel;
class ViewportWidget;

// Qt implementation of the View outbound port: routes what the core wants
// shown to the viewport and the info panel. Translation only — no logic.
class QtView : public viewer::ports::View {
public:
    QtView(ViewportWidget& viewport, InfoPanel& infoPanel);

    void showModel(const viewer::geometry::Mesh& mesh) override;
    void showModelInfo(const viewer::ports::ModelInfo& info) override;
    void showCamera(const viewer::ports::CameraState& camera) override;

private:
    ViewportWidget& viewport_;
    InfoPanel& infoPanel_;
};

}  // namespace viewer::qt