#include "util.h"

std::size_t IntegerVec3Hasher::operator()(const glm::ivec3& k) const {
    return (
        ((std::hash<int>()(k[0])
        ^ (std::hash<int>()(k[1]) << 1 )) >> 1)
        ^ (std::hash<int>()(k[2]) << 1)
    );
}

std::string loadShader(const std::string& name) {
  std::filesystem::path shaderPath = std::filesystem::path("../resources/shaders/");
  shaderPath += name + ".glsl";
  std::ifstream ifs( shaderPath.c_str());
  std::string text((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()) );
  return text;
}