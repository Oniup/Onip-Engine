#include "panels/docking_space.hpp"

#include <onip/graphics/gl_pipeline.hpp>
#include <onip/graphics/window.hpp>
#include <imgui/imgui.h>

DockingSpace::DockingSpace() {
    getImGuiWindowFlags() |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
                             ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
                             ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground;
}

void DockingSpace::settingsBeforeImGuiBegin() {
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
}

void DockingSpace::onImGuiDraw() {
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::PopStyleVar(3);
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID docking_id = ImGui::GetID("Docking Space");
        ImGui::DockSpace(docking_id, ImVec2(0.0f, 0.0f), flags);
    }
    else {
        ImGui::Text("Error, Need to Enable Dock Space");
        if (ImGui::SmallButton("Click to Enable Dock Space")) {
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        }
    }

    if (ImGui::BeginMenuBar()) {
        if (ImGui::MenuItem("Close Application")) {
            onip::Application::quit();
        }
        ImGui::EndMenuBar();
    }
}