#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
#include <array>
#include <algorithm>
#include <memory>
#include <iostream>
#include <string>
#include <glm/matrix.hpp>


#include "block.h"
#include "camera.h"
#include "world.h"

#include "stb_image.h"

class Renderer {
    public:
        Renderer(GLFWwindow& window, Camera& camera, World& world);
        void init();
        void render();

        void setPerspectiveMatrix(int width, int height);

 

    private:
        GLFWwindow& window;
        Camera& camera;
        World& world;
        std::vector<Block> objects;
        GLuint blockShader;
        GLuint skyboxShader;
        GLuint fs;
        GLuint vs;
        GLuint vertexBuffer;
        GLuint colorBuffer;
        GLuint textureCoordinatesBuffer;
        GLuint vao;
        GLuint blockTexture;
        GLuint skyboxTexture;
        glm::mat4 perspectiveMatrix;
};