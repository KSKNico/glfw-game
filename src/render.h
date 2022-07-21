#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vector>
#include <array>
#include <algorithm>

#include "triangle.h"




std::vector<Triangle> objects;

extern GLuint shader_programme;

extern GLuint fs;

extern GLuint vs;

extern GLuint vbo;

extern GLuint vao;

std::array<GLfloat, 9> points;

void init();

void render(GLFWwindow* window);

void createGeometry();

#endif