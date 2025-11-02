#include "Editor/Managers/ArchiveManager.hpp"

void ArchiveManager::defaultCallback(const ArchiveJob& job) {
  double pct = job.totalFiles() ? (100.0 * static_cast<double>(job.currentFile()) / static_cast<double>(job.totalFiles())) : 0.0;
  double filePct = job.currentFileSize() ? (100.0 * static_cast<double>(job.pendingOffset()) / static_cast<double>(job.currentFileSize())) : 100.0;
  if (!job.isFailed())
    std::print("  [{:>3.0f}%][{:>3.0f}%] {}\n", pct, filePct, job.currentOutPath().generic_string());
  else
    std::print(stderr, "  [{:>3.0f}%][{:>3.0f}%] Failed: {}\n", pct, filePct, job.currentOutPath().generic_string());
}

int ArchiveManager::addJob(std::filesystem::path archive, std::filesystem::path dest, ArchiveJob::ProgressCallback callback) {
  m_jobs.emplace_back(std::move(archive), std::move(dest), std::move(callback));
  return static_cast<int>(m_jobs.size() - 1);
}

bool ArchiveManager::proc() noexcept {
  bool anyActive = false;
  for (auto& job : m_jobs) {
    if (!job.isDone() && !job.tick()) {
      anyActive = true;
    }
  }

  if (!anyActive) {
    m_jobs.clear();
  }

  return !anyActive;
}

ArchiveJob* ArchiveManager::getJob(const int handle) {
  if (handle < 0 || handle >= m_jobs.size())
    return nullptr;
  return &m_jobs[handle];
}

ArchiveManager& ArchiveManager::instance() {
  static ArchiveManager _instance;
  return _instance;
}
