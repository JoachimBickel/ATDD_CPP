#pragma once

#include <viewer/geometry/Mesh.h>
#include <viewer/ports/CameraState.h>
#include <viewer/ports/ModelInfo.h>

namespace viewer::ports {

// Outbound port: how the core asks the UI to display things. Implemented by the
// Qt adapter; faked in tests. Speaks only in core/domain types (never Qt types).
class View {
public:
    virtual ~View() = default;
    virtual void showModel(const viewer::geometry::Mesh& mesh) = 0;
    virtual void showModelInfo(const ModelInfo& info) = 0;
    virtual void showCamera(const CameraState& camera) = 0;
};

}  // namespace viewer::ports
