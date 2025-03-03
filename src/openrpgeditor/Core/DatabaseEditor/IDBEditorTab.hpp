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

  bool hasUnicodeFormatting(const std::string& text) { return text.contains("\u25bc"); }

  /* TODO: Make pure */
  virtual bool isModified() { return false; };

protected:
  DatabaseEditor* m_parent;
};