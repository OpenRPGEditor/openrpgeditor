#pragma once

#include <string>
#include <vector>

class DatabaseEditor;
using std::string_view_literals::operator""sv;
class IDBEditorTab {
public:
  explicit IDBEditorTab(DatabaseEditor* parent)
  : m_parent(parent) {}
  virtual ~IDBEditorTab() = default;
  virtual void draw() = 0;
  virtual int getHeader(int index) = 0;
  virtual std::vector<int>& getHeaders() = 0;
  virtual bool hasHeader() = 0;
  virtual void setHeaderRange(int start, int end) = 0;
  virtual std::string getName(int index) = 0;
  virtual int getCount() = 0;
  [[nodiscard]] virtual bool isReady() const = 0;
  virtual void initialize() = 0;
  [[nodiscard]] virtual bool isInitialized() const = 0;
  [[nodiscard]] virtual bool isExperimental() const { return false; }
  [[nodiscard]] virtual std::string tabName() const = 0;
  [[nodiscard]] virtual constexpr std::string_view tabId() const = 0;

  static constexpr std::vector<std::string_view> getUnicodeFormatters() { return {"\u0D9E", "\u21C4", "\u2318", "\u2248", "\u221E", "\u25bc"}; }
  static bool hasUnicodeFormatting(const std::string& text) { return text.contains("\u25bc"); }

protected:
  DatabaseEditor* m_parent;
};