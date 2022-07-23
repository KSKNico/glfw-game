#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/matrix.hpp>


class Triangle {
    public:
        Triangle();
        glm::mat3 points;
        glm::mat3 colors;
        void render();
    private:
};