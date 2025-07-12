#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gren/window.hpp>
#include <gren/shader.hpp>
#include <gren/logger.hpp>
#include <gren/matrix.hpp>
#include <gren/mesh.hpp>
#include <gren/texture.h>

int main()
{

    gren::Logger::log("Starting OpenGL Triangle Example");
    gren::Window window(800, 600, "OpenGL Triangle");
    window.makeContextCurrent();

    gren::Mesh mesh = gren::loadFromFileOBJ("monkey.obj");

    gren::Logger::log("OpenGL resources initialized");

    gren::Logger::log("Compiling shaders");
    gren::Shader shader = gren::Shader::loadFromFile("res/shaders/shader.vert", "res/shaders/shader.frag");

    gren::Texture texture;
    texture.loadFromFile("res/skin.jpg");

    window.setKeyCallback([](int key, int scancode, int action, int mods)
                          {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            gren::Logger::log("Escape key pressed, closing window");
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        } });

    window.setResizeCallback([&window](int width, int height)
                             { 
                                glViewport(0, 0, width, height);
                                window.setSize(width, height);
                                gren::Logger::log("Window resized to: " + std::to_string(width) + "x" + std::to_string(height)); });

    float position[] = {0.0f, 0.0f, -6.0f};
    float rotation[] = {0.0f, 0.0f, 0.0f};
    float scale[] = {0.9f, 0.9f, 0.9f};

    glViewport(0, 0, window.getWidth(), window.getHeight());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    gren::Logger::log("Entering main loop");
    while (!window.shouldClose())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rotation[1] += 0.001f; // Rotate around Y-axis
        rotation[2] += 0.001f; // Rotate around Y-axis

        gren::Matrix perpective = gren::Matrix::getPerspectiveMatrix(45.0f, (float)window.getWidth() / (float)window.getHeight(), 0.1f, 100.0f);
        gren::Matrix view;
        gren::Matrix model = gren::Matrix::getModelMatrix(position, rotation, scale);

        glUseProgram(shader.getProgram());
        glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "view"), 1, GL_FALSE, view.data());
        glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"), 1, GL_FALSE, perpective.data());
        glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, GL_FALSE, model.data());

        texture.bind();
        glUniform1i(glGetUniformLocation(shader.getProgram(), "ourTexture"), 0);

        glUniform4f(glGetUniformLocation(shader.getProgram(), "color"), 0.5f, 0.5f, 0.5f, 1.0f);

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.size, GL_UNSIGNED_INT, 0);

        window.swapBuffers();
        window.pollEvents();
    }

    gren::Logger::log("Exiting main loop");

    gren::Logger::log("Cleaned up OpenGL resources");

    return 0;
}
