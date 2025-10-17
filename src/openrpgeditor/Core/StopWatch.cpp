#include "Core/StopWatch.hpp"
#include <cmath>

StopWatch StopWatch::m_GlobalTimer{};

float StopWatch::elapsedSeconds() const {
  // Convert time to seconds
  return static_cast<float>(std::chrono::duration_cast<MilliSeconds>(Time::now() - m_startTime).count()) / 1000.f;
}

uint64_t StopWatch::elapsedMicros() const {
  // Convert time to microseconds
  return std::chrono::duration_cast<MicroSeconds>(Time::now() - m_startTime).count();
}

uint64_t StopWatch::elapsedMillis() const {
  // convert time to milliseconds
  return std::chrono::duration_cast<MilliSeconds>(Time::now() - m_startTime).count();
}

float StopWatch::curSeconds() {
  // Current system seconds
  return static_cast<float>(std::chrono::duration_cast<MilliSeconds>(Time::now().time_since_epoch()).count()) / 1000.f;
}

uint64_t StopWatch::curMillis() {
  // Get current system microseconds
  return std::chrono::duration_cast<MilliSeconds>(Time::now().time_since_epoch()).count();
}

uint64_t StopWatch::curMicros() {
  // Get current system microseconds
  return std::chrono::duration_cast<MicroSeconds>(Time::now().time_since_epoch()).count();
}

void StopWatch::initGlobalTimer() { m_GlobalTimer.reset(); }

void StopWatch::reset() { m_startTime = Time::now(); }

void StopWatch::wait(float wait) const {
  if (std::fabs(wait) < 0.001f) {
    wait = 0.f;
  }

  const auto waitDur = FloatSeconds{wait};

  while ((Time::now() - m_startTime) < waitDur) {}
}
