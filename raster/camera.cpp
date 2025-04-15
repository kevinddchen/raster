#include <raster/camera.hpp>


namespace
{

/**
 * Project a point from camera space to the image plane.
 */
Eigen::Vector2f project_point(const Eigen::Vector3f& point)
{
    assert(point.z() > 0);
    return {point.x() / point.z(), point.y() / point.z()};
}

/**
 * Returns true if the point `pt` is inside the triangle with the given vertices. Returns false if the point is outside
 * of the triangle or on an edge or vertex of the triangle.
 */
bool point_in_triangle(
    const Eigen::Vector2f& pt, const Eigen::Vector2f& v1, const Eigen::Vector2f& v2, const Eigen::Vector2f& v3)
{
    // Implementation: draw lines from point to each vertex. If we compute the cross product between each pair of lines,
    // going in-order around the triangle, the point is inside the triangle iff all cross products have the same sign.
    const float c1 = (v1.x() - pt.x()) * (v2.y() - pt.y()) - (v1.y() - pt.y()) * (v2.x() - pt.x());
    const float c2 = (v2.x() - pt.x()) * (v3.y() - pt.y()) - (v2.y() - pt.y()) * (v3.x() - pt.x());
    const float c3 = (v3.x() - pt.x()) * (v1.y() - pt.y()) - (v3.y() - pt.y()) * (v1.x() - pt.x());

    return ((c1 > 0 && c2 > 0 && c3 > 0) || (c1 < 0 && c2 < 0 && c3 < 0));
}

}  // namespace


namespace raster
{

Camera::Camera(int height, int width, float horizontal_fov, const Eigen::Affine3f& pose)
    : window(newwin(height, width, 0, 0)),
      camera_to_world(pose),
      world_to_camera(pose.inverse()),
      width(width),
      height(height),
      cx(width / 2.0),
      cy(height / 2.0),
      fx((width / 2.0) * std::tan(horizontal_fov / 2.0)),
      fy(height * fx / width)
{
}

void Camera::render(const Scene& scene) const
{
    werase(window);

    // create border
    box(window, 0, 0);

    for (const auto& face : scene.mesh) {
        // project triangle points to image plane
        const Eigen::Vector2f v1 = project_point(world_to_camera * face.v1());
        const Eigen::Vector2f v2 = project_point(world_to_camera * face.v2());
        const Eigen::Vector2f v3 = project_point(world_to_camera * face.v3());

        // select color
        const chtype attr = COLOR_PAIR(face.color());
        wattron(window, attr);

        // rasterize
        for (int row = 0; row < height; ++row) {
            for (int col = 0; col < width; ++col) {
                // get image plane coordinates for the pixel
                const float u = (col - cx + 0.5) / fx;
                const float v = (row - cy + 0.5) / fy;
                const Eigen::Vector2f pt{u, v};
                if (point_in_triangle(pt, v1, v2, v3)) {
                    mvwaddch(window, row, col, 'X');
                }
            }
        }

        wattroff(window, attr);
    }

    wnoutrefresh(window);
}

}  // namespace raster
