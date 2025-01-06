#pragma once

#include <tuple>

class EventPage;
class IPageEditor {
public:
  friend class EventPage;
  virtual ~IPageEditor() = default;
  virtual std::tuple<bool, bool> draw(bool canDelete, int index) = 0;

  /* Client code must implement this to create their own event editors */
  static IPageEditor* create(EventPage* page);

  void select() {
    m_selected = true;
    m_selectedDirty = true;
  }

  bool isSelected() const { return m_selected; }

protected:
  explicit IPageEditor(EventPage* page) : m_page(page) {}
  void setPagePtr(EventPage* page) {
    m_page = page;
    pagePointerInvalidated();
  }
  virtual void pagePointerInvalidated() = 0;
  EventPage* m_page;
  bool m_selected{false};
  bool m_selectedDirty{false};
};