#include "Core/CommonUI/TilesetPicker.hpp"
#include "Core/MainWindow.hpp"

#include "imgui.h"
#include "imgui_internal.h"

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
    m_palette.invalidateCursor();
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
      }
      setPage(m_page);
    } else {
      m_palette.setTilesetNames({});
    }
  }

  m_palette.paintTiles();

  if (ImGui::Begin("Tilesets")) {
    const auto calc = ImGui::CalcTextSize("A");
    ImGui::BeginChild("##tileset", {0, ImGui::GetContentRegionAvail().y - (calc.y + (ImGui::GetStyle().ItemSpacing.y * 3) + ImGui::GetStyle().FramePadding.y)}, 0,
                      ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_HorizontalScrollbar);
    {
      // We want to always render region tiles
      const ImVec2 cursorPos = (ImGui::GetMousePos() - ImGui::GetCurrentWindow()->ContentRegionRect.Min);
      if (m_palette.isPageValid(m_page) || m_page == -1) {
        ImGui::Image(static_cast<ImTextureID>(m_palette), static_cast<ImVec2>(m_palette.imageSize()));
        if ((ImGui::IsWindowFocused() || ImGui::IsWindowHovered()) && ImGui::IsItemHovered()) {
          if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            // printf("Pick start!");
            m_palette.onCursorClicked({cursorPos.x, cursorPos.y});
            // printf(" %i %i\n", m_palette.cursorRect().x(), m_palette.cursorRect().y());
          }

          if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            // printf("Cursor Drag!");
            m_palette.onCursorDrag({cursorPos.x, cursorPos.y});
            // printf(" %i %i %i %i\n", m_palette.cursorRect().x(), m_palette.cursorRect().y(), m_palette.cursorRect().width(), m_palette.cursorRect().height());
          }

          if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            // printf("Cursor Release!\n");
            m_palette.onCursorReleased();
            auto penData = m_palette.penData();
            // auto size = m_palette.penSize();
            for (const auto& pen : penData) {
              // printf("%4i, %4i, %4i, %4i\n", pen[0], pen[1], pen[2], pen[3]);
            }
          }
        }

        if (m_palette.cursorRect().isValid()) {
          const auto* win = ImGui::GetCurrentWindow();
          const auto rect = m_palette.cursorPixelRect();
          const auto min = static_cast<ImVec2>(rect.topLeft());
          const auto max = static_cast<ImVec2>(rect.bottomRight());
          win->DrawList->AddRect(win->ContentRegionRect.Min + (min + ImVec2{3.f, 3.f}), win->ContentRegionRect.Min + (max - ImVec2{3.f, 3.f}), 0xFF000000, 0.f, 0, 5.f);
          win->DrawList->AddRect(win->ContentRegionRect.Min + (min + ImVec2{3.f, 3.f}), win->ContentRegionRect.Min + (max - ImVec2{3.f, 3.f}), 0xFFFFFFFF, 0.f, 0, 3.f);
        }
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
    }
    ImGui::EndChild();
  }

  ImGui::End();
}