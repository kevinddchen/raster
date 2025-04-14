#include <raster/camera.hpp>
#include <raster/ncurses.hpp>
#include <raster/scene.hpp>

#include <Eigen/Dense>

#include <numbers>
#include <thread>


constexpr double FRAMES_PER_SEC = 30;

std::chrono::steady_clock::time_point now()
{
    return std::chrono::steady_clock::now();
}

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

    // ----------------------------------------------------------------------
    // render loop

    // How much time passes between frames
    constexpr std::chrono::duration<double, std::milli> frame_interval(1000.0 / FRAMES_PER_SEC);
    // How much to rotate every second
    constexpr double ROTATION_PER_SEC = std::numbers::pi;

    for (long iframe = 1;; ++iframe) {
        const auto t_frame = now();

        const Eigen::Matrix3d rot(Eigen::AngleAxisd(ROTATION_PER_SEC / FRAMES_PER_SEC, Eigen::Vector3d::UnitZ()));
        scene.rotate(rot);

        camera.render(scene);
        doupdate();

        // wait until frame ends
        const std::chrono::duration<double, std::milli> remaining_interval = frame_interval - (now() - t_frame);
        std::this_thread::sleep_for(max(remaining_interval, std::chrono::duration<double, std::milli>::zero()));
    }

    // -----------------------------------------------------------------------

    doupdate();

    wgetch(camera.window);

    raster::end_ncurses();
    return EXIT_SUCCESS;
}
