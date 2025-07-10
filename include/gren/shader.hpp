#pragma once

#include <GL/glew.h>

namespace gren {
class Shader {
public:
    Shader(const char* vertexShaderSource, const char* fragmentShaderSource);

    ~Shader();

    GLuint getProgram() const { return m_program; }
private:
    GLuint m_program;

    void compileShader(GLuint shader, const char* shaderSource);
};
}
