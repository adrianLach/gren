#include <iostream>
#include <fstream>
#include "grn/shader.h"

namespace grn
{

    Shader::Shader(const char *vertexShaderSource, const char *fragmentShaderSource)
    {

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        compileShader(vertexShader, vertexShaderSource);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        compileShader(fragmentShader, fragmentShaderSource);

        m_program = glCreateProgram();
        glAttachShader(m_program, vertexShader);
        glAttachShader(m_program, fragmentShader);
        glLinkProgram(m_program);

        // Check for linking errors
        GLint success;
        GLchar infoLog[512];
        glGetProgramiv(m_program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(m_program, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                      << infoLog << std::endl;
        }

        // Clean up shaders as they're linked into the program now
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_program);
    }

    void Shader::compileShader(GLuint shader, const char *shaderSource)
    {
        glShaderSource(shader, 1, &shaderSource, NULL);
        glCompileShader(shader);

        // Check for compilation errors
        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
                      << infoLog << std::endl;
        }
    }

}