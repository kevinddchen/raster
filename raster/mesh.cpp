#include <raster/mesh.hpp>

#include <raster/io.hpp>


namespace raster
{

Mesh::Mesh(
    std::vector<Eigen::Vector3f>&& vertices,
    std::vector<Eigen::Array3f>&& vertex_colors,
    std::vector<Eigen::Array3i>&& face_vertex_indices)
    : vertices(vertices),
      vertex_colors(vertex_colors),
      face_vertex_indices(face_vertex_indices)
{
    assert(vertices.size() == vertex_colors.size());
}

Mesh::Mesh(const char* obj)
{
    const std::vector<std::string> lines = io::read_lines(obj);
    for (const auto& line : lines) {
        const std::vector<std::string> parts = io::split(line, " ");
        if (parts.size() == 0) {
            continue;
        }
        if (parts[0] == "v") {
            assert(parts.size() >= 7);
            vertices.emplace_back(std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3]));
            vertex_colors.emplace_back(std::stof(parts[4]), std::stof(parts[5]), std::stof(parts[6]));
        } else if (parts[0] == "f") {
            assert(parts.size() >= 4);
            face_vertex_indices.emplace_back(std::stoi(parts[1]), std::stoi(parts[2]), std::stoi(parts[3]));
        }
    }
}

Mesh::Mesh(Mesh&& other)
    : vertices(std::move(other.vertices)),
      vertex_colors(std::move(other.vertex_colors)),
      face_vertex_indices(std::move(other.face_vertex_indices))
{
}

Mesh& Mesh::operator=(Mesh&& other)
{
    vertices = std::move(other.vertices);
    vertex_colors = std::move(other.vertex_colors);
    face_vertex_indices = std::move(other.face_vertex_indices);
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
