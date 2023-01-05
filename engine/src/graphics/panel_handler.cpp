#include "onip/graphics/panel_handler.hpp"
#include "onip/graphics/gl_pipeline.hpp"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

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

        ImGuiStyle& style = ImGui::GetStyle();
        if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowBorderSize = 0.0f;
            style.WindowRounding = 1.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(GlPipeline::getWindow()->getInternalWindow(), true);
        ImGui_ImplOpenGL3_Init(GlPipeline::getOpenGlVersion());
    }

    PanelHandler::~PanelHandler() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void PanelHandler::onUpdate() {
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        glNewFrame();

        static ImGuiDockNodeFlags docking_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        window_flags |= ImGuiWindowFlags_NoBackground;
        ImGui::Begin("Main Docking Space", (bool*)true, window_flags);
        ImGui::PopStyleVar(2);
        if (m_io->ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID docking_id = ImGui::GetID("Docking Space");
            ImGui::DockSpace(docking_id, ImVec2(0.0f, 0.0f), docking_flags);
        }
        else {
            ImGui::Text("Error, Need to Enable Dock Space");
            if (ImGui::SmallButton("Click to Enable Dock Space")) {
                m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            }
        }

        ImGui::Begin("Test Window", (bool*)true);
        ImGui::Text("This is some Text");
        ImGui::End();

        ImGui::End();
        glEndFrame();
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
