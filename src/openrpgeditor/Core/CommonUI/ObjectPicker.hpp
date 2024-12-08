#pragma once

#include "Database/CommonEvents.hpp"
#include "IDialogController.hpp"

#include <optional>
#include <string>
#include <vector>

/*!
 * @struct ObjectPicker
 * @details Provides a pickable list broken up into groups of 100
 * @tparam T Object type to list
 */
template <typename T>
class ObjectPicker {
public:
  ObjectPicker(const std::string_view name, std::vector<T>& list, const int initialSelection) : m_name(name), m_list(&list), m_selection(initialSelection) {}

  std::tuple<bool, bool> draw();

  [[nodiscard]] int selection() const { return m_selection; }

  static int getId(const T& value) { return value.id(); }
  static const std::string& getName(const T& value) { return value.name(); }

  [[nodiscard]] bool isOpen() const { return m_open; }
  [[nodiscard]] bool isConfirmed() const { return m_confirmed; }
  void setOpen(const bool open) { m_open = open; }
  void accept() {
    m_open = false;
    m_confirmed = false;
  }

  void setNoSelectionMeansAdd(const bool noSelectionMeansAdd) { m_noSelectionMeansAdd = noSelectionMeansAdd; }
  void setUseDummyEntry(const std::string_view type) {
    m_useDummyEntry = true;
    m_entireText = type;
  }

private:
  std::string m_name;
  std::vector<T>* m_list;
  bool m_confirmed{false};
  int m_selection{0};
  bool m_open{false};
  bool m_noSelectionMeansAdd{false};
  std::string m_filter;
  std::string m_entireText;
  bool m_useDummyEntry{false};
};

#include "ObjectPicker.inl"