#include "panels/hierarchy.hpp"
#include <imgui/imgui.h>

#include "panels/console.hpp"

Hierarchy::Hierarchy() {
    Console::logMessage("Initialize Hierarchy Panel");
}

void Hierarchy::onImGuiDraw() {
    ImGui::Text("This is a test");
}