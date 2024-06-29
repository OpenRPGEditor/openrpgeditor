#pragma once

struct DatabaseEditor;
struct IDBEditorTab {
  explicit IDBEditorTab(DatabaseEditor* parent) : m_parent(parent) {}
  virtual ~IDBEditorTab() = default;
  virtual void draw() = 0;

protected:
  DatabaseEditor* m_parent;
};