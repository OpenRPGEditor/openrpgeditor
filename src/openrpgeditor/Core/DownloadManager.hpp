#pragma once

#include <curl/curl.h>
#include <fstream>
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

    void addBytesDownloaded(const curl_off_t bytesDownloaded) { m_bytesDownloaded += bytesDownloaded; }

  private:
    std::string m_url;
    std::string m_destinationPath;
    FILE* m_destination = nullptr;
    CURL* m_curlHandle = nullptr;
    curl_off_t m_bytesDownloaded{};
    curl_off_t m_bytesTotal{};
    int m_curlResult{CURLE_OK};
  };

  DownloadManager();
  ~DownloadManager();

  void addDownload(std::string_view url, std::string_view destination);

private:
  CURLM* m_multiHandle;
  std::vector<DownloadEntry> m_curlHandles;
};
