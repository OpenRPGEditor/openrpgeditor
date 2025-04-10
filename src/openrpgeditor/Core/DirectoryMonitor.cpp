#include "Core/DirectoryMonitor.hpp"
#include <filesystem>
#include <ranges>
#include <unordered_map>

void DirectoryMonitor::watchDirectory() {
  std::unordered_map<std::filesystem::path, std::filesystem::file_time_type> lastWriteTimes;

  while (m_running) {
    {
      std::unique_lock lock(m_mutex);
      m_cv.wait_for(lock, std::chrono::seconds(1), [this] { return !m_running; });
    }

    if (!m_running) {
      break;
    }

    if (!std::filesystem::exists(m_watchDirectory)) {
      continue;
    }

    for (const auto& entry : std::filesystem::recursive_directory_iterator(m_watchDirectory)) {
      auto lastWriteTime = last_write_time(entry);
      auto it = lastWriteTimes.find(entry);
      if (it == lastWriteTimes.end()) {
        if (is_directory(entry)) {
          emit_signal(m_directoryAdded, entry);
        } else if (is_regular_file(entry)) {
          emit_signal(m_fileAdded, entry);
        }
      } else if (it->second != lastWriteTime) {
        if (is_directory(entry)) {
          emit_signal(m_directoryChanged, entry.path());
        } else if (is_regular_file(entry)) {
          emit_signal(m_fileChanged, entry);
        }
      }

      lastWriteTimes[entry.path()] = lastWriteTime;
    }

    for (auto it = lastWriteTimes.begin(); it != lastWriteTimes.end();) {
      if (!is_regular_file(it->first) && !is_directory(it->first)) {
        if (!is_regular_file(it->first)) {
          emit_signal(m_fileDeleted, it->first);
          it = lastWriteTimes.erase(it);
        } else if (!is_directory(it->first)) {
          emit_signal(m_directoryDeleted, it->first);
          it = lastWriteTimes.erase(it);
        } else {
          ++it;
        }
      } else {
        ++it;
      }
    }
  }
}
