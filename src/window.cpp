#include "grn/window.h"
#include "grn/logger.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>

namespace grn {

// Static member to track GLFW initialization
static bool s_glfwInitialized = false;
static int s_windowCount = 0;

Window::Window(int width, int height, const std::string& title)
    : m_window(nullptr)
    , m_width(width)
    , m_height(height)
    , m_title(title)
{
    if (!initializeGLFW()) {
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

    // Create window
    m_window = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), NULL);
    if (!m_window) {
        cleanup();
        throw std::runtime_error("Failed to create GLFW window");
    }
    
    // Store this instance in the window user pointer for callbacks
    glfwSetWindowUserPointer(m_window, this);

    s_windowCount++;
}

Window::~Window() {
    cleanup();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::makeContextCurrent() {

    glfwMakeContextCurrent(m_window);
    // Initialize GLEW after making context current
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        cleanup();
        throw std::runtime_error("Failed to initialize GLEW");
    }

    // Log OpenGL information for debugging
    Logger::log("OpenGL Version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    Logger::log("GLSL Version: " + std::string(reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION))));
    Logger::log("Renderer: " + std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER))));
    Logger::log("Vendor: " + std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR))));

    glfwSwapInterval(GLFW_TRUE);
    
    // Check if adaptive VSync worked by testing swap interval
    // If adaptive VSync isn't supported, NVIDIA drivers usually fallback to regular VSync
    Logger::log("VSync mode set: Adaptive VSync attempted");
}

void Window::setSize(int width, int height) {
    m_width = width;
    m_height = height;
}

void Window::setTitle(const std::string& title) {
    m_title = title;
    glfwSetWindowTitle(m_window, title.c_str());
}

void Window::setShouldClose(bool value) {
    glfwSetWindowShouldClose(m_window, value);
}

void Window::setKeyCallback(std::function<void(int, int, int, int)> callback) {
    // Store callback in window user data or use a static map
    // For simplicity, we'll set the GLFW callback directly
    glfwSetKeyCallback(m_window, keyCallbackWrapper);
}

void Window::setResizeCallback(std::function<void(int, int)> callback) {
    glfwSetFramebufferSizeCallback(m_window, resizeCallbackWrapper);
}

// Private methods
bool Window::initializeGLFW() {
    if (!s_glfwInitialized) {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }
        s_glfwInitialized = true;
    }
    return true;
}

void Window::cleanup() {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
        s_windowCount--;
    }
    
    // Terminate GLFW when last window is destroyed
    if (s_windowCount == 0 && s_glfwInitialized) {
        glfwTerminate();
        s_glfwInitialized = false;
    }
}

// Static callback wrappers
void Window::keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowInstance) {
        // Handle key events here or call stored callback
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        if (key == GLFW_KEY_F && action == GLFW_PRESS) {
            //go fullscreen
            glfwMaximizeWindow(window);
        }
    }
}

void Window::resizeCallbackWrapper(GLFWwindow* window, int width, int height) {
    Window* windowInstance = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowInstance) {
        windowInstance->m_width = width;
        windowInstance->m_height = height;
        grn::Logger::log("Window resized to: " + std::to_string(width) + "x" + std::to_string(height));
        // Update OpenGL viewport
        glViewport(0, 0, width, height);
    }
}

} // namespace grn