#include "panels/console.hpp"

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
        if (ImGui::BeginMenu("Filter Options")) {
            ImGui::CheckboxFlags("Show Messages", &m_print_message_flags, onip::DebugLogSeverity_Message);
            ImGui::CheckboxFlags("Show Warning", &m_print_message_flags, onip::DebugLogSeverity_Warning);
            ImGui::CheckboxFlags("Show Errors", &m_print_message_flags, onip::DebugLogSeverity_Error);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    for (const std::tuple<std::string_view, onip::DebugLogSeverity>& log : onip::Debug::getLogs()) {
        if (m_print_message_flags & std::get<onip::DebugLogSeverity>(log)) {
            switch (std::get<onip::DebugLogSeverity>(log)) {
            case onip::DebugLogSeverity_Message: {
                ImGui::Text("[Message]: %s", std::get<std::string_view>(log).data());
            } break;
            case onip::DebugLogSeverity_Warning: {
                ImGui::TextColored(warning_message_color, "[Warning]: %s", std::get<std::string_view>(log).data());
            } break;
            case onip::DebugLogSeverity_Error: {
                ImGui::TextColored(error_message_color, "[Error]: %s", std::get<std::string_view>(log).data());
            } break;
            }
        }
    }
}