#ifndef __ONIP_CORE_APPLICATION_HPP__
#define __ONIP_CORE_APPLICATION_HPP__

#include "onip/utils/utils.hpp"
#include "onip/core/application_layer.hpp"

#include <vector>

namespace onip {
    class Application {
    public:
        virtual ~Application() = default;

        static Application* get();
        static ONIP_INLINE class GLPipeline* graphics_pipeline() { return get()->m_pipeline; }
        static ONIP_INLINE class SceneManager* scene_manager() { return get()->m_scene_manager; }

        virtual void initialize_layers() {}

        void run();
        ApplicationLayer* add_layer(ApplicationLayer* layer);
    private:
        void destroy_layers();

        std::vector<ApplicationLayer*> m_layers;
        class GLPipeline* m_pipeline;
        class SceneManager* m_scene_manager;
    };
}

#endif // __ONIP_CORE_APPLICATION_HPP__