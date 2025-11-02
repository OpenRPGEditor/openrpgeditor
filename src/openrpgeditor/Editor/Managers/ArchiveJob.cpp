#include "Editor/Managers/ArchiveJob.hpp"

ArchiveJob::ArchiveJob(std::filesystem::path archive, std::filesystem::path dest, ProgressCallback cb)
: m_archivePath(std::move(archive))
, m_destDir(std::move(dest))
, m_progressCallback(std::move(cb)) {

  if (!std::filesystem::exists(m_archivePath)) {
    std::print(stderr, "Archive not found: {}\n", m_archivePath.string());
    m_failed = true;
    return;
  }

  // Ensure destination exists
  std::error_code ec;
  std::filesystem::create_directories(m_destDir, ec);
  if (ec) {
    std::print(stderr, "Failed to create '{}': {}\n", m_destDir.string(), ec.message());
    m_failed = true;
    return;
  }

  // Detect single root folder
  detectSingleRoot();

  // Count entries (optional, for progress)
  m_archiveHandle = archive_read_new();
  archive_read_support_filter_all(m_archiveHandle);
  archive_read_support_format_all(m_archiveHandle);
  m_totalFiles = countEntries(m_archiveHandle, m_archivePath);
  archive_read_free(m_archiveHandle);

  // Prepare handles for extraction
  m_archiveHandle = archive_read_new();
  archive_read_support_filter_all(m_archiveHandle);
  archive_read_support_format_all(m_archiveHandle);

  m_extractHandle = archive_write_disk_new();
  archive_write_disk_set_options(m_extractHandle, ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS);
  archive_write_disk_set_standard_lookup(m_extractHandle);

  if (archive_read_open_filename(m_archiveHandle, m_archivePath.string().c_str(), 10240) != ARCHIVE_OK) {
    std::print(stderr, "Failed to open '{}': {}\n", m_archivePath.string(), archive_error_string(m_archiveHandle));
    m_failed = true;
    cleanup();
  }
}

ArchiveJob::~ArchiveJob() { cleanup(); }

bool ArchiveJob::tick() noexcept {
  if (m_done || m_failed)
    return true;

  if (!m_entry && !openNextEntry())
    return true; // either done or failed

  if (!copyChunk()) {
    // still copying current file
    return false;
  }

  // finished copying one file — finalize
  if (archive_write_finish_entry(m_extractHandle) < ARCHIVE_OK) {
    std::print(stderr, "Finish entry error: {}\n", archive_error_string(m_extractHandle));
    m_failed = true;
  }

  // Notify user about the file just extracted
  if (m_progressCallback)
    m_progressCallback(*this);

  m_entry = nullptr;
  return false;
}

size_t ArchiveJob::countEntries(archive* archive, const std::filesystem::path& path) noexcept {
  if (archive_read_open_filename(archive, path.string().c_str(), 10240) != ARCHIVE_OK) {
    return 0;
  }

  archive_entry* e = nullptr;
  size_t count = 0;
  while (archive_read_next_header(archive, &e) == ARCHIVE_OK) {
    ++count;
  }
  archive_read_close(archive);
  return count;
}

void ArchiveJob::detectSingleRoot() noexcept {
  archive* a = archive_read_new();
  archive_read_support_filter_all(a);
  archive_read_support_format_all(a);

  if (archive_read_open_filename(a, m_archivePath.string().c_str(), 10240) != ARCHIVE_OK) {
    archive_read_free(a);
    m_stripRoot.clear();
    return;
  }

  archive_entry* entry = nullptr;
  std::string root;

  while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
    std::string path = archive_entry_pathname(entry);
    if (path.empty())
      continue;

    // Extract the first path component (up to first '/')
    auto pos = path.find('/');
    std::string firstComponent = (pos == std::string::npos) ? path : path.substr(0, pos);

    if (root.empty()) {
      root = firstComponent; // first entry defines candidate root
    } else if (root != firstComponent) {
      // Found a different first component => no single root
      root.clear();
      break;
    }
  }

  archive_read_close(a);
  archive_read_free(a);

  m_stripRoot = root;
}

bool ArchiveJob::openNextEntry() noexcept {
  const int r = archive_read_next_header(m_archiveHandle, &m_entry);
  if (r == ARCHIVE_EOF) {
    m_done = true;
    cleanup();
    return false;
  }
  if (r < ARCHIVE_OK) {
    std::print(stderr, "Warning: {}\n", archive_error_string(m_archiveHandle));
  }
  if (r < ARCHIVE_WARN) {
    m_failed = true;
    m_done = true;
    cleanup();
    return false;
  }

  ++m_currentIndex;

  // Original path as string
  std::string path = archive_entry_pathname(m_entry);

  // Strip single root folder if detected
  if (!m_stripRoot.empty()) {
    std::string prefix = m_stripRoot + "/";
    prefix = std::filesystem::path(prefix).make_preferred().generic_string();
    if (path.rfind(prefix, 0) == 0) { // starts with root + '/'
      path = path.substr(prefix.size());
    } else if (path == m_stripRoot) {
      path.clear(); // root itself, treat as empty
    }
  }

  // Compute final extraction path
  m_currentOutPath = m_destDir / path;

  // Update archive entry path
  archive_entry_set_pathname(m_entry, m_currentOutPath.string().c_str());

  // Write header
  if (const int wr = archive_write_header(m_extractHandle, m_entry); wr < ARCHIVE_OK) {
    std::print(stderr, "Write header error: {}\n", archive_error_string(m_extractHandle));
    m_failed = true;
    return false;
  }

  // Reset chunk-copying state
  m_havePendingBlock = false;
  m_pendingBuff = nullptr;
  m_pendingSize = 0;
  m_pendingOffset = 0;
  m_currentFileSize = archive_entry_size(m_entry);

  return true;
}

bool ArchiveJob::copyChunk() noexcept {
  if (archive_entry_size(m_entry) == 0) {
    return true; // nothing to copy
  }

  size_t bytesThisTick = 0;
  while (bytesThisTick < kChunkSize) {
    if (!m_havePendingBlock) {
      const int r = archive_read_data_block(m_archiveHandle, &m_pendingBuff, &m_pendingSize, &m_pendingOffset);
      if (r == ARCHIVE_EOF) {
        m_havePendingBlock = false;
        return true; // finished this file
      }
      if (r < ARCHIVE_OK) {
        std::print(stderr, "Read data error: {}\n", archive_error_string(m_archiveHandle));
        m_failed = true;
        return true;
      }
      m_havePendingBlock = true;
    }

    const la_int64_t toWrite = static_cast<la_int64_t>(std::min(m_pendingSize, kChunkSize - bytesThisTick));
    if (const auto wr = archive_write_data_block(m_extractHandle, m_pendingBuff, toWrite, m_pendingOffset); wr < ARCHIVE_OK) {
      std::print(stderr, "Write data error: {}\n", archive_error_string(m_extractHandle));
      m_failed = true;
      return true;
    }

    // Adjust pointers for next tick or next partial chunk
    bytesThisTick += toWrite;
    m_pendingOffset += toWrite;
    m_pendingBuff = static_cast<const char*>(m_pendingBuff) + toWrite;
    m_pendingSize -= toWrite;

    if (m_pendingSize == 0)
      m_havePendingBlock = false;
  }

  // Notify user about extraction progress
  if (m_progressCallback && m_havePendingBlock) {
    m_progressCallback(*this);
  }

  return false; // still more data to read/write
}

void ArchiveJob::cleanup() noexcept {
  if (m_archiveHandle) {
    archive_read_close(m_archiveHandle);
    archive_read_free(m_archiveHandle);
    m_archiveHandle = nullptr;
  }
  if (m_extractHandle) {
    archive_write_close(m_extractHandle);
    archive_write_free(m_extractHandle);
    m_extractHandle = nullptr;
  }
}
