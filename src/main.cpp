#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gren/window.hpp>
#include <gren/shader.hpp>
#include <gren/logger.hpp>
#include <gren/matrix.hpp>
#include <gren/terrain.hpp>

int main()
{

    gren::Logger::log("Starting OpenGL Triangle Example");
    gren::Window window(800, 600, "OpenGL Triangle");
    window.makeContextCurrent();

    float cubeVertices[] = {
        // positions          // normals           // uv
        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,

        -1.0f,
        1.0f,
        -1.0f,
        0.0f,
        0.0f,
        -1.0f,
        1.0f,
        0.0f,
        1.0f,
        -1.0f,
        -1.0f,
        0.0f,
        0.0f,
        -1.0f,
        0.0f,
        1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        0.0f,
        0.0f,
        -1.0f,
        0.0f,
        0.0f,

        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,

        1.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        0.0f,
        -1.0f,
        -1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,

        1.0f,
        -1.0f,
        -1.0f,
        0.0f,
        -1.0f,
        0.0f,
        1.0f,
        0.0f,
        -1.0f,
        -1.0f,
        1.0f,
        0.0f,
        -1.0f,
        0.0f,
        0.0f,
        1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        0.0f,
        -1.0f,
        0.0f,
        0.0f,
        0.0f,

        -1.0f,
        1.0f,
        -1.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,

        -1.0f,
        1.0f,
        1.0f,
        -1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        -1.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,

        -1.0f,
        1.0f,
        -1.0f,
        0.0f,
        0.0f,
        -1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        0.0f,
        0.0f,
        -1.0f,
        0.0f,
        1.0f,
        1.0f,
        -1.0f,
        -1.0f,
        0.0f,
        0.0f,
        -1.0f,
        0.0f,
        0.0f,

        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
        0.0f,

        1.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,

        1.0f,
        -1.0f,
        -1.0f,
        0.0f,
        -1.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        0.0f,
        -1.0f,
        0.0f,
        0.0f,
        1.0f,
        -1.0f,
        -1.0f,
        1.0f,
        0.0f,
        -1.0f,
        0.0f,
        0.0f,
        0.0f,

        -1.0f,
        1.0f,
        -1.0f,
        0.0f,
        1.0f,
        0.0f,
        1.0f,
        1.0f,
        -1.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        1.0f,
        1.0f,
        1.0f,
        1.0f,
        0.0f,
        1.0f,
        0.0f,
        0.0f,
        0.0f,
    };

    unsigned int cubeIndices[36];
    for (unsigned int i = 0; i < 36; ++i)
    {
        cubeIndices[i] = i;
    }

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // uv attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "layout (location = 1) in vec3 aNormal;\n"
                                     "layout (location = 2) in vec2 aTexCoord;\n"
                                     "\n"
                                     "out VS_OUT {\n"
                                     "    vec3 FragPos;\n"
                                     "    vec3 Normal;\n"
                                     "    vec2 TexCoords;\n"
                                     "} vs_out;\n"
                                     "\n"
                                     "uniform mat4 model;\n"
                                     "uniform mat4 view;\n"
                                     "uniform mat4 projection;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
                                     "   vs_out.FragPos = vec3(model * vec4(aPos, 1.0));\n"
                                     "   vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;\n"
                                     "   vs_out.TexCoords = aTexCoord;\n"
                                     "}\0";

    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "\n"
                                       "in VS_OUT {\n"
                                       "    vec3 FragPos;\n"
                                       "    vec3 Normal;\n"
                                       "    vec2 TexCoords;\n"
                                       "} fs_in;\n"
                                       "\n"
                                       "uniform vec4 color;\n"
                                       "uniform vec3 lightPos; // In world space\n"
                                       "\n"
                                       "void main()\n"
                                       "{\n"
                                       "   // ambient\n"
                                       "   float ambientStrength = 0.2;\n"
                                       "   vec3 ambient = ambientStrength * color.rgb;\n"
                                       "\n"
                                       "   // diffuse\n"
                                       "   vec3 norm = normalize(fs_in.Normal);\n"
                                       "   vec3 lightDir = normalize(lightPos - fs_in.FragPos);\n"
                                       "   float diff = max(dot(norm, lightDir), 0.0);\n"
                                       "   vec3 diffuse = diff * color.rgb;\n"
                                       "\n"
                                       "   vec3 result = ambient + diffuse;\n"
                                       "   FragColor = vec4(result, color.a);\n"
                                       "}\n\0";

    gren::Logger::log("Compiling shaders");
    gren::Shader shader(vertexShaderSource, fragmentShaderSource);

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

        gren::Matrix perpective = gren::Matrix::getPerspectiveMatrix(45.0f, (float)window.getWidth() / (float)window.getHeight(), 0.1f, 100.0f);
        gren::Matrix view;
        gren::Matrix model = gren::Matrix::getModelMatrix(position, rotation, scale);

        glUseProgram(shader.getProgram());
        glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "view"), 1, GL_FALSE, view.data());
        glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "projection"), 1, GL_FALSE, perpective.data());
        glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, GL_FALSE, model.data());

        glUniform4f(glGetUniformLocation(shader.getProgram(), "color"), 0.5f, 0.5f, 0.5f, 1.0f);
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

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
