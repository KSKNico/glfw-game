#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <filesystem>
#include <iostream>
#include <string>

#include "stb_image.h"
#include "util.h"

class TextureCubemap {
   public:
    TextureCubemap(const std::string& directoryName);
    ~TextureCubemap();

    GLuint id;
};