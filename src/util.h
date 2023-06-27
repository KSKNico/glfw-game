#pragma once
#include <filesystem>
#include <fstream>
#include <functional>
#include <glm/vec3.hpp>
#include <string>
#include <string_view>
#include <vector>

#include "glad.h"
#include "stb_image.h"

class IntegerVec3Hasher {
   public:
    std::size_t operator()(const glm::ivec3& k) const;
};

/// @brief Reads shaderfile from ../resources/shaders/<name>.glsl
/// @param name Name of the glsl file (without extension)
/// @return Entire content of that file as an std::string
std::string loadShaderText(const std::string& name);

std::string readFile(std::string_view path);

std::vector<std::string> getSortedFilesInDirectory(const std::filesystem::path& path);
