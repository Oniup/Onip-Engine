#include "panels/hierarchy.hpp"
#include <imgui/imgui.h>

#include <onip/core/debug.hpp>
#include "panels/console.hpp"

Hierarchy::Hierarchy() {
    onip::Debug::logMessage("Initialize Hierarchy Panel");
}

void Hierarchy::onImGuiDraw() {
    ImGui::Text("This is a test");
}