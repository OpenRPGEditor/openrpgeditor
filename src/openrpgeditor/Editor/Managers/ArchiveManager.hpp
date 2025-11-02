#pragma once

#include "Editor/Managers/ArchiveJob.hpp"

#include <deque>
#include <filesystem>

class ArchiveManager {
public:
  ArchiveManager(const ArchiveManager&) = delete;
  ArchiveManager(const ArchiveManager&&) = delete;
  ArchiveManager& operator=(const ArchiveManager&) = delete;
  ArchiveManager& operator=(const ArchiveManager&&) = delete;

  int addJob(std::filesystem::path archive, std::filesystem::path dest, ArchiveJob::ProgressCallback callback = defaultCallback);

  bool proc() noexcept;

  [[nodiscard]] bool hasJobs() const noexcept { return !m_jobs.empty(); }
  ArchiveJob* getJob(int handle);

  static ArchiveManager& instance();
  [[nodiscard]] const std::deque<ArchiveJob>& jobs() const noexcept { return m_jobs; }

private:
  ArchiveManager() = default;
  std::deque<ArchiveJob> m_jobs;

  static void defaultCallback(const ArchiveJob& job);
};
