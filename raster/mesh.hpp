#pragma once

#include <raster/ncurses.hpp>

#include <Eigen/Dense>


namespace raster
{

/**
 * Triangle mesh face.
 */
class Face
{
public:
    /**
     * Constructor, by copying input vectors.
     */
    Face(
        const Eigen::Vector3d& v1,
        const Eigen::Vector3d& v2,
        const Eigen::Vector3d& v3,
        short color = COLOR_PAIR_WHITE);

    /**
     * Constructor, by moving input vectors.
     */
    Face(Eigen::Vector3d&& v1, Eigen::Vector3d&& v2, Eigen::Vector3d&& v3, short color = COLOR_PAIR_WHITE);

    /**
     * Copy constructor.
     */
    Face(const Face& face);

    /**
     * Move constructor.
     */
    Face(Face&& face);

    /**
     * Apply rotation to all vertices.
     */
    void rotate(const Eigen::Matrix3d& rot);

    inline const Eigen::Vector3d& v1() const { return _v1; }
    inline const Eigen::Vector3d& v2() const { return _v2; }
    inline const Eigen::Vector3d& v3() const { return _v3; }

    inline Eigen::Vector3d& mut_v1() { return _v1; }
    inline Eigen::Vector3d& mut_v2() { return _v2; }
    inline Eigen::Vector3d& mut_v3() { return _v3; }

    inline short color() const { return _color; }
    inline short& mut_color() { return _color; }

private:
    Eigen::Vector3d _v1;
    Eigen::Vector3d _v2;
    Eigen::Vector3d _v3;

    // ncurses color pair.
    short _color;
};

}  // namespace raster
