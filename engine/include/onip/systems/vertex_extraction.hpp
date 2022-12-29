#ifndef __ONIP_SYSTEM_GRAPHICS_EXTRACTION_HPP__
#define __ONIP_SYSTEM_GRAPHICS_EXTRACTION_HPP__

#include "onip/utils/utils.hpp"
#include "onip/components/graphics_comps.hpp"
#include "onip/core/scene_manager.hpp"
#include "onip/graphics/gl_batch_renderer.hpp"

#include <vector>
#include <tuple>

namespace onip {
    class GraphicsVertexExtractionSystem : public CustomSystem {
    public:
        GraphicsVertexExtractionSystem(GlBatchRenderer* renderer = nullptr);
        ~GraphicsVertexExtractionSystem() = default;

        ONIP_INLINE const char* getName() override { return "Vertex Extraction"; };
        void onUpdate() override;
    private:
        GlBatchRenderer* m_batch_renderer { nullptr };
        class Pool* m_target_pool { nullptr };
    };
}

#endif // __ONIP_SYSTEM_GRAPHICS_EXTRACTION_HPP__