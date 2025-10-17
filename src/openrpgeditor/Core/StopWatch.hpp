#pragma once
#include <chrono>

// Code adapted from https://raw.githubusercontent.com/AxioDL/metaforce/refs/heads/main/Runtime/CStopwatch.hpp,
// https://raw.githubusercontent.com/AxioDL/metaforce/refs/heads/main/Runtime/CStopwatch.cpp
// I'm (antidote) the original author

class StopWatch {
  static StopWatch m_GlobalTimer;

  using Time = std::chrono::steady_clock;
  using MicroSeconds = std::chrono::microseconds;
  using MilliSeconds = std::chrono::milliseconds;
  using FloatSeconds = std::chrono::duration<float>;

  Time::time_point m_startTime;

public:
  StopWatch();

  static void initGlobalTimer();
  static StopWatch& globalTimerObj() { return m_GlobalTimer; }
  static float globalTime() { return m_GlobalTimer.elapsedSeconds(); }

  void reset();
  void wait(float wait) const;

  [[nodiscard]] float elapsedSeconds() const;
  [[nodiscard]] uint64_t elapsedMillis() const;
  [[nodiscard]] uint64_t elapsedMicros() const;

  static float curSeconds();
  static uint64_t curMillis();
  static uint64_t curMicros();
};
