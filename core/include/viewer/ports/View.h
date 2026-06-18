#pragma once

#include <viewer/geometry/Mesh.h>

namespace viewer::ports {

// Outbound port: how the core asks the UI to display things. Implemented by the
// Qt adapter; faked in tests. Speaks only in core/domain types (never Qt types).
class View {
public:
    virtual ~View() = default;
    virtual void showModel(const viewer::geometry::Mesh& mesh) = 0;
};

}  // namespace viewer::ports
