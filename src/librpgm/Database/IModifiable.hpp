#pragma once
#include "Utils/SignalSlot.hpp"

class IModifiablePriv;
class IModifiable {
public:
  IModifiable();
  IModifiable(const IModifiable& other);
  IModifiable& operator=(const IModifiable& other);
  IModifiable(IModifiable&& other) = default;
  IModifiable& operator=(IModifiable&& other) = default;
  virtual ~IModifiable();
  [[nodiscard]] bool isModified() const { return m_modified; }
  void setModified(const bool modified = true) {
    m_modified = modified;
    if (m_modified) {
      onModified().fire(this);
    }
  }

  signal<void(IModifiable*)>& onModified() const;

private:
  mutable IModifiablePriv* m_onModified = nullptr;
  bool m_modified{false};
};