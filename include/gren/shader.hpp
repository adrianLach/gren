#pragma once

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iterator>

namespace gren
{
    class Shader
    {
    public:
        Shader(const char *vertexShaderSource, const char *fragmentShaderSource);

        ~Shader();

        GLuint getProgram() const { return m_program; }

        // Static function to load from file
        static Shader loadFromFile(const std::string &vertexPath, const std::string &fragmentPath)
        {
            std::ifstream vertexFile(vertexPath);
            std::ifstream fragmentFile(fragmentPath);
            if (!vertexFile.is_open() || !fragmentFile.is_open())
            {
                throw std::runtime_error("Failed to open shader files: " + vertexPath + " and " + fragmentPath);
            }

            std::string vertexShaderSource((std::istreambuf_iterator<char>(vertexFile)), std::istreambuf_iterator<char>());
            std::string fragmentShaderSource((std::istreambuf_iterator<char>(fragmentFile)), std::istreambuf_iterator<char>());
            return Shader(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
        }

    private:
        GLuint m_program;

        void compileShader(GLuint shader, const char *shaderSource);
    };
}
