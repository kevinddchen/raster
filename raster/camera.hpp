#pragma once

#include <raster/scene.hpp>

#include <ncurses.h>
#include <Eigen/Dense>


namespace raster
{

/**
 * Camera class.
 *
 * Note that we use camera space conventions where the x-axis points right and y-axis points down, so the z-axis points
 * forwards.
 */
class Camera
{
public:
    /**
     * Create new camera.
     * @param height Image height, in pixels.
     * @param width Image width, in pixels.
     * @param horizontal_fov Horizontal field of view, in radians.
     * @param pose Camera-to-world pose.
     */
    Camera(int height, int width, double horizontal_fov, const Eigen::Affine3d& pose);

    /**
     * Render the scene.
     */
    void render(const Scene& scene) const;

    WINDOW* const window;

    const Eigen::Affine3d pose;

    const int width;
    const int height;
    const double cx;
    const double cy;
    const double fx;
    const double fy;
};

}  // namespace raster
