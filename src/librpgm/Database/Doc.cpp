#include "Database/Doc.hpp"

Doc::Doc(const Doc& other)
: IModifiable(other)
, m_texts(other.m_texts)
, m_names(other.m_names) {}

Doc& Doc::operator=(const Doc& other) {
  IModifiable::operator=(other);
  m_texts = other.m_texts;
  m_names = other.m_names;
  m_oldtexts = other.m_oldtexts;
  m_oldnames = other.m_oldnames;
  return *this;
}

Doc::Doc(Doc&& other) noexcept
: IModifiable(std::move(other))
, m_texts(other.m_texts)
, m_names(std::move(other.m_names))
, m_oldtexts(other.m_oldtexts)
, m_oldnames(std::move(other.m_oldnames)) {}

Doc& Doc::operator=(Doc&& other) noexcept {
  IModifiable::operator=(std::move(other));
  m_texts = other.m_texts;
  m_names = std::move(other.m_names);
  m_oldtexts = other.m_oldtexts;
  m_oldnames = std::move(other.m_oldnames);
  return *this;
}

[[nodiscard]] std::vector<std::string> Doc::texts() const { return m_texts; }
void Doc::setTexts(const std::vector<std::string> texts) {
  if (m_texts == texts) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(texts);
  m_texts = texts;
  if (!signalsDisabled()) {
    textsModified().fire(this, texts);
  }
  setHasChanges();
}

[[nodiscard]] const std::vector<std::string>& Doc::names() const { return m_names; }
void Doc::setNames(const std::vector<std::string>& names) {
  if (m_names == names) {
    return;
  }
  MODIFIABLE_SET_OLD_VALUE(names);
  m_names = names;
  if (!signalsDisabled()) {
    namesModified().fire(this, names);
  }
  setHasChanges();
}

void Doc::restoreOriginal() {
  IModifiable::restoreOriginal();
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(texts);
  MODIFIABLE_RESTORE_ORIGINAL_VALUE(names);
}
void Doc::acceptChanges() {
  IModifiable::acceptChanges();
  MODIFIABLE_ACCEPT_VALUE(texts);
  MODIFIABLE_ACCEPT_VALUE(names);
}
nlohmann::ordered_json Doc::serializeOldValues() const { return {MODIFIABLE_SERIALIZE_VALUE(texts), MODIFIABLE_SERIALIZE_VALUE(names)}; }

rpgmutils::signal<void(Doc*, std::vector<std::string>)>& Doc::textsModified() {
  if (!m_textsModified) {
    m_textsModified.emplace();
  }
  return *m_textsModified;
}
rpgmutils::signal<void(Doc*, const std::vector<std::string>&)>& Doc::namesModified() {
  if (!m_namesModified) {
    m_namesModified.emplace();
  }
  return *m_namesModified;
}

void to_json(nlohmann::ordered_json& j, const Doc& doc) {
  j = {
      {"texts", doc.m_texts},
      {"names", doc.m_names},
  };
}

void from_json(const nlohmann::ordered_json& j, Doc& doc) {
  doc.m_texts = j.value("texts", doc.m_texts);
  doc.m_names = j.value("names", doc.m_names);
}

Doc Doc::load(std::string_view path) {
  if (std::ifstream file(path.data()); file.is_open()) {
    try {
      nlohmann::ordered_json data = nlohmann::ordered_json::parse(file);
      Doc ret;
      data.get_to(ret);
      return ret;
    } catch (...) {}
  }

  return {};
}

bool Doc::serialize(const std::string_view path) {
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