#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>
#include <array>
#include <filesystem>
#include <algorithm>

#include "util.h"

class TextureCubemap {
public:
    TextureCubemap(const std::string& directoryName);
    ~TextureCubemap();

    GLuint id;
};