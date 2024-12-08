#pragma once

#include "Database/Item.hpp"

class Items {
public:
  static Items load(std::string_view filename);
  void serialize(std::string_view filename);

  [[nodiscard]] Item* item(const int id) {
    for (auto& item : m_items) {
      if (item.id() == id && item.isValid()) {
        return &item;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Item* item(const int id) const {
    for (const auto& item : m_items) {
      if (item.id() == id && item.isValid()) {
        return &item;
      }
    }

    return nullptr;
  }
  std::vector<Item>& items() { return m_items; }
  const std::vector<Item>& items() const { return m_items; }

  int count() const { return m_items.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    const int oldSize = m_items.size();
    m_items.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_items.size(); ++i) {
        m_items[i].setId(i);
      }
    }
  }

private:
  std::vector<Item> m_items;
};
