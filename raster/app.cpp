#include <raster/app.hpp>

#include <raster/colors.hpp>

#include <Eigen/Dense>

#include <numbers>
#include <thread>
#include <vector>


namespace
{

constexpr float ANGULAR_ACCELERATION = 0.01;

std::chrono::steady_clock::time_point now()
{
    return std::chrono::steady_clock::now();
}

}  // namespace


namespace raster
{

App::App(int rows, int cols, double frames_per_sec) : mesh_kinetics(0.99f, 0.99f), frames_per_sec(frames_per_sec)
{
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
        5 * 36 + 0 * 6 + 0,   // red
        0 * 36 + 5 * 6 + 0,   // green
        0 * 36 + 0 * 6 + 5,   // blue
        5 * 36 + 5 * 6 + 0,   // yellow
        5 * 36 + 0 * 6 + 5,   // magenta
        5 * 36 + 0 * 6 + 5};  // magenta

    mesh = Mesh(std::move(vertices), std::move(face_vertex_indices), std::move(colors));

    // create a camera away from origin looking at the triangle
    camera = Camera(rows, cols, std::numbers::pi / 2);
    camera.transform(Eigen::Affine3f(Eigen::Translation3f(1, 0, 0)));
    camera.look_at(Eigen::Vector3f(0, 0, 0));

    // ncurses stuff
    init_colors();                   // initialize colors
    curs_set(0);                     // hide cursor
    keypad(camera.window(), true);   // allow arrow keys
    nodelay(camera.window(), true);  // user input is non-blocking
}

void App::run()
{
    // How much time passes between frames
    const std::chrono::duration<double, std::milli> frame_interval(1000.0 / frames_per_sec);

    while (true) {
        const auto t_frame = now();

        // get user key
        const auto key = wgetch(camera.window());
        if (!handle_keystroke(key)) {
            break;
        }

        camera.render(mesh);
        doupdate();

        // clear input buffer to avoid keystrokes from building up
        flushinp();

        // wait until frame ends
        const std::chrono::duration<double, std::milli> remaining_interval = frame_interval - (now() - t_frame);
        std::this_thread::sleep_for(max(remaining_interval, std::chrono::duration<double, std::milli>::zero()));
    }
}

bool App::handle_keystroke(int key)
{
    Eigen::Vector3f delta_angular_velocity = Eigen::Vector3f::Zero();

    // handle keystroke
    switch (key) {
        case 'a':
        case KEY_LEFT: {
            delta_angular_velocity = {0, 0, -ANGULAR_ACCELERATION};
            break;
        }
        case 's':
        case KEY_DOWN: {
            delta_angular_velocity = {0, ANGULAR_ACCELERATION, 0};
            break;
        }
        case 'w':
        case KEY_UP: {
            delta_angular_velocity = {0, -ANGULAR_ACCELERATION, 0};
            break;
        }
        case 'd':
        case KEY_RIGHT: {
            delta_angular_velocity = {0, 0, ANGULAR_ACCELERATION};
            break;
        }
        case 'r':  // refresh
            clearok(curscr, true);
            break;
        case 'q':  // quit
            return false;
        default:
            break;
    }

    // update mesh
    const Eigen::Affine3f delta_pose = mesh_inertial.update(Eigen::Vector3f::Zero(), delta_angular_velocity);
    mesh.transform(delta_pose);

    return true;
}

}  // namespace raster
