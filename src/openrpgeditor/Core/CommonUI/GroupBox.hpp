#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include <imgui.h>

class GroupBox {
public:
  GroupBox(std::string_view title, std::string_view id, const ImVec2& size = {0, 0}, bool* checked = nullptr, ImGuiChildFlags childFlags = 0, ImGuiWindowFlags windowFlags = 0);
  bool begin();
  void end();

  bool isChecked() const { return m_checked ? *m_checked : false; }
  bool wasClicked() const { return m_clicked; }

private:
  std::string m_title;
  std::string m_id;
  ImVec2 m_size;
  uint32_t m_childFlags;
  uint32_t m_windowFlags;

  bool* m_checked = nullptr;
  bool m_clicked = false;

  bool m_visible;
  ImVec2 m_groupStart;
};
