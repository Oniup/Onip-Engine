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
        ONIP_INLINE static class GlPipeline* getGraphicsPipeline() { return getInstance()->m_pipeline; }
        ONIP_INLINE static class SceneManager* getSceneManager() { return getInstance()->m_scene_manager; }
        ONIP_INLINE static class PanelHandler* getPanelHandler() { return getInstance()->m_panel_handler; }
        ONIP_INLINE static ApplicationLayer* addLayer(ApplicationLayer* layer) { return getInstance()->impl_addLayer(layer); }
        ONIP_INLINE static ApplicationLayer* getLayer(std::string_view name) { return getInstance()->impl_getLayer(name); }
        static void quit();

        virtual void initializeRequirements() {}

        void run();
    private:
        ApplicationLayer* impl_addLayer(ApplicationLayer* layer);
        ApplicationLayer* impl_getLayer(std::string_view name);
        void destroyLayers();

        std::vector<ApplicationLayer*> m_layers {};
        class GlPipeline* m_pipeline { nullptr };
        class SceneManager* m_scene_manager { nullptr };
        class PanelHandler* m_panel_handler { nullptr };
    };
}

#endif // __ONIP_CORE_APPLICATION_HPP__