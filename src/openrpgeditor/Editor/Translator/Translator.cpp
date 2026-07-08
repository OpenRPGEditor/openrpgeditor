#include "Editor/Translator/Translator.hpp"

#include "Database/Database.hpp"
#include "Editor/Log.hpp"

#include <nlohmann/json.hpp>

#include <regex>

#include <iostream>
#include <regex>
#include <string>
#include <string_view>

#include <iostream>
#include <regex>
#include <string>
#include <string_view>

bool glob_match(const std::string_view str, const std::string_view pattern) {
  std::string regex_str;
  regex_str.reserve(pattern.size() * 2);

  regex_str.push_back('^');

  int brace_depth = 0;
  bool in_brackets = false;

  for (size_t i = 0; i < pattern.size(); ++i) {
    char ch = pattern[i];

    if (in_brackets) {
      if (ch == ']') {
        in_brackets = false;
      }
      regex_str.push_back(ch);
      continue;
    }

    switch (ch) {
    case '*':
      if (i + 1 < pattern.size() && pattern[i + 1] == '*') {
        regex_str.append(".*");
        ++i;
      } else {
        regex_str.append("[^/]*");
      }
      break;

    case '?':
      regex_str.append("[^/]");
      break;

    case '[':
      in_brackets = true;
      regex_str.push_back('[');
      if (i + 1 < pattern.size() && pattern[i + 1] == '!') {
        regex_str.push_back('^');
        ++i;
      }
      break;

    case '{':
      regex_str.push_back('(');
      ++brace_depth;
      break;

    case ',':
      // Only treat comma as an OR operator if inside braces AND not inside brackets
      if (brace_depth > 0) {
        regex_str.push_back('|');
      } else {
        regex_str.push_back(',');
      }
      break;

    case '}':
      if (brace_depth > 0) {
        regex_str.push_back(')');
        --brace_depth;
      } else {
        regex_str.append("\\}");
      }
      break;

    // Escape standard regex engine control tokens
    case '\\':
    case '.':
    case '+':
    case '^':
    case '$':
    case '|':
    case '(':
    case ')':
      regex_str.push_back('\\');
      regex_str.push_back(ch);
      break;

    default:
      regex_str.push_back(ch);
      break;
    }
  }

  regex_str.push_back('$');

  try {
    const std::regex re(regex_str, std::regex_constants::ECMAScript);
    return std::regex_match(std::string(str), re);
  } catch (const std::regex_error&) { return false; }
}

// Find the first matching glob pattern priority index
int get_matching_index(const std::string& str, const std::vector<std::string>& globs) {
  for (int i = 0; i < globs.size(); ++i) {
    if (glob_match(str, globs[i])) {
      return i;
    }
  }
  return globs.size(); // Default fallback position for unmatched entries
}

bool has_extension_insensitive(const std::filesystem::path& file_path, const std::string_view target_ext) {
  std::string ext = file_path.extension().string();
  if (ext.length() != target_ext.length()) {
    return false;
  }
  return std::ranges::equal(ext, target_ext, [](const char a, const char b) { return std::tolower(a) == std::tolower(b); });
}

void to_json(nlohmann::ordered_json& j, const Translator::Manifest::Locale& locale) {
  j["description"] = locale.description;
  if (locale.directory) {
    j["directory"] = locale.directory;
  }
}

template <typename BasicJsonType>
void to_json(BasicJsonType& j, const TranslationDocument::State& e) {
  j = magic_enum::enum_name(e);
}
template <typename BasicJsonType>
void from_json(const BasicJsonType& j, TranslationDocument::State& e) {
  if (!j.is_string()) {
#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)) && !defined(JSON_NOEXCEPTION)
    throw BasicJsonType::type_error::create(302, "type must be string, but is " + std::string(j.type_name()), &j);
#else
    abort();
#endif
  }
  auto name = j.template get<std::string_view>();
  if (auto value = magic_enum::enum_cast<TranslationDocument::State>(name)) {
    e = *value;
  } else {
#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)) && !defined(JSON_NOEXCEPTION)
    throw BasicJsonType::out_of_range::create(403, "unknown enum value: " + std::string(name), &j);
#else
    abort();
#endif
  }
}

void from_json(const nlohmann::ordered_json& j, Translator::Manifest::Locale& locale) {
  // Description is required
  locale.description = j.value("description", locale.description);
  // Directory is optional, if empty Translator will use the locale's identifier as the directory
  locale.directory = j.value("directory", locale.directory);
}
void to_json(nlohmann::ordered_json& j, const Translator::Manifest& manifest) {
  j = {
      {"defaultLocale", manifest.defaultLocale},
      {"locales", manifest.locales},
      {"checkFirst", manifest.checkFirst},
  };
  if (manifest.states) {
    j["states"] = manifest.states;
  }
}

void from_json(const nlohmann::ordered_json& j, Translator::Manifest& manifest) {
  manifest.defaultLocale = j.value("defaultLocale", manifest.defaultLocale);
  manifest.locales = j.value("locales", manifest.locales);
  manifest.checkFirst = j.value("checkFirst", manifest.checkFirst);
  manifest.states = j.value<std::optional<Translator::StateMap>>("states", std::nullopt);
}

bool Translator::save(const bool excludeStates) const {
  if (!m_initialized) {
    return true;
  }

  const auto manifestPath = m_localesBasepath / "locales.json";

  try {
    nlohmann::ordered_json j;
    Manifest manifest;
    manifest.defaultLocale = m_defaultLocale;
    manifest.locales = m_localeInfo;
    manifest.checkFirst = m_checkFirst;
    if (!excludeStates) {
      manifest.states = m_states;
    }
    j = manifest;
    std::ofstream manifestFile(manifestPath);
    manifestFile << j.dump(4);
    return true;
  } catch (const nlohmann::ordered_json::exception& e) {
    APP_ERROR("Exception while saving locale \"{}\"", e.what());
    return false;
  }
}

bool Translator::initialize() {
  if (!Database::instance()) {
    return false;
  }

  m_localesBasepath = Database::instance()->basePath / "locales";

  const auto manifestPath = m_localesBasepath / "locales.json";

  if (!is_regular_file(manifestPath)) {
    return false;
  }

  try {
    std::ifstream manifestFile(manifestPath);
    const auto j = nlohmann::ordered_json::parse(manifestFile);
    const auto [defaultLocale, locales, checkFirst, states] = j.get<Manifest>();
    m_locale = defaultLocale;
    m_defaultLocale = defaultLocale;
    m_localeInfo = locales;
    m_checkFirst = checkFirst;
    m_states = states.value_or({});
    if (m_locale.empty()) {
      if (!m_localeInfo.empty()) {
        APP_WARN("No default locale defined, using first entry, this may not be intended!");
        m_locale = (m_localeInfo | std::views::keys).front();
        APP_WARN("Defaulted to locale \"{}\"", m_locale);
      } else {
        APP_ERROR("No default locale or locale list defined, invalid locales manifest!");
        return false;
      }
    }

    for (const auto& [locale, info] : m_localeInfo) {
      const auto localePath = m_localesBasepath / info.directory.value_or(locale);
      if (!is_directory(localePath)) {
        APP_WARN("Locale \"{}\" does not have a valid path defined!", locale);
        continue;
      }
      for (const auto& entry : std::filesystem::recursive_directory_iterator(localePath)) {
        if (!is_regular_file(entry) || !has_extension_insensitive(entry.path(), ".json")) {
          continue;
        }

        const auto& document = m_documents[locale].emplace_back(std::make_shared<TranslationDocument>(std::filesystem::relative(entry.path(), m_localesBasepath)));
        APP_DEBUG("Added document {} for locale \"{}\"", document->filepath(), locale);
        for (const auto& cf : m_checkFirst) {
          if (glob_match(document->filepath(), cf)) {
            APP_DEBUG("Adding document \"{}\" to priority list using glob \"{}\"", document->filepath(), cf);
            m_checkFirstDocuments[locale].emplace_back(document);
            break;
          }
        }
      }
      std::ranges::sort(m_checkFirstDocuments[m_locale], [this](const std::shared_ptr<TranslationDocument>& a, const std::shared_ptr<TranslationDocument>& b) {
        const auto name_a = a->filepath();
        const auto index_a = get_matching_index(name_a, m_checkFirst);
        const auto name_b = b->filepath();
        const auto index_b = get_matching_index(name_b, m_checkFirst);

        if (index_a != index_b) {
          return index_a < index_b;
        }

        return name_a < name_b;
      });
    }
    m_initialized = true;
  } catch (const nlohmann::ordered_json::exception& e) {
    APP_ERROR("Unable to load manifest {}", e.what());
    return false;
  }

  return true;
}
void Translator::setLocale(const std::string_view locale) {
  if (m_locale == locale) {
    return;
  }

  // Only allow valid locales
  if (m_localeInfo.contains(locale.data())) {
    return;
  }

  for (const auto& document : m_documents[m_locale]) {
    document->close(false);
  }

  m_locale = locale;
}

const std::vector<std::shared_ptr<TranslationDocument>>& Translator::documentsForLocale(const std::string& locale) const {
  if (!locale.empty() && m_documents.contains(locale)) {
    return m_documents.at(locale);
  }
  if (!locale.empty()) {
    return {};
  }

  if (!m_documents.contains(m_locale)) {
    return {};
  }
  return m_documents.at(m_locale);
}

void Translator::setCurrentMap(const int id) {
  if (m_currentMap == id && m_activeMapDocument) {
    return;
  }
  m_currentMap = id;

  const std::string map = std::format("Map{:03}", m_currentMap);
  const std::string mapBugged = std::format("Map{:04}", m_currentMap);
  const auto documents = m_documents[m_locale];
  const auto it = std::ranges::find_if(documents, [map, mapBugged](const auto& doc) { return doc->filenameNoExtension() == map || doc->filenameNoExtension() == mapBugged; });
  if (it == documents.end()) {
    m_activeMapDocument = nullptr;
    return;
  }
  m_activeMapDocument = *it;
}

std::string Translator::translate(const std::string_view text) const {
  std::string key{text};
  if (key.starts_with('{') && key.ends_with('}')) {
    // if we're a designated key, chop off the brackets
    key.erase(0, 1);
    key.erase(key.length() - 1);
  }

  if (!m_initialized) {
    return key;
  }

  // First check any documents declared in "checkFirst"
  if (!m_checkFirstDocuments.empty()) {
    for (const auto& document : m_checkFirstDocuments.at(m_locale)) {
      if (!document->isLoaded()) {
        document->open();
      } else if (const auto translation = document->translate(key); translation != TranslationDocument::kInvalidTranslation) {
        // found it
        APP_DEBUG("Translated string\n\"{}\"\nusing \"{}\" (Check First)", key, document->filenameNoExtension());
        auto str = translation.value();
        if (str.empty()) {
          APP_WARN("Translated string was empty! Returning key");
          return key;
        }
        return str;
      }
    }
  }
  // Now check the active map
  if (m_activeMapDocument) {
    if (!m_activeMapDocument->isLoaded()) {
      m_activeMapDocument->open();
    } else if (const auto translation = m_activeMapDocument->translate(key); translation != TranslationDocument::kInvalidTranslation) {
      // Found it
      APP_DEBUG("Translated string\n\"{}\"\nusing \"{}\" (Active Map)", key, m_activeMapDocument->filenameNoExtension());
      auto str = translation.value();
      if (str.empty()) {
        APP_WARN("Translated string was empty! Returning key");
        return key;
      }
      return str;
    }
  }

  // We couldn't find a map specific string, lets check the rest of the active locale
  for (const auto& document : m_documents.at(m_locale)) {
    if (!document->isLoaded()) {
      document->open();
    } else if (const auto translation = document->translate(key); translation != TranslationDocument::kInvalidTranslation) {
      // found it
      APP_DEBUG("Translated string\n\"{}\"\nusing \"{}\" (Fallback)", key, document->filenameNoExtension());
      auto str = translation.value();
      if (str.empty()) {
        APP_WARN("Translated string was empty! Returning key");
        return key;
      }
      return str;
    }
  }

  // Nope, return the key unmodified
  return key;
}

bool Translator::createLocaleDocumentFrom(const std::shared_ptr<TranslationDocument>& sourceDoc, const std::string_view targetLocale) {
  if (!sourceDoc) {
    return false;
  }
  if (!m_localeInfo.contains(targetLocale.data())) {
    // Need to call createLocale first!
    return false;
  }

  const auto [_, directory] = m_localeInfo[targetLocale.data()];
  const auto targetLocaleDir = directory.value_or(std::filesystem::path(".") / targetLocale);
  const auto& destDoc = m_documents[targetLocale.data()].emplace_back(std::make_shared<TranslationDocument>(targetLocaleDir / sourceDoc->filepath()));

  destDoc->copyKeys(*sourceDoc);
  return true;
}

size_t Translator::nextUntranslated(const std::string_view locale, const std::string_view filename, size_t cur) {
  const std::string localeStr(locale);
  const std::string filenameStr(filename);

  if (!m_documents.contains(localeStr) || !m_states[localeStr].contains(filenameStr)) {
    return -1;
  }

  const auto& states = m_states[localeStr][filenameStr];
  if (states.empty()) {
    return -1;
  }

  if (cur < 0 || cur >= states.size()) {
    cur = 0;
  }

  size_t elements_checked = 0;
  const size_t size = states.size();

  while (elements_checked < size) {
    if (states[cur] != TranslationDocument::State::Translated) {
      return cur; // Found an untranslated item!
    }

    cur = (cur + 1) % size;
    elements_checked++;
  }

  return -1; // Everything is translated
}

size_t Translator::prevUntranslated(const std::string_view locale, const std::string_view filename, size_t cur) {
  const std::string localeStr(locale);
  const std::string filenameStr(filename);

  if (!m_documents.contains(localeStr) || !m_states[localeStr].contains(filenameStr)) {
    return -1;
  }

  const auto& states = m_states[localeStr][filenameStr];
  if (states.empty()) {
    return -1;
  }

  if (cur < 0 || cur >= states.size()) {
    cur = states.size() - 1;
  }

  size_t elementsChecked = 0;
  const size_t size = states.size();

  while (elementsChecked < size) {
    if (states[cur] != TranslationDocument::State::Translated) {
      return cur; // Found an untranslated item moving backward!
    }

    cur = (cur - 1 + size) % size;
    elementsChecked++;
  }

  return -1; // Everything is translated
}

bool Translator::documentHasStates(const std::string_view locale, const std::string_view filename) {
  const std::string localeStr(locale);
  const std::string filenameStr(filename);
  if (!m_documents.contains(localeStr) || !m_states[localeStr].contains(filenameStr)) {
    return false;
  }

  return m_states[localeStr][filenameStr].empty();
}

size_t Translator::documentStateCount(const std::string_view locale, const std::string_view filename) {
  if (!documentHasStates(locale, filename)) {
    return false;
  }

  const std::string localeStr(locale);
  const std::string filenameStr(filename);
  return m_states[localeStr][filenameStr].size();
}

void Translator::initializeDocumentStates(std::string_view locale, std::string_view filename, size_t count) {
  const std::string localeStr(locale);
  const std::string filenameStr(filename);
  auto& states = m_states[localeStr][filenameStr];
  while (states.size() < count) {
    states.emplace_back(TranslationDocument::State::Unknown);
  }
}

Translator& Translator::instance() {
  static Translator _instance;
  return _instance;
}

std::set<std::string> Translator::gatherStringsForMap(const Map* map, const int mapId) {
  if (map == nullptr) {
    return {};
  }

  // TODO(pstephens): Implement stringValues for all data types and migrate this code to Map,Event,Page etc
  std::set<std::string> result;
  const auto& mapEvents = map->events();
  for (const auto& event : mapEvents) {
    if (!event || event->id() == 0) {
      continue;
    }

    for (int pageIdx = 0; const auto& page : event->pages()) {
      for (const auto& command : page.list()) {
        if (!command->hasStringValues()) {
          continue;
        }
        const auto values = command->stringValues();
        for (const auto& value : values) {
          result.insert(value);
          //result.insert(std::format("Map{:03}-{}-{}-{}:\n{}", mapId, event->name(), event->id(), pageIdx, value));
        }
      }
      pageIdx++;
    }
  }

  return result;
}
