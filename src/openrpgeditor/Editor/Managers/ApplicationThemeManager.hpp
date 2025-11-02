#pragma once
#include <filesystem>
#include <memory>
#include <vector>

class ApplicationTheme;
class ApplicationThemeManager {
public:
  ApplicationThemeManager();
  bool initialize(const std::filesystem::path& themeDir, bool dark);
  void serializeAllThemes() const;
  void setCurrentTheme(std::string_view name);
  void applyMainTheme(float uiScale);
  void restoreDefaultTheme(float uiScale);
  void setSystemDark(const bool dark) {
    m_isSystemDark = dark;
    if (m_currentTheme == m_defaultDarkTheme || m_currentTheme == m_defaultLightTheme) {
      m_currentTheme = m_isSystemDark ? m_defaultDarkTheme : m_defaultLightTheme;
    }
  }

  [[nodiscard]] std::shared_ptr<ApplicationTheme> defaultDarkTheme() const { return m_defaultDarkTheme; }
  [[nodiscard]] std::shared_ptr<ApplicationTheme> defaultLightTheme() const { return m_defaultLightTheme; }
  [[nodiscard]] std::shared_ptr<ApplicationTheme> currentTheme() const { return m_currentTheme; }

private:
  std::filesystem::path m_themeDir;
  std::vector<std::shared_ptr<ApplicationTheme>> m_themes;
  std::shared_ptr<ApplicationTheme> m_defaultDarkTheme;
  std::shared_ptr<ApplicationTheme> m_defaultLightTheme;
  std::shared_ptr<ApplicationTheme> m_currentTheme;
  bool m_isSystemDark = true;
};
