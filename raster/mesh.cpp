#include <raster/mesh.hpp>


namespace raster
{

Mesh::Mesh(
    std::vector<Eigen::Vector3f>&& vertices,
    std::vector<Eigen::Array3i>&& face_vertex_indices,
    std::optional<std::vector<short>>&& colors)
    : vertices(vertices),
      face_vertex_indices(face_vertex_indices)
{
    if (colors.has_value()) {
        assert(face_vertex_indices.size() == colors.value().size());
        this->colors = colors.value();
    } else {
        this->colors = std::vector<short>(face_vertex_indices.size(), COLOR_WHITE);
    }
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
    return {ptr->vertices[indices(0)], ptr->vertices[indices(1)], ptr->vertices[indices(2)], ptr->colors[idx]};
}

}  // namespace raster
