#pragma once
#include <algorithm>
#include <filesystem>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <Database/TrackedVector.hpp>

class TranslationDocument {
public:
  enum class State {
    Unknown,
    Untranslated,
    Draft,
    Translated,
  };

  class Translation {
    Translation(const std::string_view key, const std::string_view value)
    : m_key(key)
    , m_value(value) {}

    friend TranslationDocument;

  public:
    Translation() = default;
    static Translation create(const std::string_view key, const std::string_view value) {
      if (key.empty()) {
        return kInvalidTranslation;
      }

      return {key, value};
    }

    [[nodiscard]] std::string key() const { return m_key; }
    [[nodiscard]] bool setKey(const std::string_view key) {
      if (key.empty()) {
        return false;
      }
      if (m_oldKey.empty()) {
        m_oldKey = m_key;
      }
      m_key = key;
      return true;
    }

    [[nodiscard]] bool keyModified() const { return !m_oldKey.empty(); }
    [[nodiscard]] std::string value() const { return m_value; }
    void setValue(const std::string_view value) {
      if (!m_oldValue) {
        m_oldValue = m_value;
      }
      m_value = value;
    }

    [[nodiscard]] bool valueModified() const { return m_oldValue.has_value(); }

    void revert() {
      revertKey();
      revertValue();
    }

    void revertKey() {
      if (!m_oldKey.empty()) {
        m_key = m_oldKey;
        m_oldKey.clear();
      }
    }

    void revertValue() {
      if (m_oldValue) {
        m_value = *m_oldValue;
        m_oldValue.reset();
      }
    }

    bool operator==(const Translation& rhs) const { return m_key == rhs.m_key; }

    bool modified() const { return !m_oldKey.empty() || m_oldValue.has_value(); }

  private:
    std::string m_key;
    std::string m_oldKey;
    std::string m_value;
    std::optional<std::string> m_oldValue;
  };
  /**
   * Describes a translation document, can either be global, or per-map depending on the developer's preference
   * @param path path relative to the project's locale directory
   */
  explicit TranslationDocument(std::filesystem::path path)
  : m_path(std::move(path)) {}

  [[nodiscard]] bool isLoaded() const { return m_isLoaded; }
  [[nodiscard]] bool isModified() const {
    return std::ranges::any_of(m_translations, [](const auto& translation) { return translation.modified(); });
  }

  bool open();
  bool close(bool saveFile);
  bool save() const;

  /**
   *
   * @param key Either the untranslated string or the string key (e.g "{Some_Key}") without the brackets
   * @return
   */
  size_t indexForKey(const std::string_view key) {
    const auto it = std::ranges::find(m_translations, Translation(key, {}));
    if (it == m_translations.end()) {
      return -1;
    }

    return std::distance(m_translations.begin(), it);
  }

  std::string key(const size_t idx) {
    if (idx >= m_translations.size()) {
      return "";
    }

    return m_translations[idx].key();
  }

  bool setKey(const size_t idx, const std::string_view key) {
    if (idx >= m_translations.size()) {
      return false;
    }
    return m_translations[idx].setKey(key);
  }

  bool keyModified(const size_t idx) const {
    if (idx >= m_translations.size()) {
      return false;
    }
    return m_translations[idx].keyModified();
  }

  std::string value(const size_t idx) {
    if (idx >= m_translations.size()) {
      return "";
    }
    return m_translations[idx].value();
  }

  void setValue(const size_t idx, const std::string_view key) {
    if (idx >= m_translations.size()) {
      return;
    }
    m_translations[idx].setValue(key);
  }

  bool valueModified(const size_t idx) const {
    if (idx >= m_translations.size()) {
      return false;
    }
    return m_translations[idx].valueModified();
  }

  [[nodiscard]] size_t translationCount() const { return m_translations.size(); }

  void revert() {
    for (auto& translation : m_translations) {
      translation.revert();
    }
  }

  std::vector<Translation>& translations() { return m_translations; }
  const std::vector<Translation>& translations() const { return m_translations; }

  Translation translate(const std::string_view key) {
    if (key.empty()) {
      return kInvalidTranslation;
    }

    const auto it = std::ranges::find(m_translations, Translation(key, {}));
    if (it == m_translations.end()) {
      return kInvalidTranslation;
    }

    return *it;
  }

  [[nodiscard]] bool translationModified(const size_t idx) const {
    if (idx >= m_translations.size()) {
      return false;
    }

    return m_translations[idx].modified();
  }

  void revertTranslation(const size_t idx) { m_translations[idx].revert(); }

  void revertKey(const size_t idx) { m_translations[idx].revertKey(); }
  void revertValue(const size_t idx) { m_translations[idx].revertValue(); }

  [[nodiscard]] std::string filepath() const { return m_path.generic_string(); }
  [[nodiscard]] std::string filename() const { return m_path.filename().generic_string(); }
  [[nodiscard]] std::string filenameNoExtension() const { return m_path.filename().replace_extension("").generic_string(); }

  void copyKeys(const TranslationDocument& doc);

  static constexpr Translation kInvalidTranslation;

private:
  std::filesystem::path m_path;
  bool m_isLoaded{false};
  bool m_canLoad{true};
  std::vector<Translation> m_translations;
};