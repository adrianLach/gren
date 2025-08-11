#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <grn/window.h>
#include <grn/shader.h>
#include <grn/logger.h>
#include <grn/matrix.h>
#include <grn/texture.h>
#include <grn/vector.h>
#include <thread>
#include <chrono>
#include <grn/entity.h>
#include <grn/render.h>

#define checkError(a)                                              \
    do                                                             \
    {                                                              \
        a;                                                         \
        GLenum err = glGetError();                                 \
        if (err != GL_NO_ERROR)                                    \
        {                                                          \
            Logger::error("OpenGL error: " + std::to_string(err)); \
            break;                                                 \
        }                                                          \
    } while (false);

using namespace grn;

int main(int argc, char *argv[])
{
    // Process command line arguments
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        // Example: check for specific arguments
        if (arg == "--help" || arg == "-h")
        {
            std::cout << "Usage: " << argv[0] << " [options]\n";
            std::cout << "Options:\n";
            std::cout << "  --help, -h     Show this help message\n";
            return 0;
        }
        if (arg == "--debug" || arg == "-d")
        {
            Logger::setDebugEnabled(true);
            Logger::log("Debug mode enabled");
        }
    }

    Window window(800, 450, "Rouge Like");
    window.makeContextCurrent();

    Mesh mesh = loadFromFileOBJ("res/ball.obj");
    Mesh screenMesh = getScreenPlane();

    Logger::log("OpenGL resources initialized");

    Logger::log("Compiling shaders");
    Shader shader = Shader::loadFromFile("res/shaders/shader.vert", "res/shaders/shader.frag");
    Shader screenShader = Shader::loadFromFile("res/shaders/screen.vert", "res/shaders/screen.frag");

    // Get uniform locations once and store them
    GLint colorLoc = glGetUniformLocation(shader.getProgram(), "color");
    GLint lightPosLoc = glGetUniformLocation(shader.getProgram(), "lightPos");

    Texture texture;
    texture.loadFromFile("res/rock/diffuse.png");

    Texture normal;
    normal.loadFromFile("res/rock/normal.png");

    Texture height;
    height.loadFromFile("res/rock/height.png");

    window.setCallbacks();

    auto toRadians = [](float degrees) -> float
    {
        return degrees * (3.14159265358979323846f / 180.0f);
    };

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

    int toggleState = 0;
    std::string toggleStates[] = {
        "Position",
        "Color",
        "Tangent",
        "Bitangent",
        "Normal",
        "Diffuse",
        "Specular",
        // "Depth",
        // "Combined",
    };

    Entity entity;
    entity.position = Vector3f(0.0f, 0.0f, 0.0f);
    entity.rotation = Vector3f(0.0f, 0.0f, 0.0f);
    entity.scale = Vector3f(1.0f, 1.0f, 1.0f);

    entity.addComponent(new MeshComponent(mesh));
    entity.addComponent(new MaterialComponent(texture, normal, height));

    Camera camera(Vector3f(0.0f, 0.0f, -2.0f), Vector3f(0.0f, 0.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f));

    unsigned int scale = 100;
    FramebufferDeferred framebuffer(Vector2i(16 * scale, 9 * scale));
    EntityRenderer *renderer = new EntityRenderer(framebuffer);

    Logger::log("Entering main loop");
    while (!window.shouldClose())
    {

        entity.update(deltaTime);
        entity.rotation.y += toRadians(3.0f) * deltaTime; // Rotate the entity for demonstration
        entity.rotation.x += toRadians(1.5f) * deltaTime; // Rotate the entity for demonstration

        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (window.getInput().KeyInput.getKeyDown(GLFW_KEY_ESCAPE))
        {
            Logger::debug("Escape key pressed, closing window");
            window.setShouldClose(true);
        }

        if (window.getInput().KeyInput.getKeyDown(GLFW_KEY_TAB))
        {
            toggleState++;
            if (toggleState >= sizeof(toggleStates) / sizeof(toggleStates[0]) + 1)
            {
                toggleState = 0;
            }
            Logger::debug("Toggled render state to: " + toggleStates[toggleState]);
        }

        frames++;
        if (currentTime - lastFpsUpdate >= fpsUpdateInterval)
        {
            fps = frames / (currentTime - lastFpsUpdate);
            lastFpsUpdate = currentTime;
            frames = 0;
        }

        camera.position.x += window.getInput().getPlayerNavigation().x * deltaTime * 2.0f;
        camera.position.z += window.getInput().getPlayerNavigation().y * deltaTime * 2.0f;
        camera.setProjectionMatrix(Matrix::getPerspectiveMatrix(toRadians(60.0f), (float)window.getWidth() / (float)window.getHeight(), 0.001f, 10000.0f));

        // pitch and roll

        camera.rotation.x += window.getInput().mouseInput.getMouseDelta().y * deltaTime * 0.1f;
        camera.rotation.y += window.getInput().mouseInput.getMouseDelta().x * deltaTime * 0.1f;

        window.setTitle("OpenGL Triangle - FPS: " + std::to_string(fps) + " - Calulated: " + std::to_string(1.0 / deltaTime) + " - Delta Time: " + std::to_string(deltaTime));

        // renderer->m_framebuffer.size = Vector2i(window.getWidth(), window.getHeight());
        renderer->setup();

        glUseProgram(shader.getProgram());

        glUniform4f(colorLoc, 0.5f, 0.5f, 0.5f, 1.0f);
        glUniform3f(lightPosLoc, 30.0f, 30.0f, 30.0f);

        renderer->render(entity, camera, shader);
        renderer->cleanup();

        checkError(glViewport(0, 0, window.getWidth(), window.getHeight()));
        glClearColor(0.5f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_DEPTH_TEST);

        glUseProgram(screenShader.getProgram());
        glUniform1i(glGetUniformLocation(screenShader.getProgram(), "textureMode"), toggleState);

        glUniform3f(glGetUniformLocation(screenShader.getProgram(), "lightPos"), 30.0f, 30.0f, 30.0f);
        glUniform3f(glGetUniformLocation(screenShader.getProgram(), "viewPos"), camera.position.x, camera.position.y, camera.position.z);

        glActiveTexture(GL_TEXTURE1);
        framebuffer.position.bind();
        glUniform1i(glGetUniformLocation(screenShader.getProgram(), "positionMap"), 1);

        glActiveTexture(GL_TEXTURE2);
        framebuffer.color.bind();
        glUniform1i(glGetUniformLocation(screenShader.getProgram(), "colorMap"), 2);

        glActiveTexture(GL_TEXTURE3);
        framebuffer.tangent.bind();
        glUniform1i(glGetUniformLocation(screenShader.getProgram(), "tangentMap"), 3);

        glActiveTexture(GL_TEXTURE4);
        framebuffer.bitangent.bind();
        glUniform1i(glGetUniformLocation(screenShader.getProgram(), "bitangentMap"), 4);

        glActiveTexture(GL_TEXTURE5);
        framebuffer.normal.bind();
        glUniform1i(glGetUniformLocation(screenShader.getProgram(), "normalMap"), 5);

        glActiveTexture(GL_TEXTURE6);
        framebuffer.depth.bind();
        glUniform1i(glGetUniformLocation(screenShader.getProgram(), "depthMap"), 6);

        glBindVertexArray(screenMesh.VAO);
        glDrawElements(GL_TRIANGLES, screenMesh.size, GL_UNSIGNED_INT, 0);

        window.swapBuffers();
        window.pollEvents();
    }

    Logger::log("Exiting main loop");

    Logger::log("Cleaned up OpenGL resources");

    return 0;
}
