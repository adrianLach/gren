#pragma once

#include <string>
#include <functional>

// Forward declarations to avoid including GLFW in header
struct GLFWwindow;

namespace gren {

class Window {
public:
    // Constructor
    Window(int width, int height, const std::string& title);
    
    // Destructor
    ~Window();
    
    // Delete copy constructor and assignment (non-copyable)
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    
    // Public methods
    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    void makeContextCurrent();
    void setShouldClose(bool value); 
    // Getters
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }
    GLFWwindow* getNativeWindow() const { return m_window; }
    
    // Event callbacks
    void setKeyCallback(std::function<void(int, int, int, int)> callback);
    void setResizeCallback(std::function<void(int, int)> callback);
    
private:
    // Private member variables
    GLFWwindow* m_window;
    int m_width;
    int m_height;
    std::string m_title;
    
    // Static callback functions for GLFW
    static void keyCallbackWrapper(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void resizeCallbackWrapper(GLFWwindow* window, int width, int height);
    
    // Private methods
    bool initializeGLFW();
    void cleanup();
};

} // namespace gren