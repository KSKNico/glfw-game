#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <iostream>
#include <map>
#include <string>

#include "util.h"

class Shader {
   public:
    GLuint id;
    GLint createShader(const GLint type, const std::string& text);
    GLint getUniformLocation(const std::string& name);
    Shader(const std::string& vertexName, const std::string& fragmentName);
    ~Shader();

    void use() const;

   private:
    std::map<std::string, GLint> uniformLocations;
};