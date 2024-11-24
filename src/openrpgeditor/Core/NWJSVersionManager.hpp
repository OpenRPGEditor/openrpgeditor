#pragma once

#include <string>
#include <vector>

class NWJSVersionManager {
public:
  explicit NWJSVersionManager(std::string_view nwjsPath);

  bool getListing();

  bool downloadSelectedVersion();

  void draw();

  [[nodiscard]] bool isOpen() const { return m_isOpen; }
  void open() { m_isOpen = true; }

private:
  static size_t indexWriteCallback(const void* contents, const size_t size, const size_t nmemb, std::vector<char>* userp);
  static size_t payloadWriteCallback(const void* content, const size_t size, const size_t nmemb, FILE* userp);

  std::string m_NWJSPath;
  std::vector<std::string> m_versions;
  int m_selectedVersion = -1;
  bool m_isOpen = false;
};
