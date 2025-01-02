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

    for (const auto& entry : std::filesystem::recursive_directory_iterator(m_watchDirectory)) {
      auto lastWriteTime = last_write_time(entry);
      auto it = lastWriteTimes.find(entry);
      if (it == lastWriteTimes.end()) {
        if (is_directory(entry)) {
          m_directoryAdded.fire(entry);
        } else if (is_regular_file(entry)) {
          m_fileAdded.fire(entry);
        }
      } else if (it->second != lastWriteTime) {
        if (is_directory(entry)) {
          m_directoryChanged.fire(entry.path());
        } else if (is_regular_file(entry)) {
          m_fileChanged.fire(entry);
        }
      }

      lastWriteTimes[entry.path()] = lastWriteTime;
    }

    for (auto it = lastWriteTimes.begin(); it != lastWriteTimes.end();) {
      if (!is_regular_file(it->first) && !is_directory(it->first)) {
        if (!is_regular_file(it->first)) {
          m_fileDeleted.fire(it->first);
          it = lastWriteTimes.erase(it);
        } else if (!is_directory(it->first)) {
          m_directoryDeleted.fire(it->first);
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
