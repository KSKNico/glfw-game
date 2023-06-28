#include "shader.h"

Shader::Shader(const std::string& vertexName, const std::string& fragmentName) {
    std::string vertexShaderText = loadShaderText(vertexName);
    std::string fragmentShaderText = loadShaderText(fragmentName);

    GLuint vs;
    GLuint fs;
    GLint programLinked;

    vs = createShader(GL_VERTEX_SHADER, vertexShaderText);
    fs = createShader(GL_FRAGMENT_SHADER, fragmentShaderText);

    id = glCreateProgram();
    glAttachShader(id, fs);
    glAttachShader(id, vs);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &programLinked);
    if (programLinked != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetProgramInfoLog(id, 1024, &log_length, message);
        for (auto c : message) {
            std::cout << c;
        }
    }
}

GLint Shader::createShader(const GLint type, const std::string& text) {
    GLchar* source = (GLchar*)text.c_str();
    GLint shaderId = glCreateShader(type);
    glShaderSource(shaderId, 1, &source, NULL);
    glCompileShader(shaderId);

    int isCompiled = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

        // We don't need the shader anymore.
        glDeleteShader(shaderId);

        for (auto c : infoLog) {
            std::cout << c;
        }
        return 0;
    }
    return shaderId;
}

GLint Shader::getUniformLocation(const std::string& name) {
    // if the uniformLocations map already contains this uniform, return it,
    // otherwise get it from the shader, add it to the map and return it
    if (uniformLocations.find(name) != uniformLocations.end()) {
        return uniformLocations[name];
    } else {
        GLint location = glGetUniformLocation(id, name.c_str());
        uniformLocations[name] = location;
        return location;
    }
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::use() const {
    glUseProgram(id);
}