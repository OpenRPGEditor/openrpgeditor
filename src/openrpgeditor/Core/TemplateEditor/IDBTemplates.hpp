#pragma once
#include <vector>

class DatabaseEditor;
class IDBTemplates {
public:
  virtual ~IDBTemplates() = default;
  virtual std::vector<int> getData() { return {}; }
  virtual bool hasChanges() { return false; }
  virtual void draw() = 0;
};