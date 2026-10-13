#include <boost/test/unit_test.hpp>

#include <cmath>

#include <viewer/app/ViewerService.h>
#include <viewer/geometry/Vec3.h>
#include <viewer/ports/CameraState.h>

#include "support/fakes.h"

using namespace testsupport;

namespace {

const double pi = std::acos(-1.0);

}  // namespace

BOOST_AUTO_TEST_SUITE(camera_interaction)

BOOST_FIXTURE_TEST_CASE(zooming_in_moves_the_eye_toward_the_target, OpenedTriangleModel)
{
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

BOOST_FIXTURE_TEST_CASE(orbiting_rotates_the_eye_around_the_target, OpenedTriangleModel)
{
    service.orbit(pi / 2.0, 0.0);

    // Framed eye (0.5, 0.5, sqrt(2)) rotated 90 deg about the vertical axis
    // through the target (0.5, 0.5, 0) lands at (0.5 + sqrt(2), 0.5, 0).
    BOOST_TEST(view.shownCamera.eye.x == 0.5 + std::sqrt(2.0), boost::test_tools::tolerance(1e-9));
    BOOST_TEST(view.shownCamera.eye.y == 0.5);
    BOOST_TEST(view.shownCamera.eye.z == 0.0, boost::test_tools::tolerance(1e-9));

    BOOST_TEST(view.shownCamera.target.x == 0.5);
    BOOST_TEST(view.shownCamera.target.y == 0.5);
    BOOST_TEST(view.shownCamera.target.z == 0.0);
}

BOOST_FIXTURE_TEST_CASE(pitching_up_raises_the_eye_and_keeps_its_distance, OpenedTriangleModel)
{
    service.orbit(0.0, pi / 4.0);

    // The framed eye (0.5, 0.5, sqrt(2)) tilted 45 deg up over the target
    // (0.5, 0.5, 0) lands at (0.5, 1.5, 1): still sqrt(2) away.
    BOOST_TEST(view.shownCamera.eye.x == 0.5, boost::test_tools::tolerance(1e-9));
    BOOST_TEST(view.shownCamera.eye.y == 1.5, boost::test_tools::tolerance(1e-9));
    BOOST_TEST(view.shownCamera.eye.z == 1.0, boost::test_tools::tolerance(1e-9));

    BOOST_TEST(view.shownCamera.target.x == 0.5);
    BOOST_TEST(view.shownCamera.target.y == 0.5);
    BOOST_TEST(view.shownCamera.target.z == 0.0);
}

BOOST_FIXTURE_TEST_CASE(orbiting_continues_over_the_top_and_turns_the_view_upside_down, OpenedTriangleModel)
{
    // Two quarter tilts pass straight over the target, where a fixed up
    // direction would leave the camera's sideways direction undefined.
    service.orbit(0.0, pi / 2.0);
    service.orbit(0.0, pi / 2.0);

    // Behind the model at (0.5, 0.5, -sqrt(2)), looking back, upside down.
    BOOST_TEST(view.shownCamera.eye.x == 0.5, boost::test_tools::tolerance(1e-9));
    BOOST_TEST(view.shownCamera.eye.y == 0.5, boost::test_tools::tolerance(1e-9));
    BOOST_TEST(view.shownCamera.eye.z == -std::sqrt(2.0), boost::test_tools::tolerance(1e-9));

    BOOST_TEST(view.shownCamera.up.x == 0.0, boost::test_tools::tolerance(1e-9));
    BOOST_TEST(view.shownCamera.up.y == -1.0, boost::test_tools::tolerance(1e-9));
    BOOST_TEST(view.shownCamera.up.z == 0.0, boost::test_tools::tolerance(1e-9));
}

BOOST_FIXTURE_TEST_CASE(turning_after_tilting_keeps_the_height_and_never_rolls, OpenedTriangleModel)
{
    service.orbit(0.0, pi / 4.0);
    service.orbit(pi / 3.0, 0.0);

    const viewer::geometry::Vec3 offset = view.shownCamera.eye - view.shownCamera.target;
    const viewer::geometry::Vec3 sideways = viewer::geometry::cross(offset, view.shownCamera.up);

    // Left/right turns about the world's vertical: the eye keeps its height,
    // and the camera's sideways direction stays level, so nothing rolls.
    BOOST_TEST(view.shownCamera.eye.y == 1.5, boost::test_tools::tolerance(1e-9));
    BOOST_TEST(sideways.y == 0.0, boost::test_tools::tolerance(1e-9));

    // Up stays at right angles to the viewing direction.
    BOOST_TEST(viewer::geometry::dot(view.shownCamera.up, offset) == 0.0, boost::test_tools::tolerance(1e-9));
}

BOOST_FIXTURE_TEST_CASE(turning_while_upside_down_still_follows_the_mouse, OpenedTriangleModel)
{
    service.orbit(0.0, pi);
    service.orbit(pi / 2.0, 0.0);

    // Upside down behind the model, the same turn lands where it does from the
    // upright front view, (0.5 + sqrt(2), 0.5, 0); unreversed it would land on
    // the opposite side.
    BOOST_TEST(view.shownCamera.eye.x == 0.5 + std::sqrt(2.0), boost::test_tools::tolerance(1e-9));
    BOOST_TEST(view.shownCamera.eye.y == 0.5, boost::test_tools::tolerance(1e-9));
    BOOST_TEST(view.shownCamera.eye.z == 0.0, boost::test_tools::tolerance(1e-9));
}

BOOST_AUTO_TEST_SUITE_END()
