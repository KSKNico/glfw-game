#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <array>


class Triangle {
    public:
        Triangle(std::array<GLfloat, 3> v1, std::array<GLfloat, 3> v2, std::array<GLfloat, 3> v3);

        void render();
    private:
        std::array<GLfloat, 3> v1;
        std::array<GLfloat, 3> v2;
        std::array<GLfloat, 3> v3;
};