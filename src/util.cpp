#include "util.h"

std::size_t IntegerVec3Hasher::operator()(const glm::ivec3& k) const {
    return (
        ((std::hash<int>()(k[0])
        ^ (std::hash<int>()(k[1]) << 1 )) >> 1)
        ^ (std::hash<int>()(k[2]) << 1)
    );
}

// TODO: Figure out why this breaks when debugging the program
std::string loadShader(const std::string& name) {
  std::filesystem::path shaderPath = std::filesystem::path("../resources/shaders/");
  shaderPath += name + ".glsl";
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
    while (stream.read(& buf[0], read_size)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}