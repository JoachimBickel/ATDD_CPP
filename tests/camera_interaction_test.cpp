#include <boost/test/unit_test.hpp>

#include <cmath>

#include <viewer/app/ViewerService.h>
#include <viewer/ports/CameraState.h>

#include "support/fakes.h"

using namespace testsupport;

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
    const double quarterTurn = std::acos(-1.0) / 2.0;  // 90 degrees, in radians
    service.orbit(quarterTurn);

    // Framed eye (0.5, 0.5, sqrt(2)) rotated 90 deg about the vertical axis
    // through the target (0.5, 0.5, 0) lands at (0.5 + sqrt(2), 0.5, 0).
    BOOST_TEST(view.shownCamera.eye.x == 0.5 + std::sqrt(2.0), boost::test_tools::tolerance(1e-9));
    BOOST_TEST(view.shownCamera.eye.y == 0.5);
    BOOST_TEST(view.shownCamera.eye.z == 0.0, boost::test_tools::tolerance(1e-9));

    BOOST_TEST(view.shownCamera.target.x == 0.5);
    BOOST_TEST(view.shownCamera.target.y == 0.5);
    BOOST_TEST(view.shownCamera.target.z == 0.0);
}

BOOST_AUTO_TEST_SUITE_END()
