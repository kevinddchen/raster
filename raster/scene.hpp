#pragma once

#include <raster/mesh.hpp>

#include <vector>


namespace raster
{

/**
 * A scene consists of a collection of mesh faces.
 *
 * We use the following coordinate conventions:
 * - up is the +z direction
 */
class Scene
{
public:
    Scene() {};

    std::vector<Face> mesh;
};

}  // namespace raster
