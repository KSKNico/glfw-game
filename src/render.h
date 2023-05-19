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
#include "shader.h"
#include "textureCubemap.h"
#include "texture.h"

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
        TextureCubemap skyboxTexture;
        Texture blockTexture;
        Shader blockShader;
        Shader skyboxShader;

        glm::mat4 perspectiveMatrix;
};