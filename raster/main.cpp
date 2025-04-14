#include <raster/camera.hpp>
#include <raster/ncurses.hpp>
#include <raster/scene.hpp>

#include <Eigen/Dense>

#include <numbers>


int main()
{
    raster::init_ncurses();

    // -----------------------------------------------------------------------

    // create a triangle at the origin, in the xz-plane
    raster::Face face(
        Eigen::Vector3d{0.0, 0.0, 0.5}, Eigen::Vector3d{0.5, 0.0, -0.5}, Eigen::Vector3d{-0.5, 0.0, -0.5});

    raster::Scene scene;
    scene.mesh.push_back(std::move(face));

    // create a camera looking at the triangle
    // camera is on the y-axis looking at the origin.
    // camera +x = world -x
    // camera +y = world -z
    // camera +z = world -y
    const Eigen::Matrix3d rot{{-1, 0, 0}, {0, 0, -1}, {0, -1, 0}};
    const Eigen::Vector3d trans{0, 1, 0};
    Eigen::Affine3d pose;
    pose.linear() = rot;
    pose.translation() = trans;

    raster::Camera camera(50, 50, std::numbers::pi / 2, pose);

    // render!
    camera.render(scene);

    // -----------------------------------------------------------------------

    doupdate();

    wgetch(camera.window);

    raster::end_ncurses();
    return EXIT_SUCCESS;
}
