#include "exporter.h"

#include "chunk.h"

void exportToOBJ(const std::vector<glm::u8vec3>& vertices, 
                 const std::vector<GLuint>& indices, 
                 const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    // Write vertices (assuming 3 floats per vertex: x, y, z)
    for (const auto& vertex : vertices) {
        out << "v " << static_cast<float>(vertex.x) / (float) Chunk::CHUNK_SIZE << " "
            << static_cast<float>(vertex.y) / (float) Chunk::CHUNK_SIZE << " "
            << static_cast<float>(vertex.z) / (float) Chunk::CHUNK_SIZE << "\n";
    }

    // Write faces (assuming triangles, indices are 0-based in OpenGL, 1-based in OBJ)
    for (size_t i = 0; i < indices.size(); i += 3) {
        out << "f " << indices[i] + 1 << " " 
        << indices[i + 1] + 1 << " " 
        << indices[i + 2] + 1 << "\n";
    }

    out.close();
    std::cout << "Exported mesh to " << filename << std::endl;
}