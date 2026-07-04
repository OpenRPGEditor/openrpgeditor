#include "Editor/Translator/Translator.hpp"

#include "Database/Database.hpp"
#include "Editor/Log.hpp"

#include <regex>

bool glob_match(const std::string_view pattern, const std::string_view str) {
  std::string regex_str = "^";
  for (const char c : pattern) {
    switch (c) {
    case '*':
      regex_str += ".*";
      break;
    case '?':
      regex_str += ".";
      break;
    case '.':
      regex_str += "\\.";
      break;
    case '\\':
      regex_str += "\\\\";
      break;
    case '^':
      regex_str += "\\^";
      break;
    case '$':
      regex_str += "\\$";
      break;
    case '+':
      regex_str += "\\+";
      break;
    case '|':
      regex_str += "\\|";
      break;
    case '(':
      regex_str += "\\(";
      break;
    case ')':
      regex_str += "\\)";
      break;
    case '[':
      regex_str += "\\[";
      break;
    case ']':
      regex_str += "\\]";
      break;
    case '{':
      regex_str += "\\{";
      break;
    case '}':
      regex_str += "\\}";
      break;
    default:
      regex_str += c;
      break;
    }
  }
  regex_str += "$";

  // Compile and match the regex expression
  return std::regex_match(std::string(str), std::regex(regex_str));
}

// Find the first matching glob pattern priority index
int get_matching_index(const std::string& str, const std::vector<std::string>& globs) {
  for (int i = 0; i < globs.size(); ++i) {
    if (glob_match(globs[i], str)) {
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
}

void from_json(const nlohmann::ordered_json& j, Translator::Manifest& manifest) {
  manifest.defaultLocale = j.value("defaultLocale", manifest.defaultLocale);
  manifest.locales = j.value("locales", manifest.locales);
  manifest.checkFirst = j.value("checkFirst", manifest.checkFirst);
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
    const auto manifest = j.get<Manifest>();
    m_locale = manifest.defaultLocale;
    if (m_locale.empty()) {
      if (!manifest.locales.empty()) {
        APP_WARN("No default locale defined, using first entry, this may not be intended!");
        m_locale = (manifest.locales | std::views::keys).front();
        APP_WARN("Defaulted to locale \"{}\"", m_locale);
      } else {
        APP_ERROR("No default locale or locale list defined, invalid locales manifest!");
        return false;
      }
    }

    for (const auto& [locale, info] : manifest.locales) {
      const auto localePath = m_localesBasepath / info.directory.value_or(locale);
      if (!is_directory(localePath)) {
        APP_WARN("Locale \"{}\" does not have a valid path defined!", locale);
        continue;
      }
      for (const auto& entry : std::filesystem::recursive_directory_iterator(localePath)) {
        if (!is_regular_file(entry) || !has_extension_insensitive(entry.path(), ".json")) {
          continue;
        }

        const auto& [_, document] = m_documents[locale].emplace_back(info, std::make_shared<TranslationDocument>(std::filesystem::relative(entry.path(), m_localesBasepath)));
        APP_DEBUG("Added document {} for locale \"{}\"", document->filename(), locale);
        for (const auto& checkFirst : m_checkFirst) {
          if (glob_match(document->filename(), checkFirst)) {
            m_checkFirstDocuments[locale].emplace_back(document);
            break;
          }
        }
      }
      std::ranges::sort(m_checkFirstDocuments[m_locale], [this](const std::shared_ptr<TranslationDocument>& a, const std::shared_ptr<TranslationDocument>& b) {
        const auto name_a = a->filename();
        const auto index_a = get_matching_index(name_a, m_checkFirst);
        const auto name_b = b->filename();
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
  if (std::ranges::find(m_locales, locale) == m_locales.end()) {
    return;
  }

  for (const auto& document : m_documents[m_locale] | std::views::values) {
    document->close(false);
  }

  m_locale = locale;
}

std::vector<std::shared_ptr<TranslationDocument>> Translator::documents(const std::string& locale) {
  if (locale.empty() && m_documents.contains(m_locale)) {
    return m_documents[m_locale] | std::views::values | std::ranges::to<std::vector<std::shared_ptr<TranslationDocument>>>();
  }
  if (!locale.empty()) {
    return {};
  }

  if (!m_documents.contains(locale)) {
    return {};
  }
  return m_documents[locale] | std::views::values | std::ranges::to<std::vector<std::shared_ptr<TranslationDocument>>>();
}

void Translator::setCurrentMap(const int id) {
  if (m_currentMap == id && m_activeMapDocument) {
    return;
  }
  m_currentMap = id;

  const std::string map = std::format("Map{:03}", m_currentMap);
  const std::string mapBugged = std::format("Map{:04}", m_currentMap);
  const auto documents = m_documents[m_locale];
  const auto it = std::ranges::find_if(documents, [map, mapBugged](const auto& doc) { return doc.second->filename() == map || doc.second->filename() == mapBugged; });
  if (it == documents.end()) {
    m_activeMapDocument = nullptr;
    return;
  }
  m_activeMapDocument = it->second;
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
        APP_DEBUG("Translated string\n\"{}\"\nusing \"{}\" (Check First)", key, document->filename());
        return translation.value();
      }
    }
  }
  // Now check the active map
  if (m_activeMapDocument) {
    if (!m_activeMapDocument->isLoaded()) {
      m_activeMapDocument->open();
    } else if (const auto translation = m_activeMapDocument->translate(key); translation != TranslationDocument::kInvalidTranslation) {
      // Found it
      APP_DEBUG("Translated string\n\"{}\"\nusing \"{}\" (Active Map)", key, m_activeMapDocument->filename());
      return translation.value();
    }
  }

  // We couldn't find a map specific string, lets check the rest of the active locale
  for (const auto& document : m_documents.at(m_locale) | std::views::values) {
    if (!document->isLoaded()) {
      document->open();
    } else if (const auto translation = document->translate(key); translation != TranslationDocument::kInvalidTranslation) {
      // found it
      APP_DEBUG("Translated string\n\"{}\"\nusing \"{}\" (Fallback)", key, document->filename());
      return translation.value();
    }
  }

  // Nope, return the key unmodified
  return key;
}

Translator& Translator::instance() {
  static Translator _instance;
  return _instance;
}
