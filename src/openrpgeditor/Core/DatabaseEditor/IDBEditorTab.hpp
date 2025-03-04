#pragma once

#include <string>
#include <vector>

class DatabaseEditor;
class IDBEditorTab {
public:
  explicit IDBEditorTab(DatabaseEditor* parent) : m_parent(parent) {}
  virtual ~IDBEditorTab() = default;
  virtual void draw() = 0;
  virtual int getHeader(int index) = 0;
  virtual std::vector<int>& getHeaders() = 0;
  virtual bool hasHeader() = 0;
  virtual void setHeaderRange(int start, int end) = 0;
  virtual std::string getName(int index) = 0;
  virtual int getCount() = 0;

  std::vector<std::string> getUnicodeFormatters() { return {"\u0D9E", "\u21C4", "\u2318", "\u2248", "\u221E", "\u25bc"}; }
  bool hasUnicodeFormatting(const std::string& text) { return text.contains("\u25bc"); }

  /* TODO: Make pure */
  virtual bool isModified() { return false; };

protected:
  DatabaseEditor* m_parent;
};