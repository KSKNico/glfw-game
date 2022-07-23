#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
#include <array>
#include <algorithm>
#include <glm/matrix.hpp>

#include "triangle.h"


class Renderer {
    public:
        Renderer(GLFWwindow* window);
        void init();
        void render();
        void createGeometry();


    private:
        GLFWwindow* window;
        std::vector<Triangle> objects;
        GLuint shader_programme;
        GLuint fs;
        GLuint vs;
        GLuint vertexBuffer;
        GLuint colorBuffer;
        GLuint vao;
        glm::mat3 points;
        glm::mat3 colors;
};

#endif