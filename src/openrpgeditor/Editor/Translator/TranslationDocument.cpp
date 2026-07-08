#include "Editor/Translator/TranslationDocument.hpp"

#include "Editor/Log.hpp"
#include "TranslatorDialog.hpp"

#include <Database/Database.hpp>
#include <ranges>

bool TranslationDocument::open() {
  if (!Database::instance() || !m_canLoad) {
    return false;
  }

  if (m_isLoaded) {
    return true;
  }

  const std::filesystem::path path = Database::instance()->basePath / "locales" / m_path;

  try {
    std::ifstream ifs(path);
    for (const auto j = nlohmann::ordered_json::parse(ifs); const auto& entry : j.items()) {
      if (Translation translation = Translation::create(entry.key(), entry.value().get<std::string>()); translation != kInvalidTranslation) {
        // Only add translations if they have valid keys
        m_translations.emplace_back(translation);
      }
    }

    m_isLoaded = true;
    return true;
  } catch (const nlohmann::ordered_json::exception& e) {
    m_isLoaded = false;
    m_canLoad = false;
    APP_ERROR("Unable to parse translation {}", e.what());
    return false;
  }
}

bool TranslationDocument::close(const bool saveFile) {
  if (!m_isLoaded) {
    return true;
  }
  bool ret = true;
  if (saveFile) {
    ret = save();
  }
  m_translations.clear();
  m_isLoaded = false;
  return ret;
}
bool TranslationDocument::save() const {
  if (!Database::instance()) {
    return false;
  }

  if (!isModified()) {
    return true;
  }

  const std::filesystem::path path = Database::instance()->basePath / "locales" / m_path;
  try {
    std::ofstream ofs(path);
    nlohmann::ordered_json j = {};
    for (const auto& kp : m_translations) {
      j[kp.key()] = kp.value();
    }

    ofs << j.dump(/* Database::instance()->minifyOnSave ? 0 : */ 4) << std::endl;
    return true;
  } catch (const nlohmann::ordered_json::exception& e) {
    APP_ERROR("Unable to save translation {}", e.what());
    return false;
  }
}

void TranslationDocument::copyKeys(const TranslationDocument& doc) {
  for (const auto& translation : doc.translations()) {
    m_translations.emplace_back(Translation::create(translation.key(), {}));
  }
}
