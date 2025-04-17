#include <raster/camera.hpp>

#include <vector>


namespace
{

/**
 * Project a 3D point from camera space to the image plane.
 * @param point Input 3D point, in camera coordinates.
 * @param out Reference to output 2D point, in image plane.
 * @returns False if the point cannot be projected into the image plane.
 */
bool project_point(const Eigen::Vector3f& point, Eigen::Vector2f& out)
{
    if (point.z() <= 0) {
        return false;
    }
    out = {point.x() / point.z(), point.y() / point.z()};
    return true;
}

struct BoundingBox {
    int min_row;
    int max_row;
    int min_col;
    int max_col;
};

/**
 * Given a collection of 2D points, compute the bounding box. We round the box coordinates to integer values in a way
 * that makes the smallest box. This is because `points` is typically the vertices of a triangle, and we want a tight
 * box around points on and interior to the triangle.
 */
BoundingBox get_bounding_box(const std::vector<const Eigen::Vector2f*>& points)
{
    assert(points.size() > 0);
    float min_x = points[0]->x();
    float max_x = min_x;
    float min_y = points[0]->y();
    float max_y = min_y;

    for (auto ptr = points.begin() + 1; ptr != points.end(); ++ptr) {
        const float x = (*ptr)->x();
        const float y = (*ptr)->y();
        if (x < min_x) {
            min_x = x;
        } else if (x > max_x) {
            max_x = x;
        }
        if (y < min_y) {
            min_y = y;
        } else if (y > max_y) {
            max_y = y;
        }
    }

    return {
        .min_row = static_cast<int>(std::ceil(min_y)),
        .max_row = static_cast<int>(std::floor(max_y)),
        .min_col = static_cast<int>(std::ceil(min_x)),
        .max_col = static_cast<int>(std::floor(max_x))};
}

/**
 * Returns true if the point `pt` is inside the triangle with the given vertices. Returns false if the point is
 * outside of the triangle or on an edge or vertex of the triangle.
 */
bool point_in_triangle(
    const Eigen::Vector2f& pt, const Eigen::Vector2f& v1, const Eigen::Vector2f& v2, const Eigen::Vector2f& v3)
{
    // Implementation: draw lines from point to each vertex. If we compute the cross product between each pair of
    // lines, going in-order around the triangle, the point is inside the triangle iff all cross products have the
    // same sign.
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
      cx(width / 2.0 - 0.5),
      cy(height / 2.0 - 0.5),
      fx((width / 2.0) * std::tan(horizontal_fov / 2.0)),
      fy(height * fx / width)
{
}

void Camera::render(const Scene& scene) const
{
    werase(window);

    // draw border
    box(window, 0, 0);

    for (const auto& face : scene.mesh) {
        // project triangle points to image plane
        Eigen::Vector2f v1, v2, v3;
        if (!project_point(world_to_camera * face.v1(), v1) || !project_point(world_to_camera * face.v2(), v2) ||
            !project_point(world_to_camera * face.v3(), v3)) {
            // skip if a portion of the triangle lies outside of the image plane
            // TODO: we can still draw part of the triangle... maybe this is related to overlapping triangles...
            continue;
        }

        // convert from normalized coords to pixel coords
        v1 = {fx * v1.x() + cx, fy * v1.y() + cy};
        v2 = {fx * v2.x() + cx, fy * v2.y() + cy};
        v3 = {fx * v3.x() + cx, fy * v3.y() + cy};

        // get bounding box
        BoundingBox bbox = get_bounding_box({&v1, &v2, &v3});

        // select color
        const chtype attr = COLOR_PAIR(face.color());
        wattron(window, attr);

        // rasterize
        for (int row = bbox.min_row; row <= bbox.max_row; ++row) {
            for (int col = bbox.min_col; col <= bbox.max_col; ++col) {
                // get image plane coordinates for the pixel
                const Eigen::Vector2f pt{col, row};
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
