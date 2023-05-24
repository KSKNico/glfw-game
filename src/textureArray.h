#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "util.h"

#include <string>
#include <vector>

class TextureArray {
    public:
    TextureArray(const std::string& directoryName);
    ~TextureArray();

    GLuint id;
    constexpr static unsigned int textureSize = 1024;
};