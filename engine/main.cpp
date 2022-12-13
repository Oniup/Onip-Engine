#include "onip/core/application.hpp"

int main(int argc, char** argv) {
    onip::Application* app = onip::Application::getInstance();
    app->run();

    return 0;
}