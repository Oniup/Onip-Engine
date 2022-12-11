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
        static ONIP_INLINE class GLPipeline* getGraphicsPipeline() { return getInstance()->m_pipeline; }
        static ONIP_INLINE class SceneManager* getSceneManager() { return getInstance()->m_scene_manager; }

        virtual void initializeLayers() {}

        void run();
        ApplicationLayer* addLayer(ApplicationLayer* layer);
    private:
        void destroyLayers();

        std::vector<ApplicationLayer*> m_layers;
        class GLPipeline* m_pipeline;
        class SceneManager* m_scene_manager;
    };
}

#endif // __ONIP_CORE_APPLICATION_HPP__