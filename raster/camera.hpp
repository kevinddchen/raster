#pragma once

#include <raster/mesh.hpp>

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
 * A pose consists of a rotation `rot` and translation `trans`. The camera-to-world pose is implemented as an affine
 * transformation on 3D vectors. Explicitly, it is `rot * v + trans` where `v` is a vector.
 */
class Camera
{
public:
    Camera() = default;

    /**
     * Create new perspective camera.
     *
     * @param height Image height, in pixels.
     * @param width Image width, in pixels.
     * @param horizontal_fov Horizontal field of view, in radians.
     * @param camera_to_world Camera-to-world pose.
     */
    Camera(
        int height,
        int width,
        float horizontal_fov,
        const Eigen::Affine3f& camera_to_world = Eigen::Affine3f::Identity());

    // NOTE: copy constructors are deleted to prevent expensive copies
    Camera(const Camera&) = delete;
    Camera& operator=(const Camera&) = delete;

    Camera(Camera&& other);
    Camera& operator=(Camera&& other);

    /**
     * Render the scene.
     */
    void render(const Mesh& mesh) const;

    /**
     * Apply an affine (i.e. rigid) transformation to the camera, with respect to the world coordinates. Concretely,
     * this will be a left-multiplication to the camera-to-world pose.
     */
    void transform(const Eigen::Affine3f& t);

    /**
     * Rotate the camera to look at a point.
     *
     * @param look_at_point The 3D point to look at, in world coordinates.
     * @param world_up Vector pointing in the "up" direction, in world coordinates.
     */
    void look_at(const Eigen::Vector3f& look_at_point, const Eigen::Vector3f& world_up = Eigen::Vector3f::UnitZ());

private:
    struct Intrinsics {
        int width;
        int height;
        float cx;
        float cy;
        float fx;
        float fy;
    };

    /**
     * Convert a 2D point in the image plane coordinates to pixel coordinates.
     */
    static Eigen::Vector2f image_plane_to_pixel(const Eigen::Vector2f& p, const Intrinsics& intrinsics);

    WINDOW* window;

    Intrinsics intrinsics;
    Eigen::Affine3f camera_to_world;
    Eigen::Affine3f world_to_camera;
};

}  // namespace raster
