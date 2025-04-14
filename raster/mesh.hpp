#pragma once

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
    Face(const Eigen::Vector3d& v1, const Eigen::Vector3d& v2, const Eigen::Vector3d& v3)
        : _v1(v1),
          _v2(v2),
          _v3(v3) {};

    /**
     * Constructor, by moving input vectors.
     */
    Face(Eigen::Vector3d&& v1, Eigen::Vector3d&& v2, Eigen::Vector3d&& v3) : _v1(v1), _v2(v2), _v3(v3) {};

    /**
     * Copy constructor.
     */
    Face(const Face& face) : _v1(face._v1), _v2(face._v2), _v3(face._v3) {};

    /**
     * Move constructor.
     */
    Face(Face&& face) : _v1(std::move(face._v1)), _v2(std::move(face._v2)), _v3(std::move(face._v3)) {};

    inline const Eigen::Vector3d& v1() const { return _v1; };
    inline const Eigen::Vector3d& v2() const { return _v2; };
    inline const Eigen::Vector3d& v3() const { return _v3; };

    inline Eigen::Vector3d& mut_v1() { return _v1; };
    inline Eigen::Vector3d& mut_v2() { return _v2; };
    inline Eigen::Vector3d& mut_v3() { return _v3; };

private:
    Eigen::Vector3d _v1;
    Eigen::Vector3d _v2;
    Eigen::Vector3d _v3;
};

}  // namespace raster
