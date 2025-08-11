#pragma once

#include <GL/glew.h>
#include <string>
#include "grn/vector.h"

namespace grn
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        void loadFromFile(const std::string &filePath);
        void createColorTexture(const Vector2i &size, unsigned int format = GL_RGBA, unsigned int type = GL_UNSIGNED_BYTE);
        void createDepthTexture(const Vector2i &size);

        void bind() const;
        void unbind() const;
        GLuint getID() const { return m_ID; }

    private:
        GLuint m_ID;
        int m_width;
        int m_height;
        int m_channelCount;

        void loadTextureFromFile(const std::string &filePath);
    };
}