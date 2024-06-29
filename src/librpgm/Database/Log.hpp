#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include <memory>

namespace RPGM {

class Log {
 public:
  Log(const Log&) = delete;
  Log(const Log&&) = delete;
  Log& operator=(const Log&) = delete;
  Log& operator=(const Log&&) = delete;
  ~Log() = default;

  static std::shared_ptr<spdlog::logger>& logger() {
    return get().m_logger;
  }

 private:
  // The constructor shall not be deleted but used to bootstrap the logger. Ignoring
  // the lint warning is ignoring doing `Log() = delete`.
  // NOLINTNEXTLINE
  Log();

  static Log& get() {
    static Log instance{};
    return instance;
  }

  std::shared_ptr<spdlog::logger> m_logger;
};

}  // namespace App

#ifndef RPGM_DEACTIVATE_LOGGING

#if DEBUG
#define RPGM_TRACE(...) ::RPGM::Log::logger()->trace(__VA_ARGS__)
#define RPGM_DEBUG(...) ::RPGM::Log::logger()->debug(__VA_ARGS__)
#else
#define RPGM_TRACE(...)
#define RPGM_DEBUG(...)
#endif

#define RPGM_INFO(...) ::RPGM::Log::logger()->info(__VA_ARGS__)
#define RPGM_WARN(...) ::RPGM::Log::logger()->warn(__VA_ARGS__)
#define RPGM_ERROR(...) ::RPGM::Log::logger()->error(__VA_ARGS__)
#define RPGM_FATAL(...) ::RPGM::Log::logger()->fatal(__VA_ARGS__)

#else

#define RPGM_TRACE(...)
#define RPGM_DEBUG(...)
#define RPGM_INFO(...)
#define RPGM_WARN(...)
#define RPGM_ERROR(...)
#define RPGM_FATAL(...)

#endif
