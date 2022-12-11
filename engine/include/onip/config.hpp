#ifndef __ONIP_CONFIG_HPP__
#define __ONIP_CONFIG_HPP__

// Ecs
#define ONIP_SCENE_COMP_POOL_BLOCK_COUNT 50
#define ONIP_SCENE_ENIT_POOL_BLOCK_COUNT 100

// Debug
#define ONIP_DEBUG_COMPILE_FUNCTIONS 1

// Graphics
#define ONIP_GRAPHICS_TYPES_STORE_PATH 1

// Third Party
#define STB_IMAGE_IMPLEMENTATION

namespace onip {
    struct Config { 
        // Application instance that you inherited, return that
        static class Application* settingsApplication();
        // defines the components that every entity will be assigned with when it is created
        static void settingDefaultComponents(class Entity* entity);
    };
}

#endif // __ONIP_CONFIG_HPP__