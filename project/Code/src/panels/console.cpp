#include "panels/console.hpp"

#include <onip/graphics/gl_pipeline.hpp>
#include <onip/graphics/gl_batch_renderer.hpp>
#include <onip/core/scene_manager.hpp>

#include <imgui/imgui.h>
#include <string>
#include <vector>

Console* Console::s_instance = nullptr;

Console::Console() {
    getImGuiWindowFlags() |= ImGuiWindowFlags_MenuBar;
    if (s_instance == nullptr) {
        s_instance = this;
    }
    else {
        onip::Debug::logWarning("Cannot Create Another Console, This will cause a memory leak");
    }
}

void Console::onImGuiDraw() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::MenuItem("Clear")) {
            onip::Debug::clearLogs();
        }
        if (ImGui::BeginMenu("Debug Prints")) {
            if (ImGui::MenuItem("Print Debug ECS (Entity Component System)")) { 
                onip::debugPrintComponentGroups(); 
            }
            if (ImGui::MenuItem("Print Debug OpenGL Batch Renderer Vertex Data")) { 
                onip::GlBatchRenderer* renderer = static_cast<onip::GlBatchRenderer*>(onip::GlPipeline::getRenderer("Batch Renderer"));
                if (renderer != nullptr) {
                    renderer->debugLogBatchVertexData();
                }
                else {
                    onip::Debug::logWarning("Cannot \"Print Debug OpenGL Batch Renderer Vertex Data\" because the renderer isn't in use");
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Filter Options")) {
            ImGui::CheckboxFlags("Show Messages", &m_print_message_flags, onip::DebugLogSeverity_Message);
            ImGui::CheckboxFlags("Show Warning", &m_print_message_flags, onip::DebugLogSeverity_Warning);
            ImGui::CheckboxFlags("Show Errors", &m_print_message_flags, onip::DebugLogSeverity_Error);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    for (const std::tuple<std::string, onip::DebugLogSeverity>& log : onip::Debug::getLogs()) {
        if (m_print_message_flags & std::get<onip::DebugLogSeverity>(log)) {
            switch (std::get<onip::DebugLogSeverity>(log)) {
            case onip::DebugLogSeverity_Message: {
                ImGui::Text("[Message]: %s", std::get<std::string>(log).c_str());
            } break;
            case onip::DebugLogSeverity_Warning: {
                ImGui::TextColored(
                    onip::Application::getPanelHandler()->getGlobalImGuiStyling()->warning_color, 
                    "[Warning]: %s", std::get<std::string>(log).c_str()
                );
            } break;
            case onip::DebugLogSeverity_Error: {
                ImGui::TextColored(
                    onip::Application::getPanelHandler()->getGlobalImGuiStyling()->error_color, 
                    "[Error]: %s", std::get<std::string>(log).c_str()
                );
            } break;
            }
        }
    }
}