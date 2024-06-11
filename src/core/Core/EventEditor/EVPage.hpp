#pragma once

struct EventEditor;
struct EventPage;
struct EVPage {
  EVPage(EventEditor* parent, EventPage* page)
  : m_parent(parent), m_page(page) {}


  /* Returns true when closed, closing is the equivelant of deleting */
  bool draw(bool canDelete, int index);

  void clearPage();

private:
  EventEditor* m_parent;
  EventPage* m_page;
};
