#include "Core/ApplicationTheme.hpp"

#include "nlohmann/json.hpp"
#include "Settings.hpp"
#include <fstream>

void to_json(nlohmann::ordered_json& j, const ImVec2& v) {
  j["x"] = v.x;
  j["y"] = v.y;
}

void from_json(const nlohmann::ordered_json& j, ImVec2& v) {
  v.x = j.value("x", v.x);
  v.y = j.value("y", v.y);
}

void to_json(nlohmann::ordered_json& j, const ImVec4& v) {
  j["x"] = v.x;
  j["y"] = v.y;
  j["z"] = v.z;
  j["w"] = v.w;
}

void from_json(const nlohmann::ordered_json& j, ImVec4& v) {
  v.x = j.value("x", v.x);
  v.y = j.value("y", v.y);
  v.z = j.value("z", v.z);
  v.w = j.value("w", v.w);
}

void to_json(nlohmann::ordered_json& j, const ApplicationTheme::CommandEditor& v) {
  j["Header"] = v.Header;
  j["HeaderHovered"] = v.HeaderHovered;
  j["HeaderActive"] = v.HeaderActive;
  j["TableRowBg"] = v.TableRowBg;
  j["TableRowBgAlt"] = v.TableRowBgAlt;
}

void from_json(const nlohmann::ordered_json& j, ApplicationTheme::CommandEditor& v) {
  v.Header = j.value("Header", v.Header);
  v.HeaderHovered = j.value("HeaderHovered", v.HeaderHovered);
  v.HeaderActive = j.value("HeaderActive", v.HeaderActive);
  v.TableRowBg = j.value("TableRowBg", v.TableRowBg);
  v.TableRowBgAlt = j.value("TableRowBgAlt", v.TableRowBgAlt);
}

void to_json(nlohmann::ordered_json& j, const ImGuiStyle& style) {
  auto& colors = j["Colors"];
  for (int color = ImGuiCol_Text; color < ImGuiCol_COUNT; color++) {
    const auto colorName = magic_enum::enum_name(static_cast<ImGuiCol_>(color));
    colors[colorName.substr(colorName.find_first_of('_') + 1)] = style.Colors[color];
  }
  j["Alpha"] = style.Alpha;
  j["DisabledAlpha"] = style.DisabledAlpha;
  j["WindowPadding"] = style.WindowPadding;
  j["WindowRounding"] = style.WindowRounding;
  j["WindowBorderSize"] = style.WindowBorderSize;
  j["WindowBorderHoverPadding"] = style.WindowBorderHoverPadding;
  j["WindowMinSize"] = style.WindowMinSize;
  j["WindowTitleAlign"] = style.WindowTitleAlign;
  const auto menuButtonPositionPos = magic_enum::enum_name(style.WindowMenuButtonPosition);
  j["WindowMenuButtonPosition"] = menuButtonPositionPos.substr(menuButtonPositionPos.find_first_of('_') + 1);
  j["ChildRounding"] = style.ChildRounding;
  j["ChildBorderSize"] = style.ChildBorderSize;
  j["PopupRounding"] = style.PopupRounding;
  j["PopupBorderSize"] = style.PopupBorderSize;
  j["FramePadding"] = style.FramePadding;
  j["FrameRounding"] = style.FrameRounding;
  j["FrameBorderSize"] = style.FrameBorderSize;
  j["ItemSpacing"] = style.ItemSpacing;
  j["ItemInnerSpacing"] = style.ItemInnerSpacing;
  j["CellPadding"] = style.CellPadding;
  j["TouchExtraPadding"] = style.TouchExtraPadding;
  j["IndentSpacing"] = style.IndentSpacing;
  j["ColumnsMinSpacing"] = style.ColumnsMinSpacing;
  j["ScrollbarSize"] = style.ScrollbarSize;
  j["ScrollbarRounding"] = style.ScrollbarRounding;
  j["GrabMinSize"] = style.GrabMinSize;
  j["GrabRounding"] = style.GrabRounding;
  j["LayoutAlign"] = style.LayoutAlign;
  j["LogSliderDeadzone"] = style.LogSliderDeadzone;
  j["ImageBorderSize"] = style.ImageBorderSize;
  j["TabRounding"] = style.TabRounding;
  j["TabBorderSize"] = style.TabBorderSize;
  j["TabCloseButtonMinWidthSelected"] = style.TabCloseButtonMinWidthSelected;
  j["TabCloseButtonMinWidthUnselected"] = style.TabCloseButtonMinWidthUnselected;
  j["TabBarBorderSize"] = style.TabBarBorderSize;
  j["TabRounding"] = style.TabRounding;
  j["TableAngledHeadersAngle"] = style.TableAngledHeadersAngle;
  j["TableAngledHeadersTextAlign"] = style.TableAngledHeadersTextAlign;
  j["TreeLinesFlags"] = style.TreeLinesFlags;
  j["TreeLinesRounding"] = style.TreeLinesRounding;
  const auto colorButtonPositionPos = magic_enum::enum_name(style.ColorButtonPosition);
  j["ColorButtonPosition"] = colorButtonPositionPos.substr(colorButtonPositionPos.find_first_of('_') + 1);
  j["ButtonTextAlign"] = style.ButtonTextAlign;
  j["SelectableTextAlign"] = style.SelectableTextAlign;
  j["SeparatorTextBorderSize"] = style.SeparatorTextBorderSize;
  j["SeparatorTextAlign"] = style.SeparatorTextAlign;
  j["SeparatorTextPadding"] = style.SeparatorTextPadding;
  j["DisplayWindowPadding"] = style.DisplayWindowPadding;
  j["DisplaySafeAreaPadding"] = style.DisplaySafeAreaPadding;
  j["DockingSeparatorSize"] = style.DockingSeparatorSize;
  j["CurveTessellationTol"] = style.CurveTessellationTol;
  j["CircleTessellationMaxError"] = style.CircleTessellationMaxError;
}

void from_json(const nlohmann::ordered_json& j, ImGuiStyle& style) {
  if (j.contains("Colors") && j["Colors"].is_object()) {
    const auto colors = j["Colors"];
    for (const auto& [colorName, colorValue] : colors.items()) {
      const auto color = magic_enum::enum_cast<ImGuiCol_>(std::format("ImGuiCol_{}", colorName), magic_enum::case_insensitive).value_or(ImGuiCol_COUNT);
      if (color == ImGuiCol_COUNT) {
        continue;
      }
      style.Colors[color] = colorValue.get<ImVec4>();
    }
  }

  style.Alpha = j.value("Alpha", style.Alpha);
  style.DisabledAlpha = j.value("DisabledAlpha", style.DisabledAlpha);
  style.WindowPadding = j.value("WindowPadding", style.WindowPadding);
  style.WindowRounding = j.value("WindowRounding", style.WindowRounding);
  style.WindowBorderSize = j.value("WindowBorderSize", style.WindowBorderSize);
  style.WindowBorderHoverPadding = j.value("WindowBorderHoverPadding", style.WindowBorderHoverPadding);
  style.WindowMinSize = j.value("WindowMinSize", style.WindowMinSize);
  style.WindowTitleAlign = j.value("WindowTitleAlign", style.WindowTitleAlign);
  const auto menuPosition = std::format("ImGuiDir_{}", j.value("WindowMenuButtonPosition", ""));
  style.WindowMenuButtonPosition = magic_enum::enum_cast<ImGuiDir>(menuPosition, magic_enum::case_insensitive).value_or(style.WindowMenuButtonPosition);
  style.ChildRounding = j.value("ChildRounding", style.ChildRounding);
  style.ChildBorderSize = j.value("ChildBorderSize", style.ChildBorderSize);
  style.PopupRounding = j.value("PopupRounding", style.PopupRounding);
  style.PopupBorderSize = j.value("PopupBorderSize", style.PopupBorderSize);
  style.FramePadding = j.value("FramePadding", style.FramePadding);
  style.FrameRounding = j.value("FrameRounding", style.FrameRounding);
  style.FrameBorderSize = j.value("FrameBorderSize", style.FrameBorderSize);
  style.ItemSpacing = j.value("ItemSpacing", style.ItemSpacing);
  style.ItemInnerSpacing = j.value("ItemInnerSpacing", style.ItemInnerSpacing);
  style.CellPadding = j.value("CellPadding", style.CellPadding);
  style.TouchExtraPadding = j.value("TouchExtraPadding", style.TouchExtraPadding);
  style.IndentSpacing = j.value("IndentSpacing", style.IndentSpacing);
  style.ColumnsMinSpacing = j.value("ColumnsMinSpacing", style.ColumnsMinSpacing);
  style.ScrollbarSize = j.value("ScrollbarSize", style.ScrollbarSize);
  style.ScrollbarRounding = j.value("ScrollbarRounding", style.ScrollbarRounding);
  style.GrabMinSize = j.value("GrabMinSize", style.GrabMinSize);
  style.GrabRounding = j.value("GrabRounding", style.GrabRounding);
  style.LayoutAlign = j.value("LayoutAlign", style.LayoutAlign);
  style.LogSliderDeadzone = j.value("LogSliderDeadzone", style.LogSliderDeadzone);
  style.ImageBorderSize = j.value("ImageBorderSize", style.ImageBorderSize);
  style.TabRounding = j.value("TabRounding", style.TabRounding);
  style.TabBorderSize = j.value("TabBorderSize", style.TabBorderSize);
  style.TabCloseButtonMinWidthSelected = j.value("TabCloseButtonMinWidthSelected", style.TabCloseButtonMinWidthSelected);
  style.TabCloseButtonMinWidthUnselected = j.value("TabCloseButtonMinWidthUnselected", style.TabCloseButtonMinWidthUnselected);
  style.TabBarBorderSize = j.value("TabBarBorderSize", style.TabBarBorderSize);
  style.TabBarOverlineSize = j.value("TabBarOverlineSize", style.TabBarOverlineSize);
  style.TableAngledHeadersAngle = j.value("TableAngledHeadersAngle", style.TableAngledHeadersAngle);
  style.TableAngledHeadersTextAlign = j.value("TableAngledHeadersTextAlign", style.TableAngledHeadersTextAlign);
  style.TreeLinesFlags = j.value("TreeLinesFlags", style.TreeLinesFlags);
  style.TreeLinesSize = j.value("TreeLinesSize", style.TreeLinesSize);
  style.TreeLinesRounding = j.value("TreeLinesRounding", style.TreeLinesRounding);
  const auto colorButtonPosition = std::format("ImGuiDir_{}", j.value("ColorButtonPosition", ""));
  style.ColorButtonPosition = magic_enum::enum_cast<ImGuiDir>(colorButtonPosition, magic_enum::case_insensitive).value_or(style.ColorButtonPosition);
  style.ButtonTextAlign = j.value("ButtonTextAlign", style.ButtonTextAlign);
  style.SelectableTextAlign = j.value("SelectableTextAlign", style.SelectableTextAlign);
  style.SeparatorTextBorderSize = j.value("SeparatorTextBorderSize", style.SeparatorTextBorderSize);
  style.SeparatorTextPadding = j.value("SeparatorTextPadding", style.SeparatorTextPadding);
  style.DisplayWindowPadding = j.value("DisplayWindowPadding", style.DisplayWindowPadding);
  style.DisplaySafeAreaPadding = j.value("DisplaySafeAreaPadding", style.DisplaySafeAreaPadding);
  style.DockingSeparatorSize = j.value("DockingSeparatorSize", style.DockingSeparatorSize);
  style.CurveTessellationTol = j.value("CurveTessellationTol", style.CurveTessellationTol);
  style.CircleTessellationMaxError = j.value("CircleTessellationMaxError", style.CircleTessellationMaxError);
}

bool ApplicationTheme::serialize(const std::filesystem::path& path) {
  nlohmann::ordered_json out;
  out["Name"] = m_themeName;
  out["Main"] = m_style;
  out["CommandEditor"] = m_commandEditor;
  std::ofstream outStream(path);
  outStream << out.dump(4);
  return outStream.good();
}

std::shared_ptr<ApplicationTheme> ApplicationTheme::loadTheme(const std::filesystem::path& path) {
  ApplicationTheme theme;
  std::ifstream file(path);
  if (file.good()) {
    const auto in = nlohmann::json::parse(file);
    theme.m_themeName = in.value("Name", theme.m_themeName);
    theme.m_style = in.value("Main", theme.m_style);
    theme.m_commandEditor = in.value("CommandEditor", theme.m_commandEditor);
  }
  return std::make_shared<ApplicationTheme>(theme);
}

std::shared_ptr<ApplicationTheme> ApplicationTheme::createFromStyle(const std::filesystem::path& path, const std::string& name, ImGuiStyle style) {
  ApplicationTheme theme;
  const float curveTess = style.CurveTessellationTol;
  const float circTessMaxErr = style.CircleTessellationMaxError;

  style.ScaleAllSizes(1.f / Settings::instance()->uiScale);
  style.CurveTessellationTol = curveTess;
  style.CircleTessellationMaxError = circTessMaxErr;

  theme.m_themeName = name;
  theme.m_style = style;
  theme.serialize(path);

  loadTheme(path);
  return std::make_shared<ApplicationTheme>(theme);
}