#include "Core/ApplicationThemeManager.hpp"

#include "Core/ApplicationTheme.hpp"
#include "Core/Log.hpp"

using namespace std::string_view_literals;

static constexpr auto skDefaultDarkThemeName = "Default Dark"sv;
static constexpr auto skDefaultLightThemeName = "Default Light"sv;

static bool compareInsensitive(const std::string& a, const std::string& b) {
  if (a.length() != b.length()) {
    return false;
  }
  return std::equal(a.begin(), a.end(), b.begin(), [](char char_a, char char_b) { return std::tolower(static_cast<unsigned char>(char_a)) == std::tolower(static_cast<unsigned char>(char_b)); });
}

void AssignDefaultGeometrySettings(ImGuiStyle& style) {
  // General window settings
  style.WindowPadding = ImVec2(6, 6);
  style.WindowRounding = 3.f;
  style.FrameRounding = 3.f;
  style.ScrollbarRounding = 3.f;
  style.GrabRounding = 3.f;
  style.TabRounding = 3.f;
  style.WindowBorderSize = 1.f;
  style.FrameBorderSize = 1.0f;
  style.PopupBorderSize = 1.0f;
  style.PopupRounding = 3.f;

  // Additional styles
  style.FramePadding = ImVec2(4.0f, 3.f);
  style.ItemSpacing = ImVec2(4.0f, 3.f);
  style.IndentSpacing = 10.0f;
  style.ScrollbarSize = 12.f;
  style.CurveTessellationTol = 0.1f;
  style.CircleTessellationMaxError = 0.1f;
}

ImGuiStyle CreateLightTheme() {
  ImGuiStyle style;
  AssignDefaultGeometrySettings(style);
  ImVec4* colors = style.Colors;

  // Setting the colors (Light version)
  colors[ImGuiCol_Text] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f); // Light background
  colors[ImGuiCol_ChildBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
  colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Light frame background
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);

  // Accent colors with a soft pastel gray-green
  colors[ImGuiCol_CheckMark] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Soft gray-green for check marks
  colors[ImGuiCol_SliderGrab] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.70f, 0.60f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Light button background
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_Header] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
  colors[ImGuiCol_HeaderActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Accent color for resize grips
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.60f, 0.70f, 0.60f, 1.00f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.65f, 0.75f, 0.65f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Tabs background
  colors[ImGuiCol_TabHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
  colors[ImGuiCol_TabActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
  colors[ImGuiCol_DockingPreview] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Docking preview in gray-green
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  return style;
}

ImGuiStyle CreateDarkTheme() {
  ImGuiStyle style;
  AssignDefaultGeometrySettings(style);
  ImVec4* colors = style.Colors;

  // Setting the colors
  colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
  colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.f);
  colors[ImGuiCol_Border] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
  colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);

  // Accent colors changed to darker olive-green/grey shades
  colors[ImGuiCol_CheckMark] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);        // Dark gray for check marks
  colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f);       // Dark gray for sliders
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f); // Slightly lighter gray when active
  colors[ImGuiCol_Button] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);           // Button background (dark gray)
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);    // Button hover state
  colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);     // Button active state
  colors[ImGuiCol_Header] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);           // Dark gray for menu headers
  colors[ImGuiCol_HeaderHovered] = ImVec4(0.35, 0.35, 0.35, 1.00f);       // Slightly darker on hover
  colors[ImGuiCol_HeaderActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);     // Darker gray when active
  colors[ImGuiCol_Separator] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);        // Separators in dark gray
  colors[ImGuiCol_SeparatorHovered] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
  colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); // Resize grips in dark gray
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.55f, 0.55f, 0.55f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);        // Tabs background
  colors[ImGuiCol_TabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f); // Darker gray on hover
  colors[ImGuiCol_TabActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
  colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
  colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
  colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
  colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
  colors[ImGuiCol_DockingPreview] = ImVec4(0.45f, 0.45f, 0.45f, 1.00f); // Docking preview in gray
  colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f); // Empty dock background

  return style;
}
ApplicationThemeManager::ApplicationThemeManager()
: m_defaultDarkTheme(ApplicationTheme::createFromStyle(skDefaultDarkThemeName, CreateDarkTheme(), true))
, m_defaultLightTheme(ApplicationTheme::createFromStyle(skDefaultLightThemeName, CreateLightTheme()))
, m_currentTheme(m_defaultLightTheme) {}

bool ApplicationThemeManager::initialize(const std::filesystem::path& themeDir, const bool dark) {
  m_themeDir = themeDir;
  m_currentTheme = dark ? m_defaultDarkTheme : m_defaultLightTheme;
  m_isSystemDark = dark;

  if (!exists(m_themeDir)) {
    return false;
  }

  for (const auto& file : std::filesystem::directory_iterator(m_themeDir)) {
    if (!file.is_regular_file() || file.path().extension() == ".json") {
      continue;
    }
    const auto theme = ApplicationTheme::loadTheme(file);
    if (compareInsensitive(theme->name(), skDefaultDarkThemeName.data()) || compareInsensitive(theme->name(), skDefaultLightThemeName.data())) {
      continue;
    }
    if (std::ranges::find_if(m_themes, [theme](const auto& v) { return compareInsensitive(theme->name(), v->name()); }) == m_themes.end()) {
      m_themes.emplace_back(theme);
    } else {
      APP_WARN("Duplicate theme {}", theme->name());
    }
  }

  return false;
}

void ApplicationThemeManager::serializeAllThemes() const {
  if (!exists(m_themeDir)) {
    return;
  }

  for (const auto& theme : m_themes) {
    theme->serialize(m_themeDir / std::format("{}.json", theme->name()));
  }
}

void ApplicationThemeManager::setCurrentTheme(const std::string_view name) {
  for (const auto& theme : m_themes) {
    if (compareInsensitive(theme->name(), name.data())) {
      m_currentTheme = theme;
      return;
    }
  }
}

void ApplicationThemeManager::applyMainTheme(const float uiScale) {
  if (!m_currentTheme) {
    m_currentTheme = m_isSystemDark ? m_defaultDarkTheme : m_defaultLightTheme;
  }
  ImGui::GetStyle() = m_currentTheme->mainStyle();
  ImGui::GetStyle().ScaleAllSizes(uiScale);
}

void ApplicationThemeManager::restoreDefaultTheme(const float uiScale) {
  m_currentTheme = m_isSystemDark ? m_defaultDarkTheme : m_defaultLightTheme;
  ImGui::GetStyle() = m_currentTheme->mainStyle();
  ImGui::GetStyle().ScaleAllSizes(uiScale);
}
