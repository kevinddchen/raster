#pragma once

#include <Eigen/Dense>

#include <iterator>
#include <vector>


namespace raster
{

/**
 * Triangle face of a mesh.
 */
struct Face {
    // First vertex, as 3D point in world coordiantes.
    const Eigen::Vector3f& v1;
    // Second vertex, as 3D point in world coordiantes.
    const Eigen::Vector3f& v2;
    // Third vertex, as 3D point in world coordiantes.
    const Eigen::Vector3f& v3;
    // Color of first vertex, as RGB value normalized to [0, 1].
    const Eigen::Vector3f& c1;
    // Color of second vertex, as RGB value normalized to [0, 1].
    const Eigen::Vector3f& c2;
    // Color of third vertex, as RGB value normalized to [0, 1].
    const Eigen::Vector3f& c3;
};

/**
 * A mesh consists of a collection of triangle faces.
 */
class Mesh
{
public:
    Mesh() = default;

    /**
     * Create a mesh.
     *
     * @param vertices List of vertices, as 3D points in world coordinates.
     * @param face_vertex_indices List of faces, represented as triples of integer indices of `vertices`.
     * @param vertect_colors List of vertex colors, as RGB values normalized to [0, 1]. Same length as `vertices`.
     */
    Mesh(
        std::vector<Eigen::Vector3f>&& vertices,
        std::vector<Eigen::Array3i>&& face_vertex_indices,
        std::vector<Eigen::Vector3f>&& vertex_colors);

    // NOTE: copy constructors are deleted to prevent expensive copies
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other);
    Mesh& operator=(Mesh&& other);

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
    inline Iterator begin() const { return Iterator(this, 0); }

    /**
     * Iterator to the end of the collection of faces.
     */
    inline Iterator end() const { return Iterator(this, face_vertex_indices.size()); }

private:
    std::vector<Eigen::Vector3f> vertices;
    std::vector<Eigen::Array3i> face_vertex_indices;
    std::vector<Eigen::Vector3f> vertex_colors;
};

}  // namespace raster
