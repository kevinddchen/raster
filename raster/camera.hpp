#pragma once

#include <raster/scene.hpp>

#include <ncurses.h>
#include <Eigen/Dense>


namespace raster
{

/**
 * Camera class.
 *
 * We use the following camera coordinate conventions:
 * - "forward" is the +z direction.
 * - "up" is the -y direction.
 * - "right" is the +x direction.
 *
 * A pose consists of a rotation `rot` and translation `trans`. Explicitly, the camera-to-world transformation is
 * implemented as the affine transformation `rot * v + trans` where `v` is a vector in camera coordinates.
 */
class Camera
{
public:
    /**
     * Create new perspective camera.
     * @param height Image height, in pixels.
     * @param width Image width, in pixels.
     * @param horizontal_fov Horizontal field of view, in radians.
     * @param pose Camera-to-world pose.
     */
    Camera(int height, int width, float horizontal_fov, const Eigen::Affine3f& pose);

    /**
     * Render the scene.
     */
    void render(const Scene& scene) const;

    WINDOW* const window;

    const Eigen::Affine3f camera_to_world;
    const Eigen::Affine3f world_to_camera;

    const int width;
    const int height;
    const float cx;
    const float cy;
    const float fx;
    const float fy;
};

}  // namespace raster
