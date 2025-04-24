#include <raster/camera.hpp>
#include <raster/ncurses.hpp>

#include <Eigen/Dense>

#include <numbers>
#include <thread>
#include <vector>


constexpr double FRAMES_PER_SEC = 30;

std::chrono::steady_clock::time_point now()
{
    return std::chrono::steady_clock::now();
}

int main()
{
    raster::init_ncurses();

    // -----------------------------------------------------------------------

    // NOTE: world "up" is the +z axis

    // create a pyramid
    std::vector<Eigen::Vector3f> vertices = {
        Eigen::Vector3f{0.0, 0.0, 0.8},
        Eigen::Vector3f{0.5, 0.0, -0.4},
        Eigen::Vector3f{0.0, 0.5, -0.4},
        Eigen::Vector3f{-0.5, 0.0, -0.4},
        Eigen::Vector3f{0.0, -0.5, -0.4},
    };

    std::vector<Eigen::Array3i> face_vertex_indices = {
        Eigen::Array3i{0, 1, 2},
        Eigen::Array3i{0, 2, 3},
        Eigen::Array3i{0, 3, 4},
        Eigen::Array3i{0, 4, 1},
        Eigen::Array3i{1, 3, 2},
        Eigen::Array3i{1, 4, 3},
    };

    std::vector<short> colors = {
        raster::COLOR_PAIR_RED,
        raster::COLOR_PAIR_GREEN,
        raster::COLOR_PAIR_YELLOW,
        raster::COLOR_PAIR_BLUE,
        raster::COLOR_PAIR_MAGENTA,
        raster::COLOR_PAIR_CYAN};

    raster::Mesh mesh(std::move(vertices), std::move(face_vertex_indices), std::move(colors));

    // create a camera away from origin looking at the triangle

    raster::Camera camera(50, 50, std::numbers::pi / 2);

    camera.transform(Eigen::Affine3f(Eigen::Translation3f(1, 0, 0)));
    camera.look_at(Eigen::Vector3f(0, 0, 0));

    // ----------------------------------------------------------------------
    // render loop

    // How much time passes between frames
    constexpr std::chrono::duration<double, std::milli> frame_interval(1000.0 / FRAMES_PER_SEC);
    // How much to rotate every second
    constexpr double ROTATION_PER_SEC = std::numbers::pi;

    for (long iframe = 1;; ++iframe) {
        const auto t_frame = now();

        const Eigen::Affine3f rot(Eigen::AngleAxisf(ROTATION_PER_SEC / FRAMES_PER_SEC, Eigen::Vector3f::UnitZ()));
        mesh.transform(rot);

        camera.render(mesh);
        doupdate();

        // wait until frame ends
        const std::chrono::duration<double, std::milli> remaining_interval = frame_interval - (now() - t_frame);
        std::this_thread::sleep_for(max(remaining_interval, std::chrono::duration<double, std::milli>::zero()));
    }

    // -----------------------------------------------------------------------

    raster::end_ncurses();
    return EXIT_SUCCESS;
}
