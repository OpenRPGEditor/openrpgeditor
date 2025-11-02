#include "DownloadManager.hpp"
#include <algorithm>

int writeCallback(const char* ptr, const size_t size, const size_t numMembers, void* userdata) {
  const auto entry = static_cast<DownloadManager::DownloadEntry*>(userdata);
  if (entry->fileHandle() == nullptr) {
    return 0;
  }
  const auto written = fwrite(ptr, size, numMembers, entry->fileHandle());
  fflush(entry->fileHandle());
  entry->addBytesDownloaded(static_cast<curl_off_t>(written));
  return static_cast<int>(written);
}

DownloadManager::DownloadEntry::DownloadEntry(const std::string_view url, const std::string_view destination, const void* owner)
: m_url(url)
, m_destinationPath(destination)
, m_owner(owner) {
  m_destination = fopen(m_destinationPath.data(), "wb");
  m_curlHandle = curl_easy_init();

  if (m_curlHandle) {
    curl_easy_setopt(m_curlHandle, CURLOPT_URL, m_url.data());
    curl_easy_setopt(m_curlHandle, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(m_curlHandle, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(m_curlHandle, CURLOPT_WRITEDATA, this);
    curl_easy_setopt(m_curlHandle, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(m_curlHandle, CURLOPT_XFERINFOFUNCTION, progressCallback);
    curl_easy_setopt(m_curlHandle, CURLOPT_XFERINFODATA, this);
  }
}

DownloadManager::DownloadEntry::~DownloadEntry() {
  curl_easy_cleanup(m_curlHandle);
  m_curlHandle = nullptr;
  if (m_destination) {
    fclose(m_destination);
  }
  m_destination = nullptr;
}

void DownloadManager::DownloadEntry::updateInfo(const curl_off_t downloadTotal, const curl_off_t downloadNow [[maybe_unused]]) {
  if (!m_bytesTotal && downloadTotal) {
    m_bytesTotal = downloadTotal;
  }
}

int DownloadManager::DownloadEntry::progressCallback(void* clientPointer, const curl_off_t downloadTotal, const curl_off_t downloadNow, const curl_off_t uploadTotal [[maybe_unused]],
                                                     const curl_off_t uploadNow [[maybe_unused]]) {
  const auto self = static_cast<DownloadEntry*>(clientPointer);
  self->updateInfo(downloadTotal, downloadNow);
  return CURLE_OK;
}

DownloadManager::DownloadManager() {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  m_multiHandle = curl_multi_init();
}

DownloadManager::~DownloadManager() {
  for (const auto& handle : m_curlHandles) {
    curl_multi_remove_handle(m_multiHandle, handle.handle());
  }
  m_curlHandles.clear();
  curl_multi_cleanup(m_multiHandle);
  curl_global_cleanup();
}

int DownloadManager::addDownload(const std::string_view url, const std::string_view destination, const void* owner) {
  const auto& handle = m_curlHandles.emplace_back(url, destination, owner);
  curl_multi_add_handle(m_multiHandle, handle.handle());
  return static_cast<int>(m_curlHandles.size() - 1);
}

void DownloadManager::processDownloads() {
  int stillRunning;
  curl_multi_perform(m_multiHandle, &stillRunning);

  CURLMsg* msg = nullptr;
  while ((msg = curl_multi_info_read(m_multiHandle, &stillRunning))) {
    if (msg->msg == CURLMSG_DONE) {
      if (auto it = std::ranges::find_if(m_curlHandles, [&msg](const DownloadEntry& entry) { return msg->easy_handle == entry.handle(); }); it != m_curlHandles.end()) {
        it->setCompleted();
      }
    }
  }

  if (std::ranges::all_of(m_curlHandles, [](const DownloadEntry& entry) { return entry.completed(); })) {
    for (auto& handle : m_curlHandles) {
      curl_multi_remove_handle(m_multiHandle, handle.handle());
    }
    m_curlHandles.clear();
  }
}