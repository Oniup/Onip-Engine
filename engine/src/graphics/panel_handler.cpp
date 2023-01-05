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
        m_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Test Window");
        ImGui::Text("This is some Text");
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void PanelHandler::updateImGuiPlatformWindow() {
        if (m_io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(GlPipeline::getWindow()->getInternalWindow());
        }
    }
}
