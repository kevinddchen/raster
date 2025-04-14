#pragma once

#include <raster/mesh.hpp>

#include <vector>


namespace raster
{

/**
 * A scene consists of a collection of mesh faces.
 */
class Scene
{
public:
    Scene() {};

    std::vector<Face> mesh;
};

}  // namespace raster
