#pragma once

struct Project;
struct EventListView {
  EventListView(Project* parent) : m_parent(parent) {}


  void draw();
private:
  Project* m_parent;
};
