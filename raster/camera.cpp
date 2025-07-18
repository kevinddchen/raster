#include <raster/camera.hpp>

#include <raster/colors.hpp>

#include <algorithm>

#include <cmath>


namespace
{

/**
 * Project a 3D point from camera space to the image plane.
 *
 * @param[in] v Input 3D point, in camera coordinates.
 * @param[out] p Output 2D point, in image plane.
 * @returns False if the point cannot be projected into the image plane, true if otherwise successful.
 */
bool project_point(const Eigen::Vector3f& v, Eigen::Vector2f& p)
{
    if (v.z() <= 0) {
        return false;
    }
    p.x() = v.x() / v.z();
    p.y() = v.y() / v.z();
    return true;
}

struct BoundingBox {
    const int min_row;
    const int max_row;
    const int min_col;
    const int max_col;
};

/**
 * Given the vertices of a triangle on the 2D plane, compute the bounding box. We round the box coordinates to integer
 * values in a way that makes the tightest box around the points interior to the triangle and on its edges.
 *
 * @param p1 First 2D point, in pixel coordinates.
 * @param p2 Second 2D point, in pixel coordiantes.
 * @param p3 Third 2D point, in pixel coordinates.
 * @param height Image height, in pixels.
 * @param width Image width, in pixels.
 */
BoundingBox get_bounding_box(
    const Eigen::Vector2f& p1, const Eigen::Vector2f& p2, const Eigen::Vector2f& p3, int height, int width)
{
    const float min_x = std::min(p1.x(), std::min(p2.x(), p3.x()));
    const float max_x = std::max(p1.x(), std::max(p2.x(), p3.x()));
    const float min_y = std::min(p1.y(), std::min(p2.y(), p3.y()));
    const float max_y = std::max(p1.y(), std::max(p2.y(), p3.y()));

    return {
        .min_row = std::max(0, static_cast<int>(std::ceil(min_y))),
        .max_row = std::min(height - 1, static_cast<int>(std::floor(max_y))),
        .min_col = std::max(0, static_cast<int>(std::ceil(min_x))),
        .max_col = std::min(width - 1, static_cast<int>(std::floor(max_x))),
    };
}

/**
 * Given line a -> b and point p on the 2D plane, returns:
 * - positive value if p is on the right side of the line.
 * - zero if p is on the line.
 * - negative value if p is on the left side of the line.
 * The absolute value of the returned result is equal to twice the area of the triangle with vertices at the three
 * points, a b and p. In other words, this function computes the cross product between a -> p and a -> b.
 */
inline float edge_function(const Eigen::Vector2f& p, const Eigen::Vector2f& a, const Eigen::Vector2f& b)
{
    return (p.x() - a.x()) * (b.y() - a.y()) - (p.y() - a.y()) * (b.x() - a.x());
}

/**
 * Returns true if the point `q` is interior to the triangle with the given vertices, or on one of its edges. Also
 * computes the barycentric coordinates of `q`.
 *
 * @param[in] q Query point.
 * @param[in] p1 Triangle vertex.
 * @param[in] p2 Triangle vertex.
 * @param[in] p3 Triangle vertex.
 * @param[out] b1 The barycentric coordinate of `q` relative to `p1`.
 * @param[out] b2 The barycentric coordinate of `q` relative to `p2`.
 * @param[out] b3 The barycentric coordinate of `q` relative to `p3`.
 *
 * NOTE: Top-left rule has not been implemented.
 */
bool point_in_triangle(
    const Eigen::Vector2f& q,
    const Eigen::Vector2f& p1,
    const Eigen::Vector2f& p2,
    const Eigen::Vector2f& p3,
    float& b1,
    float& b2,
    float& b3)
{
    const float edge_12 = edge_function(q, p1, p2);
    const float edge_23 = edge_function(q, p2, p3);
    const float edge_31 = edge_function(q, p3, p1);

    const float signed_area = edge_function(p1, p2, p3);
    b1 = std::abs(edge_23 / signed_area);
    b2 = std::abs(edge_31 / signed_area);
    b3 = std::abs(edge_12 / signed_area);

    return (edge_12 >= 0 && edge_23 >= 0 && edge_31 >= 0) || (edge_12 <= 0 && edge_23 <= 0 && edge_31 <= 0);
}

}  // namespace


namespace raster
{

Camera::Camera(int height, int width, float horizontal_fov, const Eigen::Affine3f& pose)
    : _window(newwin(height, width, 0, 0)),
      intrinsics(
          {.width = width,
           .height = height,
           .cx = width / 2.f - 0.5f,
           .cy = height / 2.f - 0.5f,
           .fx = (width / 2.f) * std::tan(horizontal_fov / 2.f),
           .fy = (height / 2.f) * std::tan(horizontal_fov / 2.f)}),
      camera_to_world(pose),
      world_to_camera(pose.inverse())
{
}

Camera::Camera(Camera&& other)
    : _window(other._window),
      intrinsics(std::move(other.intrinsics)),
      camera_to_world(std::move(other.camera_to_world)),
      world_to_camera(std::move(other.world_to_camera))
{
    other._window = nullptr;
}

Camera& Camera::operator=(Camera&& other)
{
    _window = other._window;
    other._window = nullptr;
    intrinsics = std::move(other.intrinsics);
    camera_to_world = std::move(other.camera_to_world);
    world_to_camera = std::move(other.world_to_camera);
    return *this;
}

void Camera::render(const Mesh& mesh) const
{
    werase(_window);

    // draw border
    box(_window, 0, 0);

    // initialize z-buffer
    Eigen::ArrayXXf z_buf = Eigen::ArrayXXf::Constant(intrinsics.height, intrinsics.width, -1.0);

    for (auto ptr = mesh.begin(); ptr != mesh.end(); ++ptr) {
        const auto& face = *ptr;

        // get triangle points in camera space
        const Eigen::Vector3f v1 = world_to_camera * face.v1;
        const Eigen::Vector3f v2 = world_to_camera * face.v2;
        const Eigen::Vector3f v3 = world_to_camera * face.v3;

        // project triangle points to image plane
        Eigen::Vector2f p1, p2, p3;
        if (!project_point(v1, p1) || !project_point(v2, p2) || !project_point(v3, p3)) {
            // skip if a portion of the triangle lies outside of the image plane
            continue;
        }

        // convert from image plane coords to pixel coords
        const Eigen::Vector2f pix1 = image_plane_to_pixel(p1, intrinsics);
        const Eigen::Vector2f pix2 = image_plane_to_pixel(p2, intrinsics);
        const Eigen::Vector2f pix3 = image_plane_to_pixel(p3, intrinsics);

        // get bounding box
        const BoundingBox bbox = get_bounding_box(pix1, pix2, pix3, intrinsics.height, intrinsics.width);

        // divide vertex colors by z-coordinate
        const Eigen::Array3f corrected_c1 = srgb_to_linear(face.c1) / v1.z();
        const Eigen::Array3f corrected_c2 = srgb_to_linear(face.c2) / v2.z();
        const Eigen::Array3f corrected_c3 = srgb_to_linear(face.c3) / v3.z();

        // rasterize mesh face
        for (int row = bbox.min_row; row <= bbox.max_row; ++row) {
            for (int col = bbox.min_col; col <= bbox.max_col; ++col) {
                // get coords of the pixel
                const Eigen::Vector2f pixq{col, row};

                // check that pixel is interior to the triangle or on an edge
                float b1, b2, b3;  // barycentric coords
                if (!point_in_triangle(pixq, pix1, pix2, pix3, b1, b2, b3)) {
                    continue;
                }

                // compute z for the pixel using perspective-correct interpolation
                const float& prev_z = z_buf(row, col);
                const float z = 1 / (b1 / v1.z() + b2 / v2.z() + b3 / v3.z());
                if (prev_z > 0 && z >= prev_z) {
                    continue;
                }

                // update z-buffer and render pixel
                z_buf(row, col) = z;

                // compute color for the pixel using perspective-correct interpolation
                const Eigen::Array3f c =
                    linear_to_srgb(z * (corrected_c1 * b1 + corrected_c2 * b2 + corrected_c3 * b3));
                const short color_pair = rgb_to_color_pair(c);

                // draw pixel
                const chtype attr = COLOR_PAIR(color_pair);
                wattron(_window, attr);
                mvwaddch(_window, row, col, ' ');
                wattroff(_window, attr);
            }
        }
    }

    wnoutrefresh(_window);
}

void Camera::transform(const Eigen::Affine3f& t)
{
    camera_to_world = t * camera_to_world;
    world_to_camera = world_to_camera * t.inverse();
}

void Camera::look_at(const Eigen::Vector3f& look_at_point, const Eigen::Vector3f& world_up)
{
    // first, determine the camera forward, up, and right in world coordinates
    Eigen::Vector3f camera_forward = look_at_point - camera_to_world.translation();
    camera_forward.normalize();

    Eigen::Vector3f camera_up = world_up - camera_forward * world_up.dot(camera_forward);
    assert(camera_up.norm() > 1e-6);  // make sure `look_at_point` and `world_up` are not parallel
    camera_up.normalize();

    const Eigen::Vector3f camera_right = camera_forward.cross(camera_up);

    // construct the rotation. recall that:
    // - "forward" is the +z direction.
    // - "up" is the -y direction.
    // - "right" is the +x direction.
    Eigen::Matrix3f rotation;
    rotation.col(0) = camera_right;
    rotation.col(1) = -camera_up;
    rotation.col(2) = camera_forward;

    camera_to_world.linear() = rotation;
    world_to_camera = camera_to_world.inverse();
}

void Camera::set_pose(const Eigen::Affine3f& camera_to_world)
{
    this->camera_to_world = camera_to_world;
    this->world_to_camera = camera_to_world.inverse();
}

Eigen::Vector2f Camera::image_plane_to_pixel(const Eigen::Vector2f& p, const Intrinsics& intrinsics)
{
    return {intrinsics.fx * p.x() + intrinsics.cx, intrinsics.fy * p.y() + intrinsics.cy};
}

}  // namespace raster
