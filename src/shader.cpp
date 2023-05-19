#include "shader.h"

Shader::Shader(const std::string& vertexName, const std::string& fragmentName) {
    std::string vertexShaderText = loadShaderText(vertexName);
    std::string fragmentShaderText = loadShaderText(fragmentName);
    const GLchar* source;
    int isCompiled = 0;
    GLuint vs;
    GLuint fs;
    GLint programLinked;


    source = (GLchar *) vertexShaderText.c_str();
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &source, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
      {
        GLint maxLength = 0;
        glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(vs, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the shader anymore.
        glDeleteShader(vs);

        for (auto c: infoLog) {
          std::cout << c;
        }
        return;
      }

    source = (GLchar *) fragmentShaderText.c_str();
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &source, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
      {
        GLint maxLength = 0;
        glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(fs, maxLength, &maxLength, &infoLog[0]);
        
        // We don't need the shader anymore.
        glDeleteShader(fs);

        for (auto c: infoLog) {
          std::cout << c;
        }
        
        return;
      }

    id = glCreateProgram();
    glAttachShader(id, fs);
    glAttachShader(id, vs);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &programLinked);
    if (programLinked != GL_TRUE)
    {
      GLsizei log_length = 0;
      GLchar message[1024];
      glGetProgramInfoLog(id, 1024, &log_length, message);
      for (auto c: message) {
        std::cout << c;
      }
    }
}

Shader::~Shader() {
  glDeleteProgram(id);
}

void Shader::use() {
  glUseProgram(id);
}