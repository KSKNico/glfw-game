#pragma once

#include "glad.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <algorithm>
#include <array>
#include <glm/matrix.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "block.h"
#include "camera.h"
#include "shader.h"
#include "stb_image.h"
#include "textureArray.h"
#include "textureCubemap.h"
#include "world.h"

class Renderer {
   public:
    Renderer(GLFWwindow& window, Camera& camera, World& world);
    void init();
    void render();
    void drawSkybox();
    void drawBlocks();
    void drawChunk(Chunk& chunk, const glm::mat4& viewProjectionMatrix);

    void setPerspectiveMatrix(int width, int height);

   private:
    GLFWwindow& window;
    Camera& camera;
    World& world;
    std::vector<Block> objects;
    TextureCubemap skyboxTexture;
    TextureArray blockTextures;
    Shader blockShader;
    Shader skyboxShader;

    glm::mat4 perspectiveMatrix;
};