#pragma once

#include <Eigen/Dense>

#include <vector>


namespace raster
{

/**
 * Triangle mesh face.
 */
class Face
{
public:
    Face(Eigen::Vector3d&& v1, Eigen::Vector3d&& v2, Eigen::Vector3d&& v3) : v1(v1), v2(v2), v3(v3) {};
    Face(const Face& face) : v1(face.v1), v2(face.v2), v3(face.v3) {};
    Face(Face&& face) : v1(std::move(face.v1)), v2(std::move(face.v2)), v3(std::move(face.v3)) {};

    Eigen::Vector3d v1;
    Eigen::Vector3d v2;
    Eigen::Vector3d v3;
};

/**
 * A scene consists of a collection of mesh faces.
 */
class Scene
{
public:
    Scene() {};

    std::vector<Face> faces;
};

}  // namespace raster
