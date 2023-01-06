#ifndef __ONIP_ENGINE_CORE_DEBUG_HPP__
#define __ONIP_ENGINE_CORE_DEBUG_HPP__

#include "onip/utils/utils.hpp"
#include "onip/core/application_layer.hpp"

#include <vector>
#include <tuple>
#include <string>

namespace onip {
    enum DebugLogSeverity {
        DebugLogSeverity_Message    = 0x01,
        DebugLogSeverity_Warning    = 0x02,
        DebugLogSeverity_Error      = 0x04,
    };

    class Debug : public ApplicationLayer {
    public:
        Debug();
        ~Debug() override = default;

        ONIP_INLINE static const std::vector<std::tuple<std::string, DebugLogSeverity>>& getLogs() { return s_instance->m_logs; }

        static void logMessage(std::string_view message);
        static void logWarning(std::string_view warning);
        static void logError(std::string_view error);
        static void log(std::string_view log, DebugLogSeverity severity);
        static void clearLogs();
    private:
        static Debug* s_instance;

        std::vector<std::tuple<std::string, DebugLogSeverity>> m_logs {};
    };
}

#endif // __ONIP_ENGINE_CORE_DEBUG_HPP__