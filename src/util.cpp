#include "util.h"

std::size_t IntegerVec3Hasher::operator()(const glm::ivec3& k) const {
    return (
        ((std::hash<int>()(k[0])
        ^ (std::hash<int>()(k[1]) << 1 )) >> 1)
        ^ (std::hash<int>()(k[2]) << 1)
    );
}

// TODO: Figure out why this breaks when debugging the program
std::string loadShaderText(const std::string& name) {
  std::filesystem::path shaderPath = std::filesystem::path("../resources/shaders/");
  shaderPath += name + ".glsl";
  std::string text = readFile(shaderPath.string());
  if (text.empty()) {
    throw(text);
  }
  return text;
}

std::string readFile(std::string_view path) {
    constexpr auto read_size = std::size_t{4096};
    auto stream = std::ifstream{path.data()};
    stream.exceptions(std::ios_base::badbit);
    
    auto out = std::string{};
    auto buf = std::string(read_size, '\0');
    while (stream.read(& buf[0], read_size)) {
        out.append(buf, 0, stream.gcount());
    }
    out.append(buf, 0, stream.gcount());
    return out;
}

GLuint loadBlockTexture(const std::string &name) {
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    std::string path = "../resources/textures/" + name + ".jpg";
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format;
        if (nrChannels == 1) {
            format = GL_RED;
        } else if (nrChannels == 3) {
            format = GL_RGB;
        } else if (nrChannels == 4) {
            format = GL_RGBA;
        } else {
            throw("Invalid number of channels");
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        throw("Failed to load texture");
    }
    stbi_image_free(data);
    return texture;
}

GLuint loadCubeMapTexture(const std::string &name) {
  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, texture);


  int width, height, nrChannels;
  for (int i = 0; i < 6; ++i) {
    std::string path = "../resources/textures/" + name + "_" + std::to_string(i) + ".jpg";
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      if (nrChannels == 3) {
      } else {
        throw("Invalid number of channels");
      }
    } else {
      throw("Failed to load texture");
    }

    
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    stbi_image_free(data);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  return texture;
}