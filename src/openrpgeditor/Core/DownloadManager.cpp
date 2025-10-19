#include "Core/DownloadManager.hpp"

#include "EventCommands/Dialog_EnemyRecoverAll.hpp"

// TODO: For some reason this doesn't work when multiple downloads are active, there is also a random crash on exit related to the destination file handle
int writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
  const auto entry = static_cast<DownloadManager::DownloadEntry*>(userdata);
  if (entry->fileHandle() == nullptr) {
    return 0;
  }
  const auto written = fwrite(ptr, size, nmemb, entry->fileHandle());
  fflush(entry->fileHandle());
  entry->addBytesDownloaded(written);
  return written;
}

DownloadManager::DownloadEntry::DownloadEntry(const std::string_view url, const std::string_view destination)
: m_url(url)
, m_destinationPath(destination) {
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

void DownloadManager::DownloadEntry::updateInfo(const curl_off_t dltotal, const curl_off_t dlnow) {
  if (!m_bytesTotal && dltotal) {
    m_bytesTotal = dltotal;
  }
  //printf("%s %ld %ld\n", m_url.data(), dltotal, dlnow);
}

int DownloadManager::DownloadEntry::progressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
  const auto self = static_cast<DownloadEntry*>(clientp);
  self->updateInfo(dltotal, dlnow);
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

int DownloadManager::addDownload(const std::string_view url, const std::string_view destination) {
  const auto& handle = m_curlHandles.emplace_back(url, destination);
  curl_multi_add_handle(m_multiHandle, handle.handle());
  return m_curlHandles.size() - 1;
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