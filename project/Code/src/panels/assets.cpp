#include "panels/assets.hpp"
#include "panels/console.hpp"

#include <onip/core/debug.hpp>
#include <imgui/imgui.h>

Assets::Assets() {
    onip::Debug::logMessage("Initialize Assets Panel");
}

void Assets::onImGuiDraw() {
    ImGui::Text("This is a test");
}