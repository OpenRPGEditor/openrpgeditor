#include "Core/CommonUI/TilesetPicker.hpp"
#include "Core/Project.hpp"

#include "../../../../cmake-build-minsizerel/_deps/sfml-src/extlibs/headers/vulkan/vulkan_core.h"
#include "imgui.h"

#include <complex.h>

void TilesetPicker::setPage(int page) {
  m_page = page;
  m_palette.setPageIndex(m_page);
}
void TilesetPicker::drawPageButton(const std::string_view pageStr, const int page) {
  // For buttons, we always want to render autotiles and region tiles
  if (page != 0 && page != -1 && !m_palette.isPageValid(page)) {
    return;
  }

  ImGui::PushStyleColor(ImGuiCol_Button, m_page == page ? ImGui::GetColorU32(ImGuiCol_ButtonActive) : ImGui::GetColorU32(ImGuiCol_Button));
  if (ImGui::Button(std::format("  {}  ", pageStr).c_str())) {
    setPage(page);
  }
  ImGui::PopStyleColor();
}
void TilesetPicker::draw() {
  if (m_map != m_parent->currentMap()) {
    m_map = m_parent->currentMap();
    if (m_map != nullptr) {
      m_palette.setTilesetNames(Database::instance()->tilesets.tileset(m_map->tilesetId())->tilesetNames());
      while (!m_palette.isPageValid(m_page) && m_page > 0) {
        --m_page;
        printf("%i\n", m_page);
      }
      const bool samePage = m_page == m_palette.pageIndex();
      setPage(m_page);
      /* We need to force it to repaint tiles just in case we select the same page */
      if (samePage) {
        m_palette.paintTiles();
      }
    } else {
      m_palette.setTilesetNames({});
    }
  }
  if (ImGui::Begin("Tilesets")) {
    const auto height = ImGui::CalcTextSize("A").y + (ImGui::GetStyle().FramePadding.y + (ImGui::GetStyle().ItemSpacing.y * 2));
    ImGui::BeginChild("##tileset", {0, ImGui::GetContentRegionAvail().y - height}, 0, ImGuiWindowFlags_NoBackground);
    {
      // We want to always render region tiles
      if (m_palette.isPageValid(m_page) || m_page == -1) {
        ImGui::Image(static_cast<ImTextureID>(m_palette), static_cast<ImVec2>(m_palette.imageSize() * App::DPIHandler::get_ui_scale()));
      }
    }
    ImGui::EndChild();
    ImGui::BeginChild("##tileset_buttons", {}, 0, ImGuiWindowFlags_NoBackground);
    {
      drawPageButton("A"sv, 0);
      ImGui::SameLine(0.f, 0.f);
      drawPageButton("B"sv, 1);
      ImGui::SameLine(0.f, 0.f);
      drawPageButton("C"sv, 2);
      ImGui::SameLine(0.f, 0.f);
      drawPageButton("D"sv, 3);
      ImGui::SameLine(0.f, 0.f);
      drawPageButton("E"sv, 4);
      ImGui::SameLine(0.f, 0.f);
      drawPageButton("R"sv, -1);
      ImGui::SameLine(0.f, 0.f);
      int spacing = m_palette.spacing();
      ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - (ImGui::CalcTextSize("Spacing").x + ImGui::GetStyle().FramePadding.x));
      if (ImGui::SliderInt("Spacing", &spacing, 0, 10)) {
        m_palette.setSpacing(spacing);
      }
    }
    ImGui::EndChild();
  }
  ImGui::End();
}