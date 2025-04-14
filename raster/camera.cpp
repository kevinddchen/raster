#include <raster/camera.hpp>


namespace
{

/**
 * Project a point from camera space to the image plane.
 */
Eigen::Vector2d project_point(const Eigen::Vector3d& point)
{
    assert(point.z() > 0);
    return {point.x() / point.z(), point.y() / point.z()};
}

/**
 * Returns true if the point `pt` is inside the triangle with the given vertices. Returns false if the point is outside
 * of the triangle or on an edge or vertex of the triangle.
 */
bool point_in_triangle(
    const Eigen::Vector2d& pt, const Eigen::Vector2d& v1, const Eigen::Vector2d& v2, const Eigen::Vector2d& v3)
{
    // Implementation: draw lines from point to each vertex. If we compute the cross product between each pair of lines,
    // going in-order around the triangle, the point is inside the triangle iff all cross products have the same sign.
    const double c1 = (v1.x() - pt.x()) * (v2.y() - pt.y()) - (v1.y() - pt.y()) * (v2.x() - pt.x());
    const double c2 = (v2.x() - pt.x()) * (v3.y() - pt.y()) - (v2.y() - pt.y()) * (v3.x() - pt.x());
    const double c3 = (v3.x() - pt.x()) * (v1.y() - pt.y()) - (v3.y() - pt.y()) * (v1.x() - pt.x());

    return ((c1 > 0 && c2 > 0 && c3 > 0) || (c1 < 0 && c2 < 0 && c3 < 0));
}

/**
 * Debug print to console.
 */
void print(const Eigen::MatrixXi& screen)
{
    for (int row = 0; row < screen.rows(); ++row) {
        for (int col = 0; col < screen.cols(); ++col) {
            if (screen(row, col) > 0) {
                printf("X");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}


}  // namespace


namespace raster
{

Camera::Camera(int height, int width, double horizontal_fov, const Eigen::Affine3d& pose)
    : pose(pose),
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
    Eigen::MatrixXi screen = Eigen::MatrixXi::Zero(height, width);
    for (const auto& face : scene.faces) {
        // project triangle points to image plane
        const Eigen::Affine3d world_to_camera = pose.inverse();
        const Eigen::Vector2d v1 = project_point(world_to_camera * face.v1());
        const Eigen::Vector2d v2 = project_point(world_to_camera * face.v2());
        const Eigen::Vector2d v3 = project_point(world_to_camera * face.v3());

        // rasterize
        for (int row = 0; row < screen.rows(); ++row) {
            for (int col = 0; col < screen.cols(); ++col) {
                // get image plane coordinates for the pixel
                const double u = (col - cx + 0.5) / fx;
                const double v = (row - cy + 0.5) / fy;
                const Eigen::Vector2d pt{u, v};
                if (point_in_triangle(pt, v1, v2, v3)) {
                    screen(row, col) = 1;
                }
            }
        }
    }

    print(screen);
}

}  // namespace raster
