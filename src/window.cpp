#include "grn/window.h"
#include "grn/logger.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

namespace grn
{

    // Static member to track GLFW initialization
    static bool s_glfwInitialized = false;
    static int s_windowCount = 0;

    Window::Window(int width, int height, const std::string &title)
        : m_window(nullptr), m_width(width), m_height(height), m_title(title)
    {
        if (!initializeGLFW())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwDefaultWindowHints();

        // Set OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Add debug context for troubleshooting
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        glfwSetErrorCallback([](int error, const char *description)
                             { Logger::error("GLFW Error: " + std::string(description)); });

        // Create window
        m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!m_window)
        {
            cleanup();
            throw std::runtime_error("Failed to create GLFW window");
        }
        // update width and height based on the created window
        // glfwGetWindowSize(m_window, &m_width, &m_height);
        glfwGetFramebufferSize(m_window, &m_width, &m_height);

        // Store this instance in the window user pointer for callbacks
        glfwSetWindowUserPointer(m_window, this);

        s_windowCount++;
    }

    Window::Window(const std::string &title, bool fullscreen)
        : m_window(nullptr), m_title(title)
    {
        if (!initializeGLFW())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        glfwDefaultWindowHints();

        // Set OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Add debug context for troubleshooting
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, 4);

        glfwSetErrorCallback([](int error, const char *description)
                             { Logger::error("GLFW Error: " + std::string(description)); });

        // Get primary monitor and its video mode
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);

        if (!mode)
        {
            cleanup();
            throw std::runtime_error("Failed to get video mode for primary monitor");
        }

        // Set window dimensions based on screen size
        m_width = mode->width;
        m_height = mode->height;

        Logger::log("Video mode dimensions: " + std::to_string(m_width) + "x" + std::to_string(m_height));
        Logger::log("Refresh rate: " + std::to_string(mode->refreshRate) + "Hz");

        // Create window (fullscreen or windowed based on parameter)
        GLFWmonitor *windowMonitor = fullscreen ? monitor : nullptr;
        m_window = glfwCreateWindow(m_width, m_height, title.c_str(), windowMonitor, NULL);

        if (!m_window)
        {
            cleanup();
            throw std::runtime_error("Failed to create GLFW window");
        }

        // If not fullscreen, center the window
        if (!fullscreen)
        {
            int xpos = (mode->width - m_width) / 2;
            int ypos = (mode->height - m_height) / 2;
            glfwSetWindowPos(m_window, xpos, ypos);
        }

        // Update width and height based on the created window
        glfwGetWindowSize(m_window, &m_width, &m_height);

        // Get framebuffer size for high-DPI displays
        int framebufferWidth, framebufferHeight;
        glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);

        // Calculate content scale
        float xscale, yscale;
        glfwGetWindowContentScale(m_window, &xscale, &yscale);

        // Store this instance in the window user pointer for callbacks
        glfwSetWindowUserPointer(m_window, this);

        Logger::log("Window size: " + std::to_string(m_width) + "x" + std::to_string(m_height));
        Logger::log("Framebuffer size: " + std::to_string(framebufferWidth) + "x" + std::to_string(framebufferHeight));
        Logger::log("Content scale: " + std::to_string(xscale) + "x" + std::to_string(yscale));

        s_windowCount++;
    }

    Window::~Window()
    {
        cleanup();
    }

    bool Window::shouldClose() const
    {
        return glfwWindowShouldClose(m_window);
    }

    void Window::swapBuffers()
    {
        glfwSwapBuffers(m_window);
    }

    void Window::pollEvents()
    {
        // updates must came before polling events to ensure input state is current
        m_input.KeyInput.updateFrame();
        m_input.mouseInput.updateFrame();
        glfwPollEvents();
    }

    void Window::makeContextCurrent()
    {

        glfwMakeContextCurrent(m_window);
        // Initialize GLEW after making context current
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            cleanup();
            throw std::runtime_error("Failed to initialize GLEW");
        }

        // Log OpenGL information for debugging
        Logger::log("OpenGL Version: " + std::string(reinterpret_cast<const char *>(glGetString(GL_VERSION))));
        Logger::log("GLSL Version: " + std::string(reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION))));
        Logger::log("Renderer: " + std::string(reinterpret_cast<const char *>(glGetString(GL_RENDERER))));
        Logger::log("Vendor: " + std::string(reinterpret_cast<const char *>(glGetString(GL_VENDOR))));

        glfwSwapInterval(GLFW_TRUE);
        // hide cursor
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Check if adaptive VSync worked by testing swap interval
        // If adaptive VSync isn't supported, NVIDIA drivers usually fallback to regular VSync
        Logger::log("VSync mode set: Adaptive VSync attempted");
    }

    void Window::setSize(int width, int height)
    {
        m_width = width;
        m_height = height;
    }

    void Window::setTitle(const std::string &title)
    {
        m_title = title;
        glfwSetWindowTitle(m_window, title.c_str());
    }

    void Window::setShouldClose(bool value)
    {
        glfwSetWindowShouldClose(m_window, value);
    }

    int Window::getFramebufferWidth() const
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        return width;
    }

    int Window::getFramebufferHeight() const
    {
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        return height;
    }

    void Window::setCallbacks()
    {
        // Set GLFW callbacks using captureless lambdas
        glfwSetKeyCallback(m_window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
                           { 
                                Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
                               if (windowInstance) {
                                   windowInstance->m_input.KeyInput.setKeyPressed(key, action != GLFW_RELEASE);
                               } });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, int button, int action, int mods)
                                   {
            Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (windowInstance) {
                windowInstance->m_input.mouseInput.setButtonPressed(button, action != GLFW_RELEASE);
            } });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, double xpos, double ypos)
                                 {
            Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (windowInstance) {
                windowInstance->m_input.mouseInput.setMousePosition(static_cast<float>(xpos), static_cast<float>(ypos));
            } });

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow *window, int width, int height)
                                       {
            Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            if (windowInstance) {
                Logger::log("Framebuffer resized to: " + std::to_string(width) + "x" + std::to_string(height));
                
                // Get window size for comparison
                int windowWidth, windowHeight;
                glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
                Logger::log("Window size: " + std::to_string(windowWidth) + "x" + std::to_string(windowHeight));
                
                windowInstance->setSize(windowWidth, windowHeight);
                glViewport(0, 0, width, height);
            } });
    }

    Input Window::getInput()
    {
        return m_input;
    }

    // Private methods
    bool Window::initializeGLFW()
    {
        if (!s_glfwInitialized)
        {
            if (!glfwInit())
            {
                std::cerr << "Failed to initialize GLFW" << std::endl;
                return false;
            }
            s_glfwInitialized = true;
        }
        return true;
    }

    void Window::cleanup()
    {
        if (m_window)
        {
            glfwDestroyWindow(m_window);
            m_window = nullptr;
            s_windowCount--;
        }

        // Terminate GLFW when last window is destroyed
        if (s_windowCount == 0 && s_glfwInitialized)
        {
            glfwTerminate();
            s_glfwInitialized = false;
        }
    }

    // // Static callback wrappers
    // void keyCallbackWrapper(GLFWwindow * window, int key, int scancode, int action, int mods)
    // {
    //     m_input.KeyInput.setKeyPressed(key == GLFW_PRESS);
    //     Window *windowInstance = static_cast<Window *>(glfwGetWindowUserPointer(window));
    //     if (windowInstance)
    //     {
    //         // Handle key events here or call stored callback
    //         if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    //         {
    //             glfwSetWindowShouldClose(window, GLFW_TRUE);
    //         }
    //     }
    // }

    // void resizeCallbackWrapper(GLFWwindow * window, int width, int height)
    // {
    //     Window *windowInstance = static_cast<Window *>(glfwGetWindowUserPointer(window));
    //     if (windowInstance)
    //     {
    //         windowInstance->m_width = width;
    //         windowInstance->m_height = height;
    //         grn::Logger::log("Window resized to: " + std::to_string(width) + "x" + std::to_string(height));
    //         // Update OpenGL viewport
    //         glViewport(0, 0, width, height);
    //     }
    // }

} // namespace grn