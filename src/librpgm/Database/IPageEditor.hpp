#pragma once

#include <tuple>

class EventPage;
class IPageEditor {
public:
  virtual ~IPageEditor() = default;
  virtual std::tuple<bool, bool> draw(bool canDelete, int index) = 0;

  /* Client code must implement this to create their own event editors */
  static IPageEditor* create(EventPage* page);

protected:
  explicit IPageEditor(EventPage* page) : m_page(page) {}
  EventPage* m_page;
};