#include "cubemapTexture.h"

CubemapTexture::CubemapTexture(const std::string& directoryName) { 
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    std::array<std::string, 6> fileNames;
    int i = 0;
    std::string path = "../resources/textures/" + directoryName; // + "_" + std::to_string(i) + ".jpg";
    for (const auto & entry : std::filesystem::directory_iterator(path)) {
        fileNames[i] = entry.path().filename().string();
        ++i;
    }

    if (i != 6) {
        throw("Invalid number of files");
    }

    // sorts the 6 file names in ascending order
    std::sort(fileNames.begin(), fileNames.end());

    int width, height, nrChannels;
    for (int i = 0; i < 6; ++i) {
        std::string path = "../resources/textures/" + directoryName + "/" + fileNames[i];
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
}

CubemapTexture::~CubemapTexture() {
    glDeleteTextures(1, &id);
}