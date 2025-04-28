#pragma once

#include <raster/camera.hpp>
#include <raster/mesh.hpp>
#include <raster/physics.hpp>


namespace raster
{

class App
{
public:
    /**
     * Create application.
     *
     * @param rows Number of rows.
     * @param cols Number of columns.
     * @param frames_per_sec Number of frames to render per second.
     */
    App(int rows = 50, int cols = 50, double frames_per_sec = 30.0);

    /**
     * Run the application.
     */
    void run();

private:
    /**
     * Perform action associated with given keystroke.
     *
     * @param key Key pressed.
     * @returns False when we want to quit.
     */
    bool handle_keystroke(int key);

    Mesh mesh;
    Inertial mesh_inertial;
    Camera camera;

    const double frames_per_sec;
};

}  // namespace raster
