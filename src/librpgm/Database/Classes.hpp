#pragma once

#include "Database/Class.hpp"

#include <vector>
class Classes final : public IModifiable {
public:
  Classes() { m_classes.emplace_back(); }
  [[nodiscard]] Class* classType(const int id) {
    for (auto& cls : m_classes) {
      if (cls.id() == id && cls.isValid()) {
        return &cls;
      }
    }
    return nullptr;
  }

  [[nodiscard]] const Class* classType(const int id) const {
    for (const auto& cls : m_classes) {
      if (cls.id() == id && cls.isValid()) {
        return &cls;
      }
    }

    return nullptr;
  }

  const std::vector<Class>& classes() const { return m_classes; }
  std::vector<Class>& classes() { return m_classes; }

  int count() const { return m_classes.size() - 1; }

  void resize(int newSize) {
    ++newSize;
    const int oldSize = m_classes.size();
    m_classes.resize(newSize);
    if (newSize > oldSize) {
      for (int i = oldSize; i < m_classes.size(); ++i) {
        m_classes[i].setId(i);
      }
    }
    setModified();
  }

  bool isModified() const override {
    return IModifiable::isModified() | std::ranges::any_of(m_classes, [](const auto& cls) { return cls.isModified(); });
  }

private:
  std::vector<Class> m_classes;
};
