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
        const Eigen::Vector3f& v1, const Eigen::Vector3f& v2, const Eigen::Vector3f& v3, short color = COLOR_PAIR_WHITE)
        : _v1(v1),
          _v2(v2),
          _v3(v3),
          _color(color) {};

    /**
     * Constructor, by moving input vectors.
     */
    Face(Eigen::Vector3f&& v1, Eigen::Vector3f&& v2, Eigen::Vector3f&& v3, short color = COLOR_PAIR_WHITE)
        : _v1(v1),
          _v2(v2),
          _v3(v3),
          _color(color) {};

    /**
     * Copy constructor.
     */
    Face(const Face& face) : _v1(face._v1), _v2(face._v2), _v3(face._v3), _color(face._color) {};

    /**
     * Move constructor.
     */
    Face(Face&& face)
        : _v1(std::move(face._v1)),
          _v2(std::move(face._v2)),
          _v3(std::move(face._v3)),
          _color(face._color) {};

    /**
     * Apply rotation to all vertices.
     */
    void rotate(const Eigen::Matrix3f& rot);

    inline const Eigen::Vector3f& v1() const { return _v1; }
    inline const Eigen::Vector3f& v2() const { return _v2; }
    inline const Eigen::Vector3f& v3() const { return _v3; }

    inline Eigen::Vector3f& mut_v1() { return _v1; }
    inline Eigen::Vector3f& mut_v2() { return _v2; }
    inline Eigen::Vector3f& mut_v3() { return _v3; }

    inline short color() const { return _color; }
    inline short& mut_color() { return _color; }

private:
    Eigen::Vector3f _v1;
    Eigen::Vector3f _v2;
    Eigen::Vector3f _v3;

    // ncurses color pair.
    short _color;
};

}  // namespace raster
