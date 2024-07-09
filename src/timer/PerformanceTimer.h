//
// Created by bhjd1010 on 2024/7/9.
//

#pragma once

#include <chrono>
#include <optional>
#include <functional>

enum class ETimeType : uint8_t {
    nanoseconds,
    microseconds,
    milliseconds,
    seconds,
    minutes,
    hours,
};

class PerformanceTimer {
public:
    PerformanceTimer() = default;
    explicit PerformanceTimer(bool bStart);

    void Start();
    void End();
    int64_t Duration(ETimeType timeType = ETimeType::milliseconds);

private:
    std::optional<std::chrono::time_point<std::chrono::steady_clock>> _start;
    std::optional<std::chrono::time_point<std::chrono::steady_clock>> _end;
};

class PerformanceTimerScope {
public:
    explicit PerformanceTimerScope(
            const std::function<void(int64_t duration)> &CallInDestructor = std::function < void(int64_t) > ())
            : _callInDestructor(CallInDestructor) {
        _performanceTimer.Start();
    }

    ~PerformanceTimerScope() {
        _performanceTimer.End();
        _callInDestructor(_performanceTimer.Duration());
    }

private:
    std::function<void(int64_t duration)> _callInDestructor;
    PerformanceTimer _performanceTimer;
};