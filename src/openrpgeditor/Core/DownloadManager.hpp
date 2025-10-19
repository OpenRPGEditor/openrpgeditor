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

    [[nodiscard]] CURL* handle() const { return m_curlHandle; }
    [[nodiscard]] std::string_view url() const { return m_url; }
    [[nodiscard]] FILE* fileHandle() const { return m_destination; }
    [[nodiscard]] curl_off_t bytesDownloaded() const { return m_bytesDownloaded; }
    [[nodiscard]] curl_off_t bytesTotal() const { return m_bytesTotal; }

    [[nodiscard]] float percent() const { return m_bytesTotal > 0 ? static_cast<float>(m_bytesDownloaded) / static_cast<float>(m_bytesTotal) : 0.f; }

    void addBytesDownloaded(const curl_off_t bytesDownloaded) { m_bytesDownloaded += bytesDownloaded; }

    [[nodiscard]] bool completed() const { return m_completed; }
    void setCompleted() { m_completed = true; }

  private:
    friend class DownloadManager;
    void updateInfo(curl_off_t downloadTotal, curl_off_t downloadNow);
    static int progressCallback(void* clientPointer, curl_off_t downloadTotal, curl_off_t downloadNow, curl_off_t uploadTotal, curl_off_t uploadNow);
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

  [[nodiscard]] bool transferComplete(const int handle) const {
    if (handle < 0 || handle >= m_curlHandles.size()) {
      return true;
    }
    return m_curlHandles[handle].completed();
  }

  [[nodiscard]] const std::deque<DownloadEntry>& handles() const { return m_curlHandles; }

private:
  CURLM* m_multiHandle;
  std::deque<DownloadEntry> m_curlHandles;
  bool m_transferComplete{false};
};
