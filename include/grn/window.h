#pragma once

#include <string>
#include <functional>
#include "input.h"

// Forward declarations to avoid including GLFW in header
struct GLFWwindow;

namespace grn
{

    class Window
    {
    public:
        // Constructors
        Window(int width, int height, const std::string &title);
        Window(const std::string &title, bool fullscreen = false); // Auto-detect screen size

        // Destructor
        ~Window();

        // Delete copy constructor and assignment (non-copyable)
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        // Public methods
        bool shouldClose() const;
        void swapBuffers();
        void pollEvents();
        void makeContextCurrent();
        void setShouldClose(bool value);
        // Getters
        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }
        int getFramebufferWidth() const;
        int getFramebufferHeight() const;
        GLFWwindow *getNativeWindow() const { return m_window; }

        void setSize(int width, int height);
        void setTitle(const std::string &title);

        // Event callbacks
        void setCallbacks();

        Input getInput();

    private:
        // Private member variables
        GLFWwindow *m_window;
        int m_width;
        int m_height;
        std::string m_title;
        Input m_input;

        // Private methods
        bool initializeGLFW();
        void cleanup();
    };

} // namespace grn