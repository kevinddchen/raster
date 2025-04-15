#include <raster/mesh.hpp>


namespace raster
{

Face::Face(const Eigen::Vector3d& v1, const Eigen::Vector3d& v2, const Eigen::Vector3d& v3, short color)
    : _v1(v1),
      _v2(v2),
      _v3(v3),
      _color(color) {};

Face::Face(Eigen::Vector3d&& v1, Eigen::Vector3d&& v2, Eigen::Vector3d&& v3, short color)
    : _v1(v1),
      _v2(v2),
      _v3(v3),
      _color(color) {};

Face::Face(const Face& face) : _v1(face._v1), _v2(face._v2), _v3(face._v3), _color(face._color) {};

Face::Face(Face&& face)
    : _v1(std::move(face._v1)),
      _v2(std::move(face._v2)),
      _v3(std::move(face._v3)),
      _color(face._color) {};

void Face::rotate(const Eigen::Matrix3d& rot)
{
    _v1 = rot * _v1;
    _v2 = rot * _v2;
    _v3 = rot * _v3;
}

}  // namespace raster
