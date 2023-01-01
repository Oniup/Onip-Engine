#include "onip/core/input.hpp"
#include "onip/graphics/gl_pipeline.hpp"
#include "onip/graphics/window.hpp"

#include <GLFW/glfw3.h>

namespace onip {
    void Input::pollEvents() {
        glfwPollEvents();
    }

    bool Input::getKeyDown(Keyboard key) {
        return false;
    }

    bool Input::getKeyReleaseDown(Keyboard key) {
        return false;
    }

    bool Input::getKey(Keyboard key) {
        return glfwGetKey(GlPipeline::getWindow()->getInternalWindow(), static_cast<int>(key)) == GLFW_PRESS;
    }

    bool Input::getKeyReleased(Keyboard key) {
        return glfwGetKey(GlPipeline::getWindow()->getInternalWindow(), static_cast<int>(key)) == GLFW_RELEASE;
    }

    bool Input::getMouseButtonDown(Mouse mouse) {
        return false;
    }

    bool Input::getMouseButtonReleaseDown(Mouse mouse) {
        return false;
    }

    bool Input::getMouseButton(Mouse mouse) {
        return glfwGetMouseButton(GlPipeline::getWindow()->getInternalWindow(), static_cast<int>(mouse)) == GLFW_PRESS;
    }

    bool Input::getMouseButtonRelease(Mouse mouse) {
        return glfwGetMouseButton(GlPipeline::getWindow()->getInternalWindow(), static_cast<int>(mouse)) == GLFW_RELEASE;
    }
}