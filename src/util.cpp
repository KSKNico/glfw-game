#include "util.h"


// TODO: Figure out why this breaks when debugging the program
std::string loadShaderText(const std::string& name) {
    std::filesystem::path shaderPath = std::filesystem::path("../resources/shaders/");
    shaderPath += name + ".glsl";

    if (!std::filesystem::exists(shaderPath)) {
        throw("Shader file not found");
    }

    std::string text = readFile(shaderPath.string());
    if (text.empty()) {
        throw(text);
    }
    return text;
}

std::string readFile(std::string_view path) {
    constexpr auto read_size = std::size_t{4096};
    auto stream = std::ifstream{path.data()};
    stream.exceptions(std::ios_base::badbit);

    auto out = std::string{};
    auto buf = std::string(read_size, '\0');
    while (stream.read(&buf[0], read_size)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}

std::vector<std::string> getSortedFilesInDirectory(const std::filesystem::path& path) {
    std::vector<std::string> fileNames;
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        fileNames.push_back(std::filesystem::path(entry).filename().string());
    }
    std::sort(fileNames.begin(), fileNames.end());
    return fileNames;
}

bool isEdgeVertex(const glm::vec3& position) {
    return position.x == 0 || position.x == Chunk::CHUNK_SIZE || position.y == 0 || position.y == Chunk::CHUNK_SIZE || position.z == 0 || position.z == Chunk::CHUNK_SIZE;
}