#pragma once

#include <raster/ncurses.hpp>

#include <Eigen/Dense>

#include <iterator>
#include <optional>
#include <vector>


namespace raster
{

/**
 * Triangle mesh face.
 */
struct Face {
    // First vertex, as 3D point in world coordiantes.
    const Eigen::Vector3f& v1;
    // Second vertex, as 3D point in world coordiantes.
    const Eigen::Vector3f& v2;
    // Third vertex, as 3D point in world coordiantes.
    const Eigen::Vector3f& v3;
    // ncurses color pair.
    const short color;
};

/**
 * A mesh consists of a collection of faces.
 */
class Mesh
{
public:
    /**
     * Create a mesh.
     *
     * @param vertices List of vertices, as 3D points in world coordinates.
     * @param face_vertex_indices List of faces, represented as triples of integer indices corresponding to elements of
     *     `vertices`.
     * @param colors (Optional) List of color pair indices. If provided, must be the same length as `faces`. If not
     *     provided, will default to all faces being white.
     */
    Mesh(
        std::vector<Eigen::Vector3f>&& vertices,
        std::vector<Eigen::Array3i>&& face_vertex_indices,
        std::optional<std::vector<short>>&& colors = std::nullopt);

    /**
     * Apply an affine (i.e. rigid) transformation to the mesh.
     */
    void transform(const Eigen::Affine3f& t);

    // -----------------------------------------------------------------------

    /**
     * Read-only iterator over faces of the mesh.
     */
    struct Iterator {
        using difference_type = std::ptrdiff_t;
        using value_type = Face;

        Iterator() = default;
        Iterator(const Mesh* ptr, size_t idx) : ptr(ptr), idx(idx) {}

        Face operator*() const;

        Iterator& operator++()
        {
            ++idx;
            return *this;
        }

        Iterator operator++(int)
        {
            const auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const Iterator& other) const { return ptr == other.ptr && idx == other.idx; }

    private:
        const Mesh* ptr;
        size_t idx;
    };
    static_assert(std::forward_iterator<Iterator>);

    /**
     * Iterator to the beginning of the collection of faces.
     */
    Iterator begin() const { return Iterator(this, 0); }

    /**
     * Iterator to the end of the collecrtion of faces.
     */
    Iterator end() const { return Iterator(this, face_vertex_indices.size()); }

private:
    std::vector<Eigen::Vector3f> vertices;
    std::vector<Eigen::Array3i> face_vertex_indices;
    std::vector<short> colors;
};

}  // namespace raster
