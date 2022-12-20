#ifndef __ONIP_SYSTEM_GRAPHICS_EXTRACTION_HPP__
#define __ONIP_SYSTEM_GRAPHICS_EXTRACTION_HPP__

#include "onip/components/graphics_comps.hpp"
#include "onip/core/scene_manager.hpp"
#include "onip/graphics/gl_batch_renderer.hpp"

namespace onip {
    class ObtainGraphicsVertexData : public CustomSystem {
    public:
        ObtainGraphicsVertexData();
        ~ObtainGraphicsVertexData() = default;

        ONIP_INLINE const char* getName() override { return "Vertex Extraction"; };
        void onUpdate() override;
    private:
        GlBatchRenderer* m_batch { nullptr };
    };
}

#endif // __ONIP_SYSTEM_GRAPHICS_EXTRACTION_HPP__