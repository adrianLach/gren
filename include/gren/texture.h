#pragma once

#include <GL/glew.h>
#include <string>

namespace gren
{
    class Texture
    {
    public:
        Texture();
        ~Texture();

        void loadFromFile(const std::string& filePath);
        void bind() const;

    private:
        GLuint m_ID;
        int m_width;
        int m_height;
        int m_channelCount;

        void loadTextureFromFile(const std::string &filePath);
    };
}