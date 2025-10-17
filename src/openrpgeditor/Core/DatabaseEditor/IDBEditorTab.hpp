#pragma once

#include <string>
#include <vector>

class DatabaseEditor;
using std::string_view_literals::operator""sv;
class IDBEditorTab {
public:
  explicit IDBEditorTab() {}
  virtual ~IDBEditorTab() = default;
  virtual void draw() = 0;
  std::vector<int>& getHeaders() { return m_headers; }
  int getHeader(const int index) { return m_headers.at(index); }
  bool hasHeader() { return !m_headers.empty(); }
  void setHeaderRange(const int start, const int end) {
    m_categoryStart = start;
    m_categoryEnd = end;
  }

  virtual std::string getName(int index) const { return {}; };
  virtual int getCount() const { return 0; };

  [[nodiscard]] virtual bool isReady() const = 0;
  virtual void initialize() = 0;
  [[nodiscard]] virtual bool isInitialized() const = 0;
  [[nodiscard]] virtual bool isExperimental() const { return false; }
  [[nodiscard]] virtual std::string tabName() const = 0;
  [[nodiscard]] virtual constexpr std::string_view tabId() const = 0;

  static constexpr std::vector<std::string_view> getUnicodeFormatters() { return {"\u0D9E", "\u21C4", "\u2318", "\u2248", "\u221E", "\u25bc"}; }
  static bool hasUnicodeFormatting(const std::string& text) { return text.contains("\u25bc"); }

protected:
  int m_categoryStart{};
  int m_categoryEnd{};
  std::vector<int> m_headers;
};