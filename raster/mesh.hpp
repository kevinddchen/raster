#pragma once

#include <raster/ncurses.hpp>

#include <Eigen/Dense>


namespace raster
{

/**
 * Triangle mesh face in 3D space.
 */
class Face
{
public:
    /**
     * Constructor, by copying input vectors.
     */
    Face(
        const Eigen::Vector3f& v1,
        const Eigen::Vector3f& v2,
        const Eigen::Vector3f& v3,
        short color = COLOR_PAIR_WHITE);

    /**
     * Constructor, by moving input vectors.
     */
    Face(Eigen::Vector3f&& v1, Eigen::Vector3f&& v2, Eigen::Vector3f&& v3, short color = COLOR_PAIR_WHITE);

    /**
     * Copy constructor.
     */
    Face(const Face& face);

    /**
     * Move constructor.
     */
    Face(Face&& face);

    /**
     * Apply rotation to mesh face.
     */
    void rotate(const Eigen::Matrix3f& rot);

    inline const Eigen::Vector3f& v1() const { return _v1; }
    inline const Eigen::Vector3f& v2() const { return _v2; }
    inline const Eigen::Vector3f& v3() const { return _v3; }

    inline short color() const { return _color; }

private:
    Eigen::Vector3f _v1;
    Eigen::Vector3f _v2;
    Eigen::Vector3f _v3;

    // ncurses color pair.
    short _color;
};

}  // namespace raster
