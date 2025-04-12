#pragma once

#include <Eigen/Dense>

#include <array>
#include <vector>


namespace raster
{

class Scene
{
public:
    Scene();

    std::vector<std::array<Eigen::Vector3d, 3>> triangles;
};

}  // namespace raster
