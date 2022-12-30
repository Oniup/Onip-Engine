#include "onip/utils/utils.hpp"

#include <stdio.h>
#include <glfw/glfw3.h>
#include "onip/core/application.hpp"
#include "onip/graphics/window.hpp"

// Not going to pretend to understand what is going on here, got this solution from here:
// https://stackoverflow.com/questions/33010010/how-to-generate-random-64-bit-unsigned-integer-in-c
#define IMAX_BITS(m) ((m)/((m)%255+1) / 255%255*8 + 7-86/((m)%255+12))
#define RAND_MAX_WIDTH IMAX_BITS(RAND_MAX)
static_assert((RAND_MAX & (RAND_MAX + 1u)) == 0, "RAND_MAX not a Mersenne number");

namespace onip {
    float Time::s_delta_time = 0.0f;
    float Time::s_fixed_delta_time = 0.0f;

    int Utils::randInt32() {
        return rand();
    }

    uint32_t Utils::randUint32() {
        return static_cast<uint32_t>(rand());
    }

    uint64_t Utils::randUint64() {
        // Not going to pretend to understand what is going on here, got this solution from here:
        // https://stackoverflow.com/questions/33010010/how-to-generate-random-64-bit-unsigned-integer-in-c
        uint64_t r = 0;
        for (int i = 0; i < 64; i += RAND_MAX_WIDTH) {
            r <<= RAND_MAX_WIDTH;
            r ^= (unsigned) rand();
        }
        return r;
    }

    int Utils::findGcd(int a, int b) {
        if (a == 0) {
            return b;
        }
        else if (b == 0) {
            return a;
        }

        int result = a % b;
        return findGcd(b, result);
    }

    Utils::Stopwatch::Stopwatch(float duration) : m_start(Time::getElapsed()), m_end(Time::getElapsed() + duration) {
    }

    void Utils::Stopwatch::startTimer(float duration) {
        m_start = Time::getElapsed();
    }

    bool Utils::Stopwatch::finished() const {
        return Time::getElapsed() > m_end;
    }

    float Utils::Stopwatch::getTotalDuration() const {
        return m_end - m_start;
    }

    float Utils::Stopwatch::getDurationLeft() const {
        return m_end - Time::getElapsed();
    }

    float Utils::Stopwatch::getStartTime() const {
        return m_start;
    }

    float Utils::Stopwatch::getEndTime() const {
        return m_end;
    }

    float Utils::Stopwatch::getTotalDurationMil() const {
        return getTotalDuration() * 1000.0f;
    }

    float Utils::Stopwatch::getDurationLeftMil() const {
        return getDurationLeft() * 1000.0f;
    }

    float Utils::Stopwatch::getStartTimeMil() const {
        return m_start * 1000.0f;
    }

    float Utils::Stopwatch::getEndTimeMil() const {
        return m_end * 1000.0f;
    }

    float Time::getElapsed() {
        return static_cast<float>(glfwGetTime());
    }

    float Time::getElapsedMil() {
        return getElapsed() * 1000.0f;
    }

    float Time::getDeltaTime() {
        return s_delta_time;
    }

    float Time::getFixedDeltaTime() {
        return s_fixed_delta_time;
    }

    void Time::calcDeltaTime() {
        float time = getElapsed();
        static float last_time = 0.0f;
        s_delta_time = time - last_time;
        last_time = time;
    }

    void Time::calcFixedDeltaTime() {
        float time = getElapsed();
        static float last_time = 0.0f;
        s_fixed_delta_time = time - last_time;
        last_time = time;
    }
}