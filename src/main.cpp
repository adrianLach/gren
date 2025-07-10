#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gren/window.hpp>
#include <gren/shader.hpp>
#include <gren/logger.hpp>
#include <gren/matrix.hpp>

int main()
{

    gren::Logger::log("Starting OpenGL Triangle Example");
    gren::Window window(800, 600, "OpenGL Triangle");
    window.makeContextCurrent();

    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f};

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    GLuint indices[] = {0, 1, 2, 2, 1, 3};

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "uniform mat4 model;\n"
                                     "uniform mat4 view;\n"
                                     "uniform mat4 projection;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                                     "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "uniform vec4 color;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = color;\n"
                                       "}\n\0";

    gren::Logger::log("Compiling shaders");
    gren::Shader shader(vertexShaderSource, fragmentShaderSource);

    window.setKeyCallback([&window](int key, int scancode, int action, int mods)
                          {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            window.setShouldClose(true);
        } });

    window.setResizeCallback([](int width, int height)
                             { glViewport(0, 0, width, height); });

    gren::Matrix perpective = gren::Matrix::getPerspectiveMatrix(45.0f, (float)window.getWidth() / (float)window.getHeight(), 0.1f, 100.0f);
    gren::Matrix view;

    float position[] = {0.0f, 0.0f, -3.0f};
    float rotation[] = {0.0f, 0.0f, 0.0f};
    float scale[] = {1.0f, 1.0f, 1.0f};

    glViewport(0, 0, window.getWidth(), window.getHeight());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    gren::Logger::log("Entering main loop");
    while (!window.shouldClose())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        rotation[0] += 0.0001f; // Rotate the triangle
        rotation[1] += 0.001f; // Rotate the triangle
        rotation[2] += 0.01f; // Rotate the triangle

        gren::Matrix model = gren::Matrix::getModelMatrix(position, rotation, scale);

        glUseProgram(shader.getProgram());
        glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, GL_FALSE, model.data());
        glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "view"), 1, GL_FALSE, view.data());
        glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"), 1, GL_FALSE, perpective.data());

        glUniform4f(glGetUniformLocation(shader.getProgram(), "color"), 1.0f, 0.5f, 0.2f, 1.0f);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.swapBuffers();
        window.pollEvents();
    }

    gren::Logger::log("Exiting main loop");

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    gren::Logger::log("Cleaned up OpenGL resources");

    return 0;
}
