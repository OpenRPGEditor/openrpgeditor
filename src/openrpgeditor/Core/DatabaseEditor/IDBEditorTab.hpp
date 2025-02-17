#pragma once

class DatabaseEditor;
class IDBEditorTab {
public:
  explicit IDBEditorTab(DatabaseEditor* parent) : m_parent(parent) {}
  virtual ~IDBEditorTab() = default;
  virtual void draw() = 0;

  /* TODO: Make pure */
  virtual bool isModified() { return false; };

protected:
  DatabaseEditor* m_parent;
};