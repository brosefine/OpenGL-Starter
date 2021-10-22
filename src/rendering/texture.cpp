#define STB_IMAGE_IMPLEMENTATION

#include <rendering/texture.h>
#include <helpers/RootDir.h>
#include <stb_image.h>

#include <iostream>
#include <glad/glad.h>

Texture::Texture(std::string filename, bool srgb): texId_(0) {
    glGenTextures(1, &texId_);
    glBindTexture(GL_TEXTURE_2D, texId_);

    int width, height, nrComponents;

    std::string path;
    path = ROOT_DIR "resources/textures/" + filename;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {

        GLenum format = GL_RED, internalFormat = GL_RED;
        if (nrComponents == 3) {
            format = GL_RGB;
            internalFormat = srgb ? GL_SRGB : GL_RGB;
        }
        else if (nrComponents == 4) {
            format = GL_RGBA;
            internalFormat = GL_RGBA;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height,
            0, format, GL_UNSIGNED_BYTE, data);
    } else {
        std::cerr << "[loadTexture] Texture failed to load: " << path << std::endl;
    }
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}



Texture::~Texture() {
    glDeleteTextures(1, &texId_);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, texId_);
}

FBOTexture::FBOTexture(int width, int height)
    : width_(width)
    , height_(height)
    , texId_(0)
    , fboId_(0) {

    // framebuffer
    glGenFramebuffers(1, &fboId_);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId_);

    // texture
    glGenTextures(1, &texId_);
    glBindTexture(GL_TEXTURE_2D, texId_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
        GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        texId_, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fboId_ = -1; texId_ = -1;
        std::cerr << "[Texture]: Framebuffer object not initialized" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FBOTexture::~FBOTexture() {
    glDeleteFramebuffers(1, &fboId_);
    glDeleteTextures(1, &texId_);
}

void FBOTexture::resize(int width, int height) {
    width_ = width;
    height_ = height;

    glBindFramebuffer(GL_FRAMEBUFFER, fboId_);

    glDeleteTextures(1, &texId_);

    glGenTextures(1, &texId_);
    glBindTexture(GL_TEXTURE_2D, texId_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
        GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
        texId_, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fboId_ = -1; texId_ = -1;
        std::cerr << "[Texture]: Framebuffer object not initialized" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBOTexture::bindImageTex(int binding, unsigned int mode) const {
    glBindImageTexture(binding, texId_, 0, GL_FALSE, 0, mode, GL_RGBA32F);
}

void FBOTexture::bindTex() const {
    glBindTexture(GL_TEXTURE_2D, texId_);
}

CubeMap::CubeMap(std::vector<std::string> faces): ID_(0) {
    loadImages(faces);
}

void CubeMap::bind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID_);
}

void CubeMap::loadImages(std::vector<std::string> faces) {
    if (faces.size() != 6) {
        std::cerr << "[laodCubeMap] Invalid number of cube map textures!" << std::endl;
    }

    glGenTextures(1, &ID_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID_);

    int width, height, nrComponents;

    std::string path;
    for (int i = 0; i < faces.size(); ++i) {
        path = ROOT_DIR "resources/textures/" + faces.at(i);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
        if (data) {

            GLenum format = GL_RED;
            if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height,
                0, format, GL_UNSIGNED_BYTE, data);
        } else {
            std::cerr << "[laodCubeMap] Cubemap failed to load: " << path << std::endl;
        }
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

