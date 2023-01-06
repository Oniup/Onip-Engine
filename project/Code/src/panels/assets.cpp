#include "panels/assets.hpp"
#include "panels/console.hpp"

#include <imgui/imgui.h>

Assets::Assets() {
    Console::logMessage("Initialize Assets Panel");
}

void Assets::onImGuiDraw() {
    ImGui::Text("This is a test");
}