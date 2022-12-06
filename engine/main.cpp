#include "onip/core/application.hpp"

int main(int argc, char** argv) {
    onip::Application* app = onip::Application::get();
    app->run();

    return 0;
}