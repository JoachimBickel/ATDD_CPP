#pragma once

#include <string>
#include <utility>

#include <viewer/app/ViewerService.h>
#include <viewer/geometry/Mesh.h>
#include <viewer/ports/CameraState.h>
#include <viewer/ports/ModelInfo.h>
#include <viewer/ports/ModelSource.h>
#include <viewer/ports/View.h>

namespace testsupport {

// Outbound-port fake: hands back canned file content, no filesystem involved.
class FakeModelSource : public viewer::ports::ModelSource {
public:
    explicit FakeModelSource(std::string content) : content_(std::move(content)) {}
    std::string read(const std::string& /*path*/) override { return content_; }

private:
    std::string content_;
};

// Outbound-port fake: captures what the core asks the UI to display.
class FakeView : public viewer::ports::View {
public:
    bool modelShown = false;
    viewer::geometry::Mesh shownMesh;

    bool infoShown = false;
    viewer::ports::ModelInfo shownInfo;

    bool cameraShown = false;
    viewer::ports::CameraState shownCamera;

    void showModel(const viewer::geometry::Mesh& mesh) override
    {
        modelShown = true;
        shownMesh = mesh;
    }

    void showModelInfo(const viewer::ports::ModelInfo& info) override
    {
        infoShown = true;
        shownInfo = info;
    }

    void showCamera(const viewer::ports::CameraState& camera) override
    {
        cameraShown = true;
        shownCamera = camera;
    }
};

// Arrange + act: open a model from the given OBJ text, returning what the view
// was shown. For one-off content tests that only inspect the view.
inline FakeView openModelWith(std::string objText)
{
    FakeModelSource source{std::move(objText)};
    FakeView view;
    viewer::app::ViewerService service{source, view};
    service.openModel("model.obj");
    return view;
}

// The one-triangle model reused across the loading and interaction cases. Holds
// the live service so tests can drive further commands (zoom/orbit).
struct OpenedTriangleModel {
    FakeModelSource source{
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "f 1 2 3\n"};
    FakeView view;
    viewer::app::ViewerService service{source, view};

    OpenedTriangleModel() { service.openModel("triangle.obj"); }
};

}  // namespace testsupport
