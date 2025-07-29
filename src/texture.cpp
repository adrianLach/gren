#include "grn/texture.h"
#include "grn/logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb/stb_image.h"

#include <GL/glew.h>

grn::Texture::Texture() : m_ID(0), m_width(0), m_height(0), m_channelCount(0)
{
}

grn::Texture::~Texture()
{
    glDeleteTextures(1, &m_ID);
}

void grn::Texture::loadFromFile(const std::string& filePath)
{
    loadTextureFromFile(filePath);
}

void grn::Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void grn::Texture::loadTextureFromFile(const std::string &filePath)
{
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filePath.c_str(), &m_width, &m_height, &m_channelCount, 0);
    if (data)
    {
        GLenum format;
        if (m_channelCount == 1)
            format = GL_RED;
        else if (m_channelCount == 3)
            format = GL_RGB;
        else if (m_channelCount == 4)
            format = GL_RGBA;

        glGenTextures(1, &m_ID);
        glBindTexture(GL_TEXTURE_2D, m_ID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        grn::Logger::error("Texture failed to load at path: " + filePath);
    }
}