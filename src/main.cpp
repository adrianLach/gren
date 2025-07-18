#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gren/window.hpp>
#include <gren/shader.hpp>
#include <gren/logger.hpp>
#include <gren/matrix.hpp>
#include <gren/mesh.hpp>
#include <gren/texture.h>
#include <gren/vector.h>
#include <thread>
#include <chrono>

using namespace gren;

int main()
{

    Logger::log("Starting OpenGL Triangle Example");
    Window window(800, 600, "OpenGL Triangle");
    window.makeContextCurrent();

    Mesh mesh = loadFromFileOBJ("res/ball.obj");

    Logger::log("OpenGL resources initialized");

    Logger::log("Compiling shaders");
    Shader shader = Shader::loadFromFile("res/shaders/shader.vert", "res/shaders/shader.frag");

    // Get uniform locations once and store them
    GLint viewLoc = glGetUniformLocation(shader.getProgram(), "view");
    GLint projLoc = glGetUniformLocation(shader.getProgram(), "projection");
    GLint modelLoc = glGetUniformLocation(shader.getProgram(), "model");
    GLint texLoc = glGetUniformLocation(shader.getProgram(), "diffuseMap");
    GLint normalLoc = glGetUniformLocation(shader.getProgram(), "normalMap");
    GLint colorLoc = glGetUniformLocation(shader.getProgram(), "color");
    GLint lightPosLoc = glGetUniformLocation(shader.getProgram(), "lightPos");

    Texture texture;
    texture.loadFromFile("res/grayrocks/diffuse.png");

    Texture normal;
    normal.loadFromFile("res/grayrocks/normal.png");

    Texture height;
    height.loadFromFile("res/grayrocks/height.png");

    window.setKeyCallback([](int key, int scancode, int action, int mods)
                          {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            Logger::log("Escape key pressed, closing window");
            glfwSetWindowShouldClose(glfwGetCurrentContext(), true);
        } });

    window.setResizeCallback([&window](int width, int height)
                             { 
                                if (width > 0 && height > 0) {
                                   glViewport(0, 0, width, height);
                                   window.setSize(width, height);
                                   Logger::log("Window resized to: " + std::to_string(width) + "x" + std::to_string(height));
                                }
                             });

    auto toRadians = [](float degrees) -> float {
        return degrees * (3.14159265358979323846f / 180.0f);
    };

    Vector position(0.0f, 0.0f, 0.0f);
    Vector rotation(0.0f, 0.0f, 0.0f);
    Vector scale(1.0f, 1.0f, 1.0f);

    struct Camera
    {
        Vector position;
        Vector rotation;
        Camera() : position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f) {}
    };

    Camera camera;

    camera.position = Vector(0.0f, 0.0f, 3.0f);

    glViewport(0, 0, window.getWidth(), window.getHeight());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Frame limiting configuration
    const bool USE_FRAME_LIMITER = true; // Set to true to enable manual frame limiting
    const double TARGET_FPS = 144.0; // Target framerate when frame limiter is enabled
    const double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;

    double lastTime = glfwGetTime();
    double deltaTime = 0.0;
    int frames = 0;
    double currentTime;
    double fps = 0.0;
    double fpsUpdateInterval = 1.0; // Update FPS every second
    double lastFpsUpdate = lastTime;

    Logger::log("Entering main loop");
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
            Logger::log("FPS: " + std::to_string(fps));
        }

        window.setTitle("OpenGL Triangle - FPS: " + std::to_string(fps) + " - Delta Time: " + std::to_string(deltaTime));

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        rotation.y = toRadians(20.0f * sin(currentTime / 2.0f) - 20.0f);
        rotation.x = toRadians(20.0f * cos(currentTime / 2.0f));

        Matrix perpective = Matrix::getPerspectiveMatrix(45.0f, (float)window.getWidth() / (float)window.getHeight(), 0.1f, 100.0f);
        // Matrix::getOrthographicMatrix(
        //     -1.0f, 1.0f, 
        //     -1.0f * (float)window.getHeight() / (float)window.getWidth(), 
        //     1.0f * (float)window.getHeight() / (float)window.getWidth(), 
        //     0.1f, 100.0f
        // );
        Matrix view = Matrix::getModelMatrix(-camera.position, -camera.rotation, Vector(1.0f, 1.0f, 1.0f));
        Matrix model = Matrix::getModelMatrix(position, rotation, scale);

        glUseProgram(shader.getProgram());
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, perpective);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model);

        glUniform3fv(glGetUniformLocation(shader.getProgram(), "viewPos"), 1, -camera.position);

        glActiveTexture(GL_TEXTURE0);
        texture.bind();
        glUniform1i(texLoc, 0);

        glActiveTexture(GL_TEXTURE1);
        normal.bind();
        glUniform1i(normalLoc, 1);

        glActiveTexture(GL_TEXTURE2);
        height.bind();
        glUniform1i(glGetUniformLocation(shader.getProgram(), "heightMap"), 2);

        glUniform4f(colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);
        //light Pos
        glUniform3f(lightPosLoc, 30.0f, 30.0f, 30.0f);

        glBindVertexArray(mesh.VAO);
        glDrawElements(GL_TRIANGLES, mesh.size, GL_UNSIGNED_INT, 0);

        window.swapBuffers();
        window.pollEvents();

        // Manual frame limiting (if enabled)
        if (USE_FRAME_LIMITER) {
            double frameEndTime = glfwGetTime();
            double frameTime = frameEndTime - currentTime;
            
            if (frameTime < TARGET_FRAME_TIME) {
                double sleepTime = TARGET_FRAME_TIME - frameTime;
                // Use busy wait for more precise timing (alternative: std::this_thread::sleep_for)
                double targetTime = frameEndTime + sleepTime;
                while (glfwGetTime() < targetTime) {
                    // Busy wait - more CPU intensive but more precise
                    // For less CPU usage, replace with:
                    // std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime * 0.9));
                }
            }
        }
    }

    Logger::log("Exiting main loop");

    Logger::log("Cleaned up OpenGL resources");

    return 0;
}
