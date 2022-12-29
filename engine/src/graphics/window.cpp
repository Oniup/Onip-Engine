#include "onip/graphics/window.hpp"

#include "onip/utils/utils.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace onip {
    bool Window::s_InitializedGlad = false;
    
    Window::Window(const glm::ivec2& size, const char* title)
        : Window(size, title, Config_None) {}
    
    Window::Window(const glm::ivec2& size, const char* title, Config config)
        : m_internal(nullptr), m_size(size), m_position(), m_title(title), m_resolution(Resolution_Custom), m_config(config) {
        initInstance();
    }
    
    Window::Window(const char* title, Config config)
        : Window(title, Resolution_Auto, config) {}
    
    Window::Window(const char* title, Resolution resolution, Config config)
        : m_internal(nullptr), m_size({ -1, -1 }), m_position(), m_title(title), m_resolution(resolution), m_config(config) {
        initInstance();
    }
    
    Window::Window(const char* title, Resolution resolution)
        : Window(title, resolution, Config_None) {}
    
    Window::Window(const Window& other)
        : m_internal(nullptr), m_size(other.m_size), m_position(other.m_position), m_title(other.m_title), 
          m_resolution(other.m_resolution), m_config(other.m_config), m_viewport_offset(other.m_viewport_offset) {
        initInstance();
    }
    
    Window::Window(Window&& other)
        : m_internal(std::move(other.m_internal)), 
          m_size(std::move(other.m_size)), m_position(std::move(other.m_position)),
          m_title(std::move(other.m_title)), m_resolution(other.m_resolution), m_config(other.m_config), 
          m_clear_color(std::move(other.m_clear_color)), m_viewport_offset(std::move(other.m_viewport_offset)) {
        other.m_internal = nullptr;
        other.m_config = Config_None;
    }
    
    Window::~Window() {
        if (m_internal) {
            glfwDestroyWindow(m_internal);
            m_internal = nullptr;
        }
    }
    
    Window& Window::operator=(const Window& other) {
        if (m_internal) {
            glfwDestroyWindow(m_internal);
        }
        m_internal = nullptr;
        m_size = other.m_size;
        m_position = other.m_position;
        m_title = other.m_title;
        m_config = other.m_config;
        m_clear_color = other.m_clear_color;
        m_viewport_offset = other.m_viewport_offset;
    
        initInstance();
        return *this;
    }
    
    Window& Window::operator=(Window&& other) {
        if (m_internal) {
            glfwDestroyWindow(m_internal);
        }
        m_internal = std::move(other.m_internal);
        m_size = std::move(other.m_size);
        m_position = std::move(other.m_position);
        m_title = std::move(other.m_title);
        m_config = other.m_config;
        m_clear_color = std::move(other.m_clear_color);
        m_viewport_offset = std::move(other.m_viewport_offset);
    
        other.m_internal = nullptr;
        other.m_config = Config_None;
        
        return *this;
    }
    
    void Window::setConfig(Config config) {
        switch (config) {
        case Config_None:
            return;
        case Config_FullScreen: {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
            int vsync = m_config & Config_DisableVsync ? 0 : 1;
            if (m_config & Config_FullScreen) {
                glfwSetWindowMonitor(m_internal, nullptr, m_position[0], m_position[1], m_size[0], m_size[1], vsync);
            }
            else {
                glfwSetWindowMonitor(m_internal, monitor, m_position[0], m_position[1], m_size[0], m_size[1], vsync);
            }
        }
            break;
        case Config_TransparentFrameBuffer:
            return;
        case Config_EnableDepthTesting:
            if (m_config & Config_EnableDepthTesting) {
                glEnable(GL_DEPTH);
            }
            else {
                glDisable(GL_DEPTH);
            }
            break;
        case Config_DisableBlendFunc:
            if (m_config & Config_DisableBlendFunc) {
                glDisable(GL_BLEND);
            }
            else {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            }
            return;
        case Config_Resizable:
            return;
        case Config_RemoveDecoration:
            return;
        case Config_AlwaysOnTop:
            return;
        case Config_EnableDoubleBuffering:
            return;
        case Config_DisableVsync: {
            int vsync = m_config & Config_DisableVsync ? 1 : 0;
            glfwSwapInterval(vsync);
        }
            break;
        case Config_UpdateViewportPerFrame:
            break;
        case Config_DisableClearingScreen:
            break;
        }
        if (m_config & config) {
            m_config = (Config)(m_config & ~config);
        }
        else {
            m_config = (Config)(m_config & config);
        }
    }
    
    void Window::getResolutionInPixels(Resolution resolution, int* width, int* height) const {
        switch (resolution) {
            case Resolution_Auto: {
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
                *width  = vidmode->width - vidmode->width / 4;
                *height = vidmode->height - vidmode->height / 4;
            }
                break;
            case Resolution_Custom:
                break;
            case Resolution_Maximize: {
                GLFWmonitor* monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
                *width  = vidmode->width - vidmode->width / 4;
                *height = vidmode->height - vidmode->height / 4;
            }
                break;
            case Resolution_SXGA:
                *width  = 1128;
                *height = 1024;
                break;
            case Resolution_HD:
                *width  = 1366;
                *height = 768;
                break;
            case Resolution_HD_Plus:
                *width  = 1600;
                *height = 900;
                break;
            case Resolution_FHD:
                *width  = 1920;
                *height = 1080;
                break;
            case Resolution_WUXGA:
                *width  = 1920;
                *height = 1200;
                break;
            case Resolution_QHD:
                *width  = 2560;
                *height = 1440;
                break;
            case Resolution_WQHD:
                *width  = 3440;
                *height = 1440;
                break;
            case Resolution_UHD:
                *width  = 3840;
                *height = 2160;
                break;
            case Resolution_None:
                *width  = 0;
                *height = 0;
                break;
        }
    }
    
    bool Window::isClosing() const {
        return glfwWindowShouldClose(m_internal);
    }
    
    void Window::clearScreen() {
        if (!(m_config & Config_DisableClearingScreen)) {
            glClearColor(m_clear_color[0], m_clear_color[1], m_clear_color[2], m_clear_color[3]);
            if (m_config & Config_EnableDepthTesting) {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }
            else {
                glClear(GL_COLOR_BUFFER_BIT);
            }
        }
        if (m_config & Config_UpdateViewportPerFrame) {
            glfwGetFramebufferSize(m_internal, &m_size.x, &m_size.y);
            glViewport(m_viewport_offset[0], m_viewport_offset[1], m_size.x, m_size.y);
        }
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(m_internal);
    }
    
    void Window::initInstance() {
        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_VERSION_MINOR, 5);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    #endif 
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, m_config & Config_TransparentFrameBuffer ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, m_config & Config_Resizable ? GLFW_FALSE : GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, m_config & Config_RemoveDecoration ? GLFW_FALSE : GLFW_TRUE);
        glfwWindowHint(GLFW_FLOATING, m_config & Config_AlwaysOnTop ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, m_config & Config_EnableDoubleBuffering ? GLFW_TRUE : GLFW_FALSE);
    
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
        GLFWmonitor* target_monitor = nullptr;
        if (m_config & Config_FullScreen) {
            m_size = { 
                vidmode->width - vidmode->width / 4,
                vidmode->height - vidmode->height / 4
            };
            target_monitor = monitor;
        }
        else {
            if (m_resolution != Resolution_Custom) {
                getResolutionInPixels(m_resolution, &m_size[0], &m_size[1]);
            }
            else if (m_size[0] == -1) {
                m_size = { 
                    vidmode->width - vidmode->width / 4,
                    vidmode->height - vidmode->height / 4
                };
            }
        }
        m_internal = glfwCreateWindow(m_size[0], m_size[1], m_title.c_str(), target_monitor, nullptr);
        assert(m_internal && "failed to create glfw window for some reason ...");
        glm::ivec2 centre_pos = {
            vidmode->width / 2 - m_size[0] / 2,
            vidmode->height / 2 - m_size[1] / 2
        };
        glfwSetWindowPos(m_internal, centre_pos[0], centre_pos[1]);
    
        if (m_resolution == Resolution_Maximize) {
            glfwMaximizeWindow(m_internal);
        }
    
        glfwMakeContextCurrent(m_internal);
        assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) && "failed to initialize glad for some reason ...");
        if (!(m_config & Config_DisableBlendFunc)) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        if (m_config & Config_EnableDepthTesting) {
            glEnable(GL_DEPTH);
        }
        glfwSwapInterval(m_config & Config_DisableVsync ? 0 : 1);
    
        glViewport(m_viewport_offset[0], m_viewport_offset[1], m_size.x, m_size.y);
    }
}