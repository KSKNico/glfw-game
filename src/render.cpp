#include "render.h"
#include "shader.h"

Renderer::Renderer() {};

void Renderer::createGeometry() {
    objects.push_back(
        Triangle{
            std::array<GLfloat, 3>{1.0, 0.0, 0.0},
            std::array<GLfloat, 3>{0.5, 0.0, 1.0},
            std::array<GLfloat, 3>{0.0, 0.0, 1.0},
        }
    );

    GLfloat tmp[] = { 
        0.0f,  0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f
    };

    for (int i = 0; i < 9; i++) {
        points[0] = tmp[0];
    }
}


void Renderer::init() {
    createGeometry();


    // Shaders
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader_text, NULL);
    glCompileShader(vs);
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader_text, NULL);
    glCompileShader(fs);

    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), &points[0], GL_STATIC_DRAW);

    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

}

void Renderer::render(GLFWwindow* window) {
  // wipe the drawing surface clear
  glClearColor(0.2, 0.3, 0.2, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(shader_programme);
  glBindVertexArray(vao);
  // draw points 0-3 from the currently bound VAO with current in-use shader
  glDrawArrays(GL_TRIANGLES, 0, 3);

}