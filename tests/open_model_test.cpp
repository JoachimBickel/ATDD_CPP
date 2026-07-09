#include <boost/test/unit_test.hpp>

#include <cmath>
#include <string>
#include <utility>

#include <viewer/app/ViewerService.h>
#include <viewer/geometry/Mesh.h>
#include <viewer/ports/CameraState.h>
#include <viewer/ports/ModelInfo.h>
#include <viewer/ports/ModelSource.h>
#include <viewer/ports/View.h>

namespace {

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
// was shown. The single home for wiring the fakes to the service.
FakeView openModelWith(std::string objText)
{
    FakeModelSource source{std::move(objText)};
    FakeView view;
    viewer::app::ViewerService service{source, view};
    service.openModel("model.obj");
    return view;
}

// The one-triangle model reused across the loading cases.
struct OpenedTriangleModel {
    FakeView view = openModelWith(
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "f 1 2 3\n");
};

}  // namespace

BOOST_AUTO_TEST_SUITE(loading_models)

BOOST_FIXTURE_TEST_CASE(opening_a_model_displays_it_in_the_view, OpenedTriangleModel)
{
    BOOST_TEST(view.modelShown);
    BOOST_TEST(view.shownMesh.vertexCount() == 3u);
    BOOST_TEST(view.shownMesh.triangleCount() == 1u);
}

BOOST_FIXTURE_TEST_CASE(opening_a_model_shows_its_info, OpenedTriangleModel)
{
    BOOST_TEST(view.infoShown);
    BOOST_TEST(view.shownInfo.vertexCount == 3u);
    BOOST_TEST(view.shownInfo.triangleCount == 1u);
}

BOOST_FIXTURE_TEST_CASE(opening_a_model_shows_its_bounding_box, OpenedTriangleModel)
{
    BOOST_TEST(view.shownInfo.bounds.min.x == 0.0);
    BOOST_TEST(view.shownInfo.bounds.min.y == 0.0);
    BOOST_TEST(view.shownInfo.bounds.min.z == 0.0);
    BOOST_TEST(view.shownInfo.bounds.max.x == 1.0);
    BOOST_TEST(view.shownInfo.bounds.max.y == 1.0);
    BOOST_TEST(view.shownInfo.bounds.max.z == 0.0);
}

BOOST_FIXTURE_TEST_CASE(opening_a_model_frames_it, OpenedTriangleModel)
{
    BOOST_TEST(view.cameraShown);
    BOOST_TEST(view.shownCamera.target.x == 0.5);
    BOOST_TEST(view.shownCamera.target.y == 0.5);
    BOOST_TEST(view.shownCamera.target.z == 0.0);
}

BOOST_FIXTURE_TEST_CASE(framing_places_the_camera_back_from_the_target, OpenedTriangleModel)
{
    BOOST_TEST(view.shownCamera.up.x == 0.0);
    BOOST_TEST(view.shownCamera.up.y == 1.0);
    BOOST_TEST(view.shownCamera.up.z == 0.0);

    BOOST_TEST(view.shownCamera.eye.x == 0.5);
    BOOST_TEST(view.shownCamera.eye.y == 0.5);
    BOOST_TEST(view.shownCamera.eye.z == std::sqrt(2.0), boost::test_tools::tolerance(1e-9));
}

BOOST_AUTO_TEST_CASE(zooming_in_moves_the_eye_toward_the_target)
{
    FakeModelSource source{
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "f 1 2 3\n"};
    FakeView view;
    viewer::app::ViewerService service{source, view};
    service.openModel("triangle.obj");

    service.zoom(0.5);

    // Framing put the eye at (0.5, 0.5, sqrt(2)) aimed at (0.5, 0.5, 0);
    // zooming by 0.5 halves the eye->target distance, target unchanged.
    BOOST_TEST(view.shownCamera.target.x == 0.5);
    BOOST_TEST(view.shownCamera.target.y == 0.5);
    BOOST_TEST(view.shownCamera.target.z == 0.0);
    BOOST_TEST(view.shownCamera.eye.x == 0.5);
    BOOST_TEST(view.shownCamera.eye.y == 0.5);
    BOOST_TEST(view.shownCamera.eye.z == std::sqrt(2.0) / 2.0, boost::test_tools::tolerance(1e-9));
}

BOOST_AUTO_TEST_CASE(parses_real_world_obj_with_comments_blanks_and_slash_faces)
{
    const FakeView view = openModelWith(
        "# exported by something\n"
        "o triangle\n"
        "\n"
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "\n"
        "# the face\n"
        "f 1/1/1 2/2/2 3/3/3\n");

    BOOST_TEST(view.shownMesh.vertexCount() == 3u);
    BOOST_TEST(view.shownMesh.triangleCount() == 1u);

    const auto& triangle = view.shownMesh.triangles().at(0);
    BOOST_TEST(triangle.v0 == 0u);
    BOOST_TEST(triangle.v1 == 1u);
    BOOST_TEST(triangle.v2 == 2u);
}

BOOST_AUTO_TEST_SUITE_END()
