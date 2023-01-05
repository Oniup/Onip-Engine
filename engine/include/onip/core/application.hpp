#ifndef __ONIP_CORE_APPLICATION_HPP__
#define __ONIP_CORE_APPLICATION_HPP__

#include "onip/utils/utils.hpp"
#include "onip/core/application_layer.hpp"

#include <vector>
#include <string_view>

namespace onip {
    class Application {
    public:
        virtual ~Application() = default;

        static Application* getInstance();
        static ONIP_INLINE class GlPipeline* getGraphicsPipeline() { return getInstance()->m_pipeline; }
        static ONIP_INLINE class SceneManager* getSceneManager() { return getInstance()->m_scene_manager; }
        static ONIP_INLINE class PanelHandler* getPanelHandler() { return getInstance()->m_panel_handler; }

        virtual void initializeRequirements() {}

        void run();
        ApplicationLayer* addLayer(ApplicationLayer* layer);
        const ApplicationLayer* getLayer(std::string_view name) const;
        ApplicationLayer* getLayer(std::string_view name);
    private:
        void destroyLayers();

        std::vector<ApplicationLayer*> m_layers {};
        class GlPipeline* m_pipeline { nullptr };
        class SceneManager* m_scene_manager { nullptr };
        class PanelHandler* m_panel_handler { nullptr };
    };
}

#endif // __ONIP_CORE_APPLICATION_HPP__