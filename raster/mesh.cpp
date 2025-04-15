#include <raster/mesh.hpp>


namespace raster
{

void Face::rotate(const Eigen::Matrix3f& rot)
{
    _v1 = rot * _v1;
    _v2 = rot * _v2;
    _v3 = rot * _v3;
}

}  // namespace raster
