#pragma once

#include <curl/curl.h>
#include <deque>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

class DownloadManager {
public:
  class DownloadEntry {
  public:
    DownloadEntry(std::string_view url, std::string_view destination);
    ~DownloadEntry();

    CURL* handle() const { return m_curlHandle; }
    std::string_view url() const { return m_url; }
    FILE* fileHandle() const { return m_destination; }
    curl_off_t bytesDownloaded() const { return m_bytesDownloaded; }
    curl_off_t bytesTotal() const { return m_bytesTotal; }

    double percent() const { return m_bytesTotal > 0 ? static_cast<double>(m_bytesDownloaded) / static_cast<double>(m_bytesTotal) : 0.0; }

    void addBytesDownloaded(const curl_off_t bytesDownloaded) { m_bytesDownloaded += bytesDownloaded; }

    bool completed() const { return m_completed; }
    void setCompleted() { m_completed = true; }

  private:
    friend class DownloadManager;
    void updateInfo(curl_off_t dltotal, curl_off_t dlnow);
    static int progressCallback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);
    std::string m_url;
    std::string m_destinationPath;
    FILE* m_destination = nullptr;
    CURL* m_curlHandle = nullptr;
    curl_off_t m_bytesDownloaded{};
    curl_off_t m_bytesTotal{};
    bool m_completed{false};
  };

  DownloadManager();
  ~DownloadManager();

  int addDownload(std::string_view url, std::string_view destination);

  void processDownloads();

  bool transferComplete(const int handle) const {
    if (handle < 0 || handle >= m_curlHandles.size()) {
      return true;
    }
    return m_curlHandles[handle].completed();
  }

  const std::deque<DownloadEntry>& handles() const { return m_curlHandles; }

private:
  CURLM* m_multiHandle;
  std::deque<DownloadEntry> m_curlHandles;
  bool m_transferComplete{false};
};
