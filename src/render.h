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
#include <fstream>
#include <filesystem>
#include <glm/matrix.hpp>


#include "block.h"
#include "camera.h"
#include "world.h"

#include <stb_image.h>

class Renderer {
    public:
        Renderer(GLFWwindow& window, Camera& camera, World& world);
        void init();
        void render();

        /// @brief Reads shaderfile from ../resources/shaders/<name>.glsl
        /// @param name Name of the glsl file (without extension)
        /// @return Entire content of that file as an std::string
        static std::string loadShader(const std::string &name);

    private:
        GLFWwindow& window;
        Camera& camera;
        World& world;
        std::vector<Block> objects;
        GLuint shader;
        GLuint fs;
        GLuint vs;
        GLuint vertexBuffer;
        GLuint colorBuffer;
        GLuint textureCoordinatesBuffer;
        GLuint indexBuffer;
        GLuint vao;
        GLuint texture;
        glm::mat4 perspectiveMatrix;
};