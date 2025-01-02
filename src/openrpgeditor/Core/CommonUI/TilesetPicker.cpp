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
    const auto height = ImGui::CalcTextSize("A").y + (ImGui::GetStyle().FramePadding.y + (ImGui::GetStyle().ItemSpacing.y * 2));
    ImGui::BeginChild("##tileset", {0, ImGui::GetContentRegionAvail().y - height}, 0, ImGuiWindowFlags_NoBackground);
    {
      // We want to always render region tiles
      const ImVec2 cursorPos = (ImGui::GetMousePos() - ImGui::GetCurrentWindow()->ContentRegionRect.Min) / App::DPIHandler::get_ui_scale();
      if (m_palette.isPageValid(m_page) || m_page == -1) {
        ImGui::Image(static_cast<ImTextureID>(m_palette), static_cast<ImVec2>(m_palette.imageSize() * App::DPIHandler::get_ui_scale()));
        if (ImGui::IsWindowFocused() || ImGui::IsWindowHovered()) {
          if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            printf("Pick start!");
            m_palette.onCursorClicked({cursorPos.x, cursorPos.y});
            printf(" %i %i\n", m_palette.cursorRect().x(), m_palette.cursorRect().y());
          }

          if (ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            printf("Cursor Drag!");
            m_palette.onCursorDrag({cursorPos.x, cursorPos.y});
            printf(" %i %i %i %i\n", m_palette.cursorRect().x(), m_palette.cursorRect().y(), m_palette.cursorRect().width(), m_palette.cursorRect().height());
          }

          if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
            printf("Cursor Release!\n");
            m_palette.onCursorReleased();
            auto penData = m_palette.penData();
            // auto size = m_palette.penSize();
            int i = 0;
            for (; const auto& pen : penData) {
              if (!(i % 4)) {
                printf("\n");
              }
              printf("%4i, ", pen);
              ++i;
            }
            if (i != 0) {
              printf("\n");
            }
          }
        }

        const auto* win = ImGui::GetCurrentWindow();
        const auto rect = m_palette.cursorPixelRect();
        const auto min = static_cast<ImVec2>(rect.topLeft() * App::DPIHandler::get_ui_scale());
        const auto max = static_cast<ImVec2>(rect.bottomRight() * App::DPIHandler::get_ui_scale());
        win->DrawList->AddRect(win->ContentRegionRect.Min + (min + ImVec2{3.f, 3.f}), win->ContentRegionRect.Min + (max - ImVec2{2.f, 2.f}), 0xFF000000, 0.f, 0, 5.f);
        win->DrawList->AddRect(win->ContentRegionRect.Min + (min + ImVec2{3.f, 3.f}), win->ContentRegionRect.Min + (max - ImVec2{2.f, 2.f}), 0xFFFFFFFF, 0.f, 0, 3.f);
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