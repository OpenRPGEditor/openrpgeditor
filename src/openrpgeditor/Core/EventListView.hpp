#pragma once

struct MainWindow;
struct EventListView {
  EventListView(MainWindow* parent) : m_parent(parent) {}

  void draw();

private:
  MainWindow* m_parent;
};
