#include "onip/core/debug.hpp"

namespace onip {
    Debug* Debug::s_instance = nullptr;

    Debug::Debug() : ApplicationLayer("Debug") {
        if (s_instance != nullptr) {
            logWarning("Cannot create another Debug Application Layer Instance");
        }
        else {
            s_instance = this;
        }
    }

    void Debug::logMessage(std::string_view message) {
        log(message, DebugLogSeverity_Message);
    }

    void Debug::logWarning(std::string_view warning) {
        log(warning, DebugLogSeverity_Warning);
    }

    void Debug::logError(std::string_view error) {
        log(error, DebugLogSeverity_Error);
    }

    void Debug::log(std::string_view log, DebugLogSeverity severity) {
        s_instance->m_logs.push_back(std::tuple<std::string, DebugLogSeverity>(log, severity));
    }

    void Debug::clearLogs() {
        s_instance->m_logs.clear();
    }
}