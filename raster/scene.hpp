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
    /**
     * Create empty scene.
     */
    Scene() {};

    /**
     * Apply rotation to all mesh faces.
     */
    inline void rotate(const Eigen::Matrix3f& rot)
    {
        for (auto& face : mesh) {
            face.rotate(rot);
        }
    }

    std::vector<Face> mesh;
};

}  // namespace raster
