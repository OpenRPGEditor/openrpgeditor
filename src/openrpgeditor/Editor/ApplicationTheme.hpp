#pragma once

#define MAGIC_ENUM_RANGE_MIN (-4096)
#define MAGIC_ENUM_RANGE_MAX (4096)
#include "magic_enum/magic_enum.hpp"

#include <algorithm>
#include <filesystem>
#include <imgui.h>
#include <map>
#include <memory>
#include <string>
class ApplicationTheme {
public:
  struct CommandEditor {
    ImVec4 Header;
    ImVec4 HeaderHovered;
    ImVec4 HeaderActive;
    ImVec4 TableRowBg;
    ImVec4 TableRowBgAlt;
  };

  ApplicationTheme() = default;

  [[nodiscard]] const std::string& name() const { return m_themeName; }
  void setName(const std::string_view name) { m_themeName = name; }
  bool serialize(const std::filesystem::path& path);
  [[nodiscard]] bool isDark() const { return m_isDark; }
  void setDark(const bool isDark) { m_isDark = isDark; }

  static std::shared_ptr<ApplicationTheme> loadTheme(const std::filesystem::path& path);
  static std::shared_ptr<ApplicationTheme> createFromStyle(std::string_view name, ImGuiStyle style = ImGuiStyle(), bool dark = false, bool scaled = false);

  [[nodiscard]] ImGuiStyle mainStyle() const { return m_mainStyle; }
  [[nodiscard]] CommandEditor commandEditorStyle() const { return m_commandEditor; }

private:
  std::string m_themeName;
  bool m_isDark = false;
  ImGuiStyle m_mainStyle;
  CommandEditor m_commandEditor;
};
