#include "Core/DownloadManager.hpp"

void writeCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
  const auto entry = static_cast<DownloadManager::DownloadEntry*>(userdata);
  fwrite(ptr, size, nmemb, entry->fileHandle());
  entry->addBytesDownloaded(size * nmemb);
}

DownloadManager::DownloadEntry::DownloadEntry(const std::string_view url, const std::string_view destination) : m_url(url), m_destinationPath(destination) {
  m_destination = fopen(m_destinationPath.data(), "wb");
  m_destinationPath = destination;
  m_curlHandle = curl_easy_init();

  if (m_curlHandle) {}
}

DownloadManager::DownloadManager() {
  curl_global_init(CURL_GLOBAL_DEFAULT);
  m_multiHandle = curl_multi_init();
}

DownloadManager::~DownloadManager() {
  curl_multi_cleanup(m_multiHandle);
  curl_global_cleanup();
}

void DownloadManager::addDownload(const std::string_view url, const std::string_view destination) { m_curlHandles.emplace_back(url, destination); }
