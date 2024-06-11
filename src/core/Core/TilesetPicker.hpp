#pragma once

struct Project;
struct TilesetPicker {
  TilesetPicker(Project* parent) : m_parent(parent) {}

  void draw();

private:
  Project* m_parent;
};
