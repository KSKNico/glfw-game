#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include "glad.h"


void exportToOBJ(const std::vector<glm::u8vec3>& vertices, 
                 const std::vector<GLuint>& indices, 
                 const std::string& filename);