#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
#include <array>
#include <algorithm>

#include "triangle.h"


class Renderer {
    public:
        Renderer();
        void init();
        void render(GLFWwindow* window);
        void createGeometry();


    private:
        std::vector<Triangle> objects;
        GLuint shader_programme;
        GLuint fs;
        GLuint vs;
        GLuint vbo;
        GLuint vao;
        std::array<GLfloat, 9> points;
};

#endif