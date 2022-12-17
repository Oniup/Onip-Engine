#ifndef __ONIP_CORE_APPLICATION_HPP__
#define __ONIP_CORE_APPLICATION_HPP__

#include "onip/utils/utils.hpp"
#include "onip/core/application_layer.hpp"

#include <vector>

namespace onip {
    class Application {
    public:
        virtual ~Application() = default;

        static Application* getInstance();
        static ONIP_INLINE class GlPipeline* getGraphicsPipeline() { return getInstance()->m_pipeline; }
        static ONIP_INLINE class SceneManager* getSceneManager() { return getInstance()->m_scene_manager; }

        virtual void initializeLayers() {}

        void run();
        ApplicationLayer* addLayer(ApplicationLayer* layer);
    private:
        void destroyLayers();

        std::vector<ApplicationLayer*> m_layers {};
        class GlPipeline* m_pipeline { nullptr };
        class SceneManager* m_scene_manager { nullptr };
    };
}

#endif // __ONIP_CORE_APPLICATION_HPP__