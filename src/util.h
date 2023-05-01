#pragma once
#include "glad.h"
#include "stb_image.h"

#include <functional>
#include <glm/vec3.hpp>
#include <string>
#include <fstream>
#include <filesystem>
#include <string_view>


class IntegerVec3Hasher {
    public:
        std::size_t operator() (const glm::ivec3& k) const;
};

/// @brief Reads shaderfile from ../resources/shaders/<name>.glsl
/// @param name Name of the glsl file (without extension)
/// @return Entire content of that file as an std::string
std::string loadShader(const std::string &name);

std::string readFile(std::string_view path);

GLuint loadBlockTexture(const std::string &name);

GLuint loadCubeMapTexture(const std::string &name);