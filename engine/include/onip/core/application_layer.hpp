#ifndef __ONIP_CORE_LAYER_HPP__
#define __ONIP_CORE_LAYER_HPP__

#include <string>
#include <string_view>

namespace onip {
    class ApplicationLayer {
    public:
        ApplicationLayer(std::string_view name) : m_name(name) {}
        virtual ~ApplicationLayer() = default;

        virtual void on_update() {}

        const std::string& name() const { return m_name; }
    private:
        std::string m_name;
    };
}

#endif // __ONIP_CORE_LAYER_HPP__