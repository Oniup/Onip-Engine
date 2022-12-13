#include "onip/core/input.hpp"


#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace onip {
    void Input::pollEvents() {
        glfwPollEvents();
    }
}