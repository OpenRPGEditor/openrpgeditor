#pragma once

struct IDBEditorTab {
  virtual ~IDBEditorTab() = default;
  virtual void draw() = 0;
};