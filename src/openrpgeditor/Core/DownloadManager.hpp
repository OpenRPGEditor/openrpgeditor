#pragma once

#include <string>
#include <vector>
#include <curl/curl.h>
class DownloadManager {
public:
  struct DownloadEntry {
    std::string url;
    CURL* curlHandle;
    curl_off_t bytesDownloaded;
    curl_off_t bytesTotal;
  };

  DownloadManager();


private:
  CURLM* m_multiHandle;
  std::vector<CURL*> m_curlHandles;
};
