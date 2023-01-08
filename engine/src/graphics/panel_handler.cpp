#include "onip/graphics/panel_handler.hpp"
#include "onip/graphics/gl_pipeline.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace onip {
    PanelHandler::PanelHandler() : ApplicationLayer("Panel Handler") {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        m_io = &ImGui::GetIO(); (void)*m_io;
        m_io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        m_io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        m_io->ConfigViewportsNoAutoMerge = true;
        m_io->ConfigViewportsNoTaskBarIcon = true;

        ImGui::StyleColorsDark();

        m_global_styling.style = &ImGui::GetStyle();
        m_global_styling.style->WindowBorderSize = 0.0f;
        m_global_styling.style->WindowRounding = 1.0f;
        m_global_styling.style->Colors[ImGuiCol_WindowBg].w = 1.0f;

        ImGui_ImplGlfw_InitForOpenGL(GlPipeline::getWindow()->getInternalWindow(), true);
        ImGui_ImplOpenGL3_Init(GlPipeline::getOpenGlVersion());
    }

    PanelHandler::~PanelHandler() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void PanelHandler::onUpdate() {
        glNewFrame();
        for (Panel* panel : m_panels) {
            panel->settingsBeforeImGuiBegin();
            ImGui::Begin(panel->getName(), panel->getOpenedPtr(), panel->getImGuiWindowFlags());
            panel->onImGuiDraw();
            ImGui::End();
        }
        glEndFrame();
    }

    void PanelHandler::updateGlobalStyle(const GlobalStyling& global_style) {
        if (global_style.style != nullptr) {
            *m_global_styling.style = *global_style.style;
        }
        m_global_styling.warning_color = global_style.warning_color;
        m_global_styling.error_color = global_style.error_color;

        if (global_style.font_path.length() > 0 && global_style.font_size > 0.0f) {
            m_io->Fonts->AddFontFromFileTTF(global_style.font_path.c_str(), global_style.font_size);
            m_global_styling.font_path = global_style.font_path;
            m_global_styling.font_size = global_style.font_size;
        }
    }

    void PanelHandler::updateImGuiPlatformWindow() {
        if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(GlPipeline::getWindow()->getInternalWindow());
        }
    }

    void PanelHandler::enableMainWindowDockSpace(bool enable) {
        if (enable) {
            m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        }
        else {
            m_io->ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;
        }
    }

    bool PanelHandler::removePanel(const char* name) {
        for (std::vector<Panel*>::iterator panel = m_panels.begin(); panel != m_panels.end(); panel++) {
            Panel* ptr = *panel;
            if (std::strncmp(name, ptr->getName(), std::strlen(name))) {
                m_panels.erase(panel);
                return true;
            }
        }
        return false;
    }

    void PanelHandler::glNewFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void PanelHandler::glEndFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
