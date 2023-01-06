#include "panels/inspector.hpp"
#include "panels/console.hpp"

#include <onip/core/debug.hpp>
#include <imgui/imgui.h>

Inspector::Inspector() {
    onip::Debug::logMessage("Initialize Inspector panel");
}

void Inspector::onImGuiDraw() {
    ImGui::Text("This is a test");
}