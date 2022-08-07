#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
#include <algorithm>
#include <glm/matrix.hpp>

#include "triangle.h"
#include "camera.h"
// #include "shader.h"

class Renderer {
    public:
        Renderer(GLFWwindow& window, Camera& camera);
        void init();
        void render();
        void createGeometry();


    private:
        GLFWwindow& window;
        Camera& camera;
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