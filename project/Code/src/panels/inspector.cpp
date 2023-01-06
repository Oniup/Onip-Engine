#include "panels/inspector.hpp"
#include "panels/console.hpp"

#include <imgui/imgui.h>

Inspector::Inspector() {
    Console::logMessage("Initialize Inspector panel");
}

void Inspector::onImGuiDraw() {
    ImGui::Text("This is a test");
}