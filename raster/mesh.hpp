#pragma once

#include <raster/ncurses.hpp>

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
    Face(
        const Eigen::Vector3f& v1,
        const Eigen::Vector3f& v2,
        const Eigen::Vector3f& v3,
        short color = COLOR_PAIR_WHITE);

    /**
     * Constructor, by moving input vectors.
     */
    Face(Eigen::Vector3f&& v1, Eigen::Vector3f&& v2, Eigen::Vector3f&& v3, short color = COLOR_PAIR_WHITE);

    /**
     * Copy constructor.
     */
    Face(const Face& face);

    /**
     * Move constructor.
     */
    Face(Face&& face);

    /**
     * Apply rotation to mesh face.
     */
    void rotate(const Eigen::Matrix3f& rot);

    inline const Eigen::Vector3f& v1() const { return _v1; }
    inline const Eigen::Vector3f& v2() const { return _v2; }
    inline const Eigen::Vector3f& v3() const { return _v3; }

    inline short color() const { return _color; }

private:
    // NOTE: Vertices are in world coordinates.
    Eigen::Vector3f _v1;
    Eigen::Vector3f _v2;
    Eigen::Vector3f _v3;

    // ncurses color pair.
    short _color;
};

/**
 * A mesh consists of a collection of faces.
 */
class Mesh
{
public:
    // NOTE: For now, pass in a list of `Face`s, but in the future we want to pass in a list of vertices and faces as
    // triples of vertex indices, like an OBJ file...
    Mesh(std::vector<Face>&& faces) : faces(faces) {}

    /**
     * Iterator over faces of the mesh.
     */
    template <bool is_const>
    struct Iterator {
        using difference_type = std::ptrdiff_t;
        using value_type = Face;

        using TFace = typename std::conditional_t<is_const, const Face, Face>;
        using TMesh = typename std::conditional_t<is_const, const Mesh, Mesh>;

        Iterator() = default;
        Iterator(TMesh* ptr, size_t idx) : ptr(ptr), idx(idx) {}

        TFace& operator*() const { return ptr->faces[idx]; }

        Iterator& operator++()
        {
            ++idx;
            return *this;
        }

        Iterator operator++(int)
        {
            auto tmp = *this;
            ++*this;
            return tmp;
        }

        bool operator==(const Iterator& other) const { return ptr == other.ptr && idx == other.idx; }

    private:
        TMesh* ptr;
        size_t idx;
    };
    static_assert(std::forward_iterator<Iterator<false>>);
    static_assert(std::forward_iterator<Iterator<true>>);

    Iterator<false> begin() { return Iterator<false>(this, 0); }
    Iterator<false> end() { return Iterator<false>(this, faces.size()); }
    Iterator<true> cbegin() const { return Iterator<true>(this, 0); }
    Iterator<true> cend() const { return Iterator<true>(this, faces.size()); }

private:
    // NOTE: Internal data structure to be changed...
    std::vector<Face> faces;
};

}  // namespace raster
