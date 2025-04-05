#pragma once
#include <vector>

class DatabaseEditor;
class IDBTemplates {
public:
  explicit IDBTemplates(DatabaseEditor* parent)
  : m_parent(parent) {}
  virtual ~IDBTemplates() = default;
  virtual std::vector<int> getData() { return {}; }
  virtual bool hasChanges() { return false; }
  virtual void draw() = 0;

protected:
  DatabaseEditor* m_parent;
};