#pragma once
#include <algorithm>
#include <filesystem>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <Database/TrackedVector.hpp>
#include <orei18n.hpp>

class TranslationDocument {
public:
  enum class State {
    trENUM("Unknown", Unknown),
    trENUM("Untranslated", Untranslated),
    trENUM("Draft", Draft),
    trENUM("Translated", Translated),
    trENUM("Approved", Approved),
  };

  class Translation {
    Translation(const std::string_view key, const std::string_view value)
    : m_key(key)
    , m_value(value)
    , m_state(State::Unknown) {}

    friend TranslationDocument;

  public:
    constexpr Translation()
    : m_state(State::Unknown) {}

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
      if (m_key == key) {
        return true;
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
      if (m_value == value) {
        return;
      }

      if (!m_oldValue) {
        m_oldValue = m_value;
      }
      m_value = value;
    }

    [[nodiscard]] bool valueModified() const { return m_oldValue.has_value(); }

    [[nodiscard]] State state() const { return m_state; }
    void setState(const State state) {
      if (state == m_state) {
        return;
      }

      if (!m_oldState) {
        m_oldState = m_state;
      }
      m_state = state;
    }

    [[nodiscard]] bool stateModified() const { return m_oldState.has_value(); }

    void revert() {
      revertKey();
      revertValue();
      revertState();
    }

    void revertKey() {
      m_key = m_oldKey.empty() ? m_key : m_oldKey;
      m_oldKey.clear();
    }

    void revertValue() {
      m_value = m_oldValue.value_or(m_value);
      m_oldValue.reset();
    }

    void revertState() {
      m_state = m_oldState.value_or(m_state);
      m_oldState.reset();
    }
    bool operator==(const Translation& rhs) const { return m_key == rhs.m_key; }

    bool modified() const { return !m_oldKey.empty() || m_oldValue.has_value() || m_oldState.has_value(); }

  private:
    std::string m_key;
    std::string m_oldKey;
    std::string m_value;
    std::optional<std::string> m_oldValue;
    State m_state{};
    std::optional<State> m_oldState;
  };
  /**
   * Describes a translation document, can either be global, or per-map depending on the developer's preference
   * @param path path relative to the project's locale directory
   */
  explicit TranslationDocument(std::filesystem::path path, const std::string_view locale)
  : m_path(std::move(path))
  , m_locale(locale) {}

  [[nodiscard]] bool isLoaded() const { return m_isLoaded; }
  [[nodiscard]] bool isModified() const {
    return std::ranges::any_of(m_translations, [](const auto& translation) { return translation.modified(); });
  }

  std::string_view locale() const { return m_locale; }
  void setLocale(const std::string_view locale) { m_locale = locale; }

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

  std::string value(const size_t idx) const {
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

  State state(const size_t idx) const {
    if (idx >= m_translations.size()) {
      return State::Unknown;
    }

    return m_translations[idx].state();
  }

  void setState(const size_t idx, const State state) {
    if (idx >= m_translations.size()) {
      return;
    }
    m_translations[idx].setState(state);
  }

  bool stateModified(const size_t idx) const {
    if (idx >= m_translations.size()) {
      return false;
    }

    return m_translations[idx].stateModified();
  }

  [[nodiscard]] size_t translationCount() const { return m_translations.size(); }

  void revert() {
    for (auto& translation : m_translations) {
      translation.revert();
    }
  }

  std::vector<Translation>& translations() { return m_translations; }
  const std::vector<Translation>& translations() const { return m_translations; }

  Translation* translation(const size_t idx) {
    if (idx == -1 || idx >= m_translations.size()) {
      return nullptr;
    }
    return &m_translations[idx];
  }

  const Translation* translation(const size_t idx) const {
    if (idx == -1 || idx >= m_translations.size()) {
      return nullptr;
    }
    return &m_translations[idx];
  }

  State translationState(const size_t idx) const {
    if (idx == -1 || idx >= m_translations.size()) {
      return State::Unknown;
    }

    return m_translations[idx].state();
  }

  void setTranslationState(const size_t idx, const State state) {
    if (idx == -1 || idx >= m_translations.size()) {
      return;
    }

    m_translations[idx].setState(state);
  }

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
  void revertState(const size_t idx) { m_translations[idx].revertState(); }

  [[nodiscard]] std::string filepath() const { return m_path.generic_string(); }
  [[nodiscard]] std::string filename() const { return m_path.filename().generic_string(); }
  [[nodiscard]] std::string filenameNoExtension() const { return m_path.filename().replace_extension("").generic_string(); }

  void copyKeys(const TranslationDocument& doc);

  static const Translation kInvalidTranslation;

  size_t nextUntranslated(size_t cur) const;
  size_t prevUntranslated(size_t cur) const;

private:
  std::filesystem::path m_path;
  std::string m_locale;
  bool m_isLoaded{false};
  bool m_canLoad{true};
  std::vector<Translation> m_translations;
};