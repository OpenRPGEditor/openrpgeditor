#pragma once

#include <string>
#include <Utils/Typename.hpp>
#include <vector>

class DatabaseEditor;
using namespace std::string_view_literals;
class IDBEditorTab {
public:
  explicit IDBEditorTab() = default;
  virtual ~IDBEditorTab() = default;
  virtual void draw() = 0;
  std::vector<int>& headers() { return m_headers; }
  [[nodiscard]] int header(const int index) const {
    if (index < 0 || index >= m_headers.size()) {
      return -1;
    }
    
    return m_headers.at(index);
  }
  [[nodiscard]] bool hasHeader() const { return !m_headers.empty(); }
  void setHeaderRange(const int start, const int end) {
    m_categoryStart = start;
    m_categoryEnd = end;
  }

  [[nodiscard]] virtual std::string objectName(int index) const { return {}; }
  [[nodiscard]] virtual int getCount() const { return 0; }

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

template <class T>
class IDBCoreEditorTab : public IDBEditorTab {
public:
  IDBCoreEditorTab() { m_instance = static_cast<T*>(this); }
  ~IDBCoreEditorTab() override {
    if (m_instance == this) {
      m_instance = nullptr;
    }
  }

  static T* instance() { return m_instance; }

  template <class V = T>
  static constexpr std::string_view TabID() {
    return type_name<V>();
  }

  constexpr std::string_view tabId() const final { return TabID(); }

private:
  static T* m_instance;
};

template <class T>
T* IDBCoreEditorTab<T>::m_instance = nullptr;