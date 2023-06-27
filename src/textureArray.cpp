#include "textureArray.h"

TextureArray::TextureArray(const std::string &directoryName) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D_ARRAY, id);

    std::vector<std::string> fileNames;
    fileNames = getSortedFilesInDirectory(std::filesystem::path("../resources/textures/" + directoryName));

    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, textureSize, textureSize, fileNames.size(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    int layer = 0;

    for (const auto &fileName : fileNames) {
        std::string path = "../resources/textures/" + directoryName + "/" + fileName;

        int width, height, nrChannels;
        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            if (nrChannels == 3) {
            } else {
                throw("Invalid number of channels");
            }
        } else {
            throw("Failed to load texture");
        }

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
        ++layer;
    }
}

TextureArray::~TextureArray() {
    glDeleteTextures(1, &id);
}