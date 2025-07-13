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

    gren::Mesh mesh = gren::loadFromFileOBJ("res/monkey.obj");

    gren::Logger::log("OpenGL resources initialized");

    gren::Logger::log("Compiling shaders");
    gren::Shader shader = gren::Shader::loadFromFile("res/shaders/shader.vert", "res/shaders/shader.frag");

    // Get uniform locations once and store them
    GLint viewLoc = glGetUniformLocation(shader.getProgram(), "view");
    GLint projLoc = glGetUniformLocation(shader.getProgram(), "projection");
    GLint modelLoc = glGetUniformLocation(shader.getProgram(), "model");
    GLint texLoc = glGetUniformLocation(shader.getProgram(), "diffuseMap");
    GLint normalLoc = glGetUniformLocation(shader.getProgram(), "normalMap");
    GLint colorLoc = glGetUniformLocation(shader.getProgram(), "color");
    GLint lightPosLoc = glGetUniformLocation(shader.getProgram(), "lightPos");

    gren::Texture texture;
    texture.loadFromFile("res/rocky/diffuse.png");

    gren::Texture normal;
    normal.loadFromFile("res/rocky/normal.png");

    window.setKeyCallback([](int key, int scancode, int action, int mods)
                          {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            gren::Logger::log("Escape key pressed, closing window");
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        } });

    window.setResizeCallback([&window](int width, int height)
                             { 
                                if (width > 0 && height > 0) {
                                   glViewport(0, 0, width, height);
                                   window.setSize(width, height);
                                   gren::Logger::log("Window resized to: " + std::to_string(width) + "x" + std::to_string(height));
                                }
                             });

    float position[] = {0.0f, 0.0f, -3.0f};
    float rotation[] = {0.0f, 0.0f, 0.0f};
    float scale[] = {0.9f, 0.9f, 0.9f};

    glViewport(0, 0, window.getWidth(), window.getHeight());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    double lastTime = glfwGetTime();
    double deltaTime = 0.0;
    int frames = 0;
    double currentTime;
    double fps = 0.0;
    double fpsUpdateInterval = 1.0; // Update FPS every second
    double lastFpsUpdate = lastTime;

    auto toRadians = [](float degrees) -> float {
        return degrees * (3.14159265358979323846f / 180.0f);
    };

    gren::Logger::log("Entering main loop");
    while (!window.shouldClose())
    {
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        frames++;
        if (currentTime - lastFpsUpdate >= fpsUpdateInterval)
        {
            fps = frames / (currentTime - lastFpsUpdate);
            lastFpsUpdate = currentTime;
            frames = 0;
            gren::Logger::log("FPS: " + std::to_string(fps));
        }

        window.setTitle("OpenGL Triangle - FPS: " + std::to_string(fps) + " - Delta Time: " + std::to_string(deltaTime));

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rotation[1] += toRadians(5.0f) * (float)deltaTime; // Rotate 50 degrees per second around Y-axis

        gren::Matrix perpective = gren::Matrix::getPerspectiveMatrix(45.0f, (float)window.getWidth() / (float)window.getHeight(), 0.1f, 100.0f);
        // gren::Matrix::getOrthographicMatrix(
        //     -1.0f, 1.0f, 
        //     -1.0f * (float)window.getHeight() / (float)window.getWidth(), 
        //     1.0f * (float)window.getHeight() / (float)window.getWidth(), 
        //     0.1f, 100.0f
        // );
        gren::Matrix view;
        gren::Matrix model = gren::Matrix::getModelMatrix(position, rotation, scale);

        glUseProgram(shader.getProgram());
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.data());
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, perpective.data());
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.data());

        glActiveTexture(GL_TEXTURE0);
        texture.bind();
        glUniform1i(texLoc, 0);

        glActiveTexture(GL_TEXTURE1);
        normal.bind();
        glUniform1i(normalLoc, 1);

        glUniform4f(colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);
        //light Pos
        glUniform3f(lightPosLoc, -3.0f, -3.0f, 3.0f);

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.size, GL_UNSIGNED_INT, 0);

        window.swapBuffers();
        window.pollEvents();
    }

    gren::Logger::log("Exiting main loop");

    gren::Logger::log("Cleaned up OpenGL resources");

    return 0;
}
