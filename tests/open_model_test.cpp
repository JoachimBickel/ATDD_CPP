#include <boost/test/unit_test.hpp>

#include <string>
#include <utility>

#include <viewer/app/ViewerService.h>
#include <viewer/geometry/Mesh.h>
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
};

}  // namespace

BOOST_AUTO_TEST_SUITE(loading_models)

BOOST_AUTO_TEST_CASE(opening_a_model_displays_it_in_the_view)
{
    FakeModelSource source{
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "f 1 2 3\n"};
    FakeView view;
    viewer::app::ViewerService service{source, view};

    service.openModel("triangle.obj");

    BOOST_TEST(view.modelShown);
    BOOST_TEST(view.shownMesh.vertexCount() == 3u);
    BOOST_TEST(view.shownMesh.triangleCount() == 1u);
}

BOOST_AUTO_TEST_CASE(opening_a_model_shows_its_info)
{
    FakeModelSource source{
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "f 1 2 3\n"};
    FakeView view;
    viewer::app::ViewerService service{source, view};

    service.openModel("triangle.obj");

    BOOST_TEST(view.infoShown);
    BOOST_TEST(view.shownInfo.vertexCount == 3u);
    BOOST_TEST(view.shownInfo.triangleCount == 1u);
}

BOOST_AUTO_TEST_SUITE_END()
