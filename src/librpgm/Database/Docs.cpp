#include "Database/Docs.hpp"

Docs::Docs(const Docs& other) : IModifiable(other), m_id(other.m_id), m_string(other.m_string) {}

Docs& Docs::operator=(const Docs& other) {
  IModifiable::operator=(other);
  m_id = other.m_id;
  m_string = other.m_string;
  m_oldid = other.m_oldid;
  m_oldstring = other.m_oldstring;
  return *this;
}

Docs::Docs(Docs&& other) noexcept : IModifiable(other), m_id(other.m_id), m_string(std::move(other.m_string)), m_oldid(other.m_oldid), m_oldstring(std::move(other.m_oldstring)) {}

Docs& Docs::operator=(Docs&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_id = other.m_id;
  m_string = std::move(other.m_string);
  m_oldid = other.m_oldid;
  m_oldstring = std::move(other.m_oldstring);
  return *this;
}

[[nodiscard]] int Docs::id() const { return m_id; }
void Docs::setId(const int id) {
  if (m_id == id) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(id);
  m_id = id;
  if (!signalsDisabled()) {
    idModified().fire(this, id);
  }
  setHasChanges();
}

[[nodiscard]] const std::string& Docs::str() const { return m_string; }
void Docs::setString(const std::string& str) {
  if (m_string == str) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(string);
  m_string = str;
  if (!signalsDisabled()) {
    stringModified().fire(this, str);
  }
  setHasChanges();
}

void Docs::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(id);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(string);
}
void Docs::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(id);
  MODIFIABLE_ACCEPT_VALUE(string);
}
nlohmann::ordered_json Docs::serializeOldValues() const { return {MODIFIABLE_SERIALIZE_VALUE(id), MODIFIABLE_SERIALIZE_VALUE(string)}; }

rpgmutils::signal<void(Docs*, int)>& Docs::idModified() {
  if (!m_idModified) {
    m_idModified.emplace();
  }
  return *m_idModified;
}
rpgmutils::signal<void(Docs*, const std::string&)>& Docs::stringModified() {
  if (!m_stringModified) {
    m_stringModified.emplace();
  }
  return *m_stringModified;
}

void to_json(nlohmann::ordered_json& j, const Docs& doc) {
  j = {
      {"id", doc.m_id},
      {"string", doc.m_string},
  };
}

void from_json(const nlohmann::ordered_json& j, Docs& doc) {
  doc.m_id = j.value("id", doc.m_id);
  doc.m_string = j.value("string", doc.m_string);
}

Docs Docs::load(std::string_view path) {
  if (std::ifstream file(path.data()); file.is_open()) {
    try {
      nlohmann::ordered_json data = nlohmann::ordered_json::parse(file);
      Docs ret;
      data.get_to(ret);
      return ret;
    } catch (...) {}
  }

  return {};
}

bool Docs::serialize(const std::string_view path) {
  std::ofstream file(path.data());
  try {
    if (file.is_open()) {
      const nlohmann::ordered_json data = *this;
      file << data.dump(4);
      return true;
    }
  } catch (...) {}
  return false;
}