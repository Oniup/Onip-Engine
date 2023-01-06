#include "panels/console.hpp"

Console* Console::s_instance = nullptr;

Console::Console() {
    getImGuiWindowFlags() |= ImGuiWindowFlags_MenuBar;
    if (s_instance == nullptr) {
        s_instance = this;
    }
    else {
        logWarning("Cannot Create Another Console, This will cause a memory leak");
    }
}

void Console::onImGuiDraw() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::MenuItem("Clear")) {
            m_logs.clear();
        }
        if (ImGui::BeginMenu("Filter Options")) {
            ImGui::CheckboxFlags("Show Messages", &m_print_message_flags, Severity_Message);
            ImGui::CheckboxFlags("Show Warning", &m_print_message_flags, Severity_Warning);
            ImGui::CheckboxFlags("Show Errors", &m_print_message_flags, Severity_Error);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    for (std::tuple<std::string, Severity>& log : m_logs) {
        if (m_print_message_flags & std::get<Severity>(log)) {
            const char* message = std::get<std::string>(log).c_str();
            switch (std::get<Severity>(log)) {
            case Severity_Message: {
                ImGui::Text("[Message]: %s", message);
            } break;
            case Severity_Warning: {
                ImGui::TextColored(warning_message_color, "[Warning]: %s", message);
            } break;
            case Severity_Error: {
                ImGui::TextColored(error_message_color, "[Error]: %s", message);
            } break;
            }
        }
    }
}

void Console::logMessage(std::string_view message) {
    log(message, Severity_Message);
}

void Console::logWarning(std::string_view warning) {
    log(warning, Severity_Warning);
}

void Console::logError(std::string_view error) {
    log(error, Severity_Error);
}

void Console::log(std::string_view log, Severity severity) {
    s_instance->m_logs.push_back(std::tuple<std::string, Severity>(log, severity));
}