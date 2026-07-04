#pragma once
#include "Editor/Translator/TranslationDocument.hpp"

#include <filesystem>
#include <ranges>

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
    m_locales.clear();
    m_initialized = false;
    m_checkFirst.clear();
    m_checkFirstDocuments.clear();
  }

  static Translator& instance();

  bool initialize();

  std::vector<std::shared_ptr<TranslationDocument>> documents(const std::string& locale = {});

  void setLocale(std::string_view locale);

  void setCurrentMap(int id);

  [[nodiscard]] std::string translate(std::string_view text) const;

  [[nodiscard]] bool hasTranslations() const { return !m_documents.empty(); }

  [[nodiscard]] const std::map<std::string, std::vector<std::pair<Manifest::Locale, std::shared_ptr<TranslationDocument>>>>& localeDocuments() const { return m_documents; }
  [[nodiscard]] std::vector<std::string> locales() const { return m_documents | std::ranges::views::keys | std::ranges::to<std::vector<std::string>>(); }
  [[nodiscard]] const std::vector<std::string>& checkFirst() const { return m_checkFirst; }

private:
  bool m_initialized{false};
  std::filesystem::path m_localesBasepath;
  std::vector<std::string> m_locales;
  std::map<std::string, std::vector<std::pair<Manifest::Locale, std::shared_ptr<TranslationDocument>>>> m_documents; // locale -> translation
  std::vector<std::string> m_checkFirst;

  std::string m_locale;
  int m_currentMap = -1;
  std::shared_ptr<TranslationDocument> m_activeMapDocument;
  std::map<std::string, std::vector<std::shared_ptr<TranslationDocument>>> m_checkFirstDocuments;
};
