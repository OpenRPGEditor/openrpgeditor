#include "Database/IModifiable.hpp"

class IModifiablePriv {
public:
  signal<void(IModifiable*)>& modifiedSignal() { return m_onModified; }

private:
  signal<void(IModifiable*)> m_onModified;
};

IModifiable::IModifiable() {}
IModifiable::IModifiable(const IModifiable&) {}
IModifiable& IModifiable::operator=(const IModifiable&) { return *this; }

IModifiable::~IModifiable() {
  delete m_onModified;
  m_onModified = nullptr;
}

signal<void(IModifiable*)>& IModifiable::onModified() const {
  if (!m_onModified) {
    m_onModified = new IModifiablePriv();
  }
  return m_onModified->modifiedSignal();
}