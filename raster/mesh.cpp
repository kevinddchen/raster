#include <raster/mesh.hpp>


namespace raster
{

Mesh::Mesh(
    std::vector<Eigen::Vector3f>&& vertices,
    std::vector<Eigen::Array3i>&& face_vertex_indices,
    std::vector<Eigen::Vector3f>&& vertex_colors)
    : vertices(vertices),
      face_vertex_indices(face_vertex_indices),
      vertex_colors(vertex_colors)
{
    assert(vertices.size() == vertex_colors.size());
}

Mesh::Mesh(Mesh&& other)
    : vertices(std::move(other.vertices)),
      face_vertex_indices(std::move(other.face_vertex_indices)),
      vertex_colors(std::move(other.vertex_colors))
{
}

Mesh& Mesh::operator=(Mesh&& other)
{
    vertices = std::move(other.vertices);
    face_vertex_indices = std::move(other.face_vertex_indices);
    vertex_colors = std::move(other.vertex_colors);
    return *this;
}

void Mesh::transform(const Eigen::Affine3f& t)
{
    for (auto& v : vertices) {
        v = t * v;
    }
}

Face Mesh::Iterator::operator*() const
{
    const auto& indices = ptr->face_vertex_indices[idx];
    return {
        .v1 = ptr->vertices[indices(0)],
        .v2 = ptr->vertices[indices(1)],
        .v3 = ptr->vertices[indices(2)],
        .c1 = ptr->vertex_colors[indices(0)],
        .c2 = ptr->vertex_colors[indices(1)],
        .c3 = ptr->vertex_colors[indices(2)]};
}

}  // namespace raster
