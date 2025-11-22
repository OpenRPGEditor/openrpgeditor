#pragma once
#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
#include <functional>
#include <print>
#include <string>

struct archive;
struct archive_entry;
class ArchiveJob {
public:
  using ProgressCallback = std::function<void(const ArchiveJob& job)>;

  ArchiveJob(std::filesystem::path archive, std::filesystem::path dest, ProgressCallback cb);

  ~ArchiveJob();

  // Advances the extraction in small non-blocking chunks.
  // Returns true when this archive is fully done.
  [[nodiscard]] bool tick() noexcept;

  [[nodiscard]] bool isDone() const noexcept { return m_done || m_failed; }
  [[nodiscard]] bool isFailed() const noexcept { return m_failed; }

  [[nodiscard]] const std::filesystem::path& currentOutPath() const noexcept { return m_currentOutPath; }
  [[nodiscard]] const std::filesystem::path& archivePath() const noexcept { return m_archivePath; }

  [[nodiscard]] size_t currentFile() const { return m_currentIndex; }
  [[nodiscard]] size_t totalFiles() const noexcept { return m_totalFiles; }
  [[nodiscard]] size_t pendingSize() const { return m_pendingSize; }
  [[nodiscard]] size_t pendingOffset() const { return m_pendingOffset; }
  [[nodiscard]] la_int64_t currentFileSize() const { return m_currentFileSize; }

private:
  std::filesystem::path m_archivePath;
  std::filesystem::path m_destDir;
  std::filesystem::path m_currentOutPath;
  ProgressCallback m_progressCallback;
  archive* m_archiveHandle = nullptr;
  archive* m_extractHandle = nullptr;
  archive_entry* m_entry = nullptr;
  const void* m_pendingBuff = nullptr;
  size_t m_pendingSize = 0;
  la_int64_t m_pendingOffset = 0;
  bool m_havePendingBlock = false;

  la_int64_t m_currentFileSize = 0;
  size_t m_currentIndex = 0;
  size_t m_totalFiles = 0;
  bool m_initialized = false;
  bool m_done = false;
  bool m_failed = false;

  std::string m_stripRoot;

  // Limit how much data is processed per tick (8 KiB)
  static constexpr size_t kChunkSize = 64 * 1024;

  // Count number of entries in the archive
  static size_t countEntries(archive* archive, const std::filesystem::path& path) noexcept;

  // Detect if the archive has a single root folder, this way we can control where exactly the contents of the archive are extracted
  void detectSingleRoot() noexcept;

  // This should be called after m_stripRoot has been set
  bool openNextEntry() noexcept;

  // Copy a chunk of data from archive to disk
  bool copyChunk() noexcept;

  // Cleanup archive handles
  void cleanup() noexcept;
};