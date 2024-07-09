//
// Created by bhjd1010 on 2024/7/9.
//

#include "PerformanceTimer.h"
#include <cassert>

PerformanceTimer::PerformanceTimer(bool bStart) {
    if(bStart)
        Start();
}

void PerformanceTimer::Start() {
    _start = std::chrono::high_resolution_clock::now();
}

void PerformanceTimer::End() {
    _end = std::chrono::high_resolution_clock::now();
}

int64_t PerformanceTimer::Duration(ETimeType timeType) {
    assert(_start.has_value() && _end.has_value());
    switch (timeType) {
        case ETimeType::milliseconds:
            return std::chrono::duration_cast<std::chrono::milliseconds>(_end.value() - _start.value()).count();
        case ETimeType::nanoseconds:
            return std::chrono::duration_cast<std::chrono::nanoseconds>(_end.value() - _start.value()).count();
        case ETimeType::microseconds:
            return std::chrono::duration_cast<std::chrono::microseconds>(_end.value() - _start.value()).count();
        case ETimeType::seconds:
            return std::chrono::duration_cast<std::chrono::seconds>(_end.value() - _start.value()).count();
        case ETimeType::minutes:
            return std::chrono::duration_cast<std::chrono::minutes>(_end.value() - _start.value()).count();
        case ETimeType::hours:
            return std::chrono::duration_cast<std::chrono::hours>(_end.value() - _start.value()).count();
    }
    return 0;
}
