#pragma once
#include "Editor/Translator/TranslationDocument.hpp"

#include <filesystem>
#include <ranges>
#include <utility>

class Translator {
  Translator() = default;

public:
  struct Manifest {
    std::string defaultLocale;
    struct Locale {
      std::string description;
      std::optional<std::filesystem::path> directory;
    };

    std::map<std::string, Locale> locales;
    std::vector<std::string> checkFirst;
  };

  Translator(const Translator&) = delete;
  Translator& operator=(const Translator&) = delete;
  Translator(Translator&&) = delete;
  Translator& operator=(Translator&&) = delete;

  void clear() {
    m_documents.clear();
    m_currentMap = -1;
    m_activeMapDocument = nullptr;
    m_locale.clear();
    m_initialized = false;
    m_checkFirst.clear();
    m_checkFirstDocuments.clear();
  }

  static Translator& instance();

  bool initialize();

  [[nodiscard]] const std::vector<std::shared_ptr<TranslationDocument>>& documentsForLocale(const std::string& locale = {}) const;

  void setLocale(std::string_view locale);

  void setCurrentMap(int id);

  [[nodiscard]] std::string translate(std::string_view text) const;

  [[nodiscard]] bool hasTranslations(const std::string& locale = {}) const { return !documentsForLocale(locale).empty(); }

  [[nodiscard]] const std::map<std::string, std::vector<std::shared_ptr<TranslationDocument>>>& documents() const { return m_documents; }
  [[nodiscard]] std::vector<std::string> locales() const { return m_documents | std::ranges::views::keys | std::ranges::to<std::vector<std::string>>(); }
  [[nodiscard]] const std::vector<std::string>& checkFirst() const { return m_checkFirst; }

  [[nodiscard]] std::string localeDescription(const std::string& locale) const {
    if (m_localeInfo.contains(locale)) {
      return m_localeInfo.at(locale).description.empty() ? locale : m_localeInfo.at(locale).description;
    }
    return "";
  }

  [[nodiscard]] std::filesystem::path localePath(const std::string& locale) const {
    auto fallback = std::filesystem::path(".") / locale;
    if (m_localeInfo.contains(locale)) {
      return m_localeInfo.at(locale).directory.value_or(fallback);
    }

    return fallback;
  }

  void createLocale(const std::string_view locale, const std::string_view description, std::optional<std::filesystem::path> directory = {}) {
    if (m_localeInfo.contains(locale.data())) {
      return;
    }

    m_localeInfo[locale.data()] = {description.data(), std::move(directory)};
    m_documents[locale.data()] = {};
  }

  bool createLocaleDocumentFrom(const std::shared_ptr<TranslationDocument>& sourceDoc, const std::string_view targetLocale);
  const std::string& currentLocale() const { return m_locale; }
  

private:
  bool m_initialized{false};
  std::filesystem::path m_localesBasepath;
  std::map<std::string, Manifest::Locale> m_localeInfo;
  std::map<std::string, std::vector<std::shared_ptr<TranslationDocument>>> m_documents; // locale -> translation
  std::vector<std::string> m_checkFirst;

  std::string m_locale;
  int m_currentMap = -1;
  std::shared_ptr<TranslationDocument> m_activeMapDocument;
  std::map<std::string, std::vector<std::shared_ptr<TranslationDocument>>> m_checkFirstDocuments;
};
