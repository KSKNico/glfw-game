#include "glad.h"
#include <vector>
#include <glm/matrix.hpp>


class Skybox {
    public:
        GLuint vao;
        GLuint vertexBuffer;
        GLuint indexBuffer;

        std::vector<glm::vec3> vertexPositions;
        std::vector<GLubyte> indices;

        Skybox();
 
        void createVAO();
};