#include "Editor/Translator/TranslationDocument.hpp"

#include "Editor/Log.hpp"
#include "Editor/Translator/Translator.hpp"

#include <Database/Database.hpp>
#include <ranges>

const TranslationDocument::Translation TranslationDocument::kInvalidTranslation;

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
        // Retrieve this translation's state, set directly to avoid tripping the modification flag
        m_translations.back().m_state = Translator::instance().translationState(m_locale, filenameNoExtension(), m_translations.size() - 1);
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

size_t TranslationDocument::nextUntranslated(size_t cur) const {
  if (m_translations.empty()) {
    return -1;
  }

  if (cur < 0 || cur >= translationCount()) {
    cur = 0;
  } else {
    ++cur;
  }

  size_t elementsChecked = 0;
  const size_t size = translationCount();

  while (elementsChecked < size) {
    if (m_translations[cur].state() != State::Translated && m_translations[cur].state() != State::Approved) {
      return cur; // Found an untranslated item!
    }

    cur = (cur + 1) % size;
    elementsChecked++;
  }

  return -1; // Everything is translated
}

size_t TranslationDocument::prevUntranslated(size_t cur) const {
  if (m_translations.empty()) {
    return -1;
  }

  if (cur < 0 || cur >= translationCount()) {
    cur = translationCount() - 1;
  } else {
    --cur;
  }

  size_t elementsChecked = 0;
  const size_t size = translationCount();

  while (elementsChecked < size) {
    if (m_translations[cur].state() != State::Translated && m_translations[cur].state() != State::Approved) {
      return cur; // Found an untranslated item moving backward!
    }

    cur = (cur - 1 + size) % size;
    elementsChecked++;
  }

  return -1; // Everything is translated
}