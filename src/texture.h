#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "util.h"

class Texture {
   public:
    Texture(const std::string& fileName);
    ~Texture();

    GLuint id;
};