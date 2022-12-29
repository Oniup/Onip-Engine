#ifndef __ONIP_GRAPHICS_WINDOW_HPP__
#define __ONIP_GRAPHICS_WINDOW_HPP__

#include "onip/utils/utils.hpp"

#include <glm/glm.hpp>
#include <string>

struct GLFWwindow;

namespace onip {
    class Window {
    public:
        enum Resolution
        {
            Resolution_Auto = 0,            // Finds the resolution of the primary monitor and sets the window to that size
            Resolution_Custom,              // User defined window size
            Resolution_Maximize,            // sets the window to be in its maximize state
            Resolution_SXGA,                // 1280 x 1024 Super-eXtended Graphics Array 
            Resolution_HD,                  // 1366 x 768 High Definition 
            Resolution_HD_Plus,             // 1600 x 900 High Definition Plus  
            Resolution_FHD,                 // 1920 x 1080 Full High Definition 
            Resolution_WUXGA,               // 1920 x 1200 Wide Ultra Extended Graphics Array 
            Resolution_QHD,                 // 2560 x 1440 Quad High Definition 
            Resolution_WQHD,                // 3440 x 1440 Wide Quad High Definition 
            Resolution_UHD,                 // 3840 x 2160 4K or Ultra High Definition
            Resolution_None                 // there is no window
        };
        enum Config
        {
            Config_None                     = 0x000,
            Config_FullScreen               = 0x001,
            Config_TransparentFrameBuffer   = 0x002,
            Config_EnableDepthTesting       = 0x004,
            Config_DisableBlendFunc         = 0x008,
            Config_Resizable                = 0x010,
            Config_RemoveDecoration         = 0x020,
            Config_AlwaysOnTop              = 0x040,
            Config_EnableDoubleBuffering    = 0x080,
            Config_DisableVsync             = 0x100,
            Config_UpdateViewportPerFrame   = 0x200,
            Config_DisableClearingScreen    = 0x400
        };

        Window() = default;
        Window(const glm::ivec2& size, const char* title);
        Window(const glm::ivec2& size, const char* title, Config config);
        Window(const char* title, Config config);
        Window(const char* title, Resolution resolution, Config config);
        Window(const char* title, Resolution resolution);
        Window(const Window& other);
        Window(Window&& other);
        ~Window();
    
        Window& operator=(const Window& other);
        Window& operator=(Window&& other);
    
        ONIP_INLINE GLFWwindow* internalWindow() const { return m_internal; }
        ONIP_INLINE const glm::ivec2& getSize() const { return m_size; }
        ONIP_INLINE const glm::ivec2& getPosition() const { return m_position; }
        ONIP_INLINE const std::string& getTitle() const { return m_title; }
        ONIP_INLINE Resolution getResolution() const { return m_resolution; }
        ONIP_INLINE Config getConfig() const { return m_config; }
        ONIP_INLINE const glm::vec4& getClearColor() const { return m_clear_color; }
        ONIP_INLINE const glm::ivec2& getViewportOffset() const { return m_viewport_offset; }
    
        void setConfig(Config config);
        void getResolutionInPixels(Resolution resolution, int* width, int* height) const;
        bool isClosing() const;
        void clearScreen();
        void swapBuffers();
    private:
        void initInstance();
    
        static bool s_InitializedGlad;
    
        GLFWwindow* m_internal { nullptr };
        glm::ivec2 m_size { 0, 0 };
        glm::ivec2 m_position { 0, 0 };
        std::string m_title {};
        Resolution m_resolution { Resolution_None };
        Config m_config { Config_None };
        glm::vec4 m_clear_color {glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) };
        glm::ivec2 m_viewport_offset { 0, 0 };
    };
}

#endif // __ONIP_GRAPHICS_WINDOW_HPP__