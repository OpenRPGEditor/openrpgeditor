#include "Database/IModifiable.hpp"

IModifiable::IModifiable() {}

IModifiable::IModifiable(const IModifiable& other) : m_hasChanges(other.m_hasChanges), m_modified(other.m_modified), m_signalsDisabled(other.m_signalsDisabled) {}

IModifiable& IModifiable::operator=(const IModifiable& other) {
  m_hasChanges = other.m_hasChanges;
  m_modified = other.m_modified;
  m_signalsDisabled = other.m_signalsDisabled;
  return *this;
}

IModifiable::IModifiable(IModifiable&& other) noexcept : m_hasChanges(other.m_hasChanges), m_modified(other.m_modified), m_signalsDisabled(other.m_signalsDisabled) {}

IModifiable& IModifiable::operator=(IModifiable&& other) noexcept {
  m_hasChanges = other.m_hasChanges;
  m_modified = other.m_modified;
  m_signalsDisabled = other.m_signalsDisabled;
  return *this;
}

IModifiable::~IModifiable() {}

rpgmutils::signal<void(IModifiable*)>& IModifiable::onModified() {
  if (!m_onModified) {
    m_onModified.emplace();
  }
  return *m_onModified;
}