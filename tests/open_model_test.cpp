#include <boost/test/unit_test.hpp>

#include <cmath>
#include <string>

#include <viewer/geometry/Mesh.h>
#include <viewer/ports/CameraState.h>
#include <viewer/ports/ModelInfo.h>

#include "support/fakes.h"

using namespace testsupport;

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

BOOST_AUTO_TEST_CASE(opening_a_model_shows_its_object_name)
{
    const FakeView view = openModelWith(
        "o cube\n"
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "f 1 2 3\n");

    BOOST_TEST(view.shownInfo.name == "cube");
}

BOOST_AUTO_TEST_CASE(a_model_without_an_object_name_is_named_after_its_file)
{
    const FakeView view = openModelWith(
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "f 1 2 3\n",
        "models/teapot.obj");

    BOOST_TEST(view.shownInfo.name == "teapot");
}

BOOST_AUTO_TEST_CASE(forward_and_backward_slashes_give_the_same_file_name)
{
    const std::string unnamedTriangle =
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "f 1 2 3\n";

    // On Linux, std::filesystem does not treat a backslash as a separator, so
    // the rule has to live in the core rather than in a library call.
    BOOST_TEST(openModelWith(unnamedTriangle, "C:/models/teapot.obj").shownInfo.name == "teapot");
    BOOST_TEST(openModelWith(unnamedTriangle, "C:\\models\\teapot.obj").shownInfo.name == "teapot");
}

BOOST_AUTO_TEST_CASE(the_first_of_several_object_names_is_used)
{
    const FakeView view = openModelWith(
        "o body\n"
        "v 0 0 0\n"
        "v 1 0 0\n"
        "v 0 1 0\n"
        "f 1 2 3\n"
        "o lid\n"
        "v 0 0 1\n"
        "f 1 2 4\n");

    BOOST_TEST(view.shownInfo.name == "body");
}

BOOST_AUTO_TEST_SUITE_END()
