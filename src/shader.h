#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include "util.h"


class Shader {
public:
    GLuint id;
    Shader(const std::string& vertexName, const std::string& fragmentName);
};