#include "Core/MapEditor.hpp"

#include "DPIHandler.hpp"
#include "Core/Project.hpp"

#include "ImGuiUtils.hpp"

#include "imgui_internal.h"
#include "imgui.h"

#include <format>

struct EventMoveUndoCommand : IUndoCommand {
  EventMoveUndoCommand() = delete;
  EventMoveUndoCommand(Event* event, int x, int y) : m_event(event), m_x(x), m_y(y) {}
  int type() const override { return -1; }

  bool undo() override {
    if (m_event) {
      std::swap(m_event->x, m_x);
      std::swap(m_event->y, m_y);
      return true;
    }

    return false;
  }

  std::string description() override {
    if (!m_event) {
      return "Invalid move command!";
    }
    if (!m_isRedo) {
      return std::format("Move {} from {},{} to {},{}",
                         m_event->name.empty() ? std::to_string(m_event->id) : m_event->name, m_x, m_y, m_event->x,
                         m_event->y);
    }
    return std::format("Move {} from {},{} to {},{}",
                       m_event->name.empty() ? std::to_string(m_event->id) : m_event->name, m_event->x, m_event->y, m_x,
                       m_y);
  }

private:
  Event* m_event = nullptr;
  int m_x = -1;
  int m_y = -1;
};

void MapEditor::setMap(Map* map, MapInfo* info) {
  if (map == nullptr) {
    m_mapRenderer.setMap(nullptr, nullptr);
  } else {
    m_mapRenderer.setMap(map, m_parent->tileset(map->tilesetId));
  }

  m_eventEditors.clear();
  m_map = map;
  m_mapInfo = info;
  m_lowerLayer.clear();
  m_upperLayer.clear();
}

int MapEditor::tileSize() { return m_parent->system().tileSize; }

void MapEditor::drawGrid(ImGuiWindow* win) {
  for (int y = tileSize(); y < (m_map->height * tileSize()) * m_mapScale; y += tileSize() * m_mapScale) {
    win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{0.f, static_cast<float>(y)},
                           win->ContentRegionRect.Min +
                               ImVec2{(m_map->width * tileSize()) * m_mapScale, static_cast<float>(y)},
                           0x7f0a0a0a, 3.f);
  }

  for (int x = tileSize(); x < (m_map->width * tileSize()) * m_mapScale; x += tileSize() * m_mapScale) {
    win->DrawList->AddLine(win->ContentRegionRect.Min + ImVec2{static_cast<float>(x), 0.f},
                           win->ContentRegionRect.Min +
                               ImVec2{static_cast<float>(x), (m_map->height * tileSize()) * m_mapScale},
                           0x7f0a0a0a, 3.f);
  }
}
float roundToNearestQuarter(float num) { return static_cast<float>(static_cast<int>(num * 4)) / 4; }

void MapEditor::handleEventDrag() {
  if (m_movingEvent) {
    /* For now we'll prevent events from occupying the same tile */
    /* TODO(phil): Implement some way to sort through events on the same tile */
    int oldX = m_movingEvent->x;
    int oldY = m_movingEvent->y;

    auto it = std::find_if(m_map->events.begin(), m_map->events.end(), [&](const std::optional<Event>& e) {
      return e && e->x == tileCellX() && e->y == tileCellY() && &e.value() != m_movingEvent;
    });

    m_movingEvent->x = m_tileCursor.tileX();
    m_movingEvent->y = m_tileCursor.tileY();
    if (it != m_map->events.end()) {
      m_movingEvent->x = oldX;
      m_movingEvent->y = oldY;
    }
  }
}
void MapEditor::handleMouseInput(ImGuiWindow* win) {

  if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow) || ImGui::IsKeyPressed(ImGuiKey_RightArrow) ||
      ImGui::IsKeyPressed(ImGuiKey_UpArrow) || ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
    m_tileCursor.setKeyboardMode();
  } else if (std::fabs(ImGui::GetIO().MouseDelta.x) > 0.5f || std::fabs(ImGui::GetIO().MouseDelta.y) > 0.5f) {
    m_tileCursor.setMouseMode();
  }

  if (ImGui::IsWindowHovered() && m_parent->editMode() == EditMode::Map) {
    m_tileCursor.update(m_mapScale, m_map->width, m_map->height, m_parent->system().tileSize, win);
  } else if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left) &&
             (m_parent->editMode() == EditMode::Event || m_movingEvent)) {
    m_tileCursor.update(m_mapScale, m_map->width, m_map->height, m_parent->system().tileSize, win);
    handleEventDrag();
    m_scaleChanged = false;
  } else if (m_scaleChanged || m_tileCursor.mode() == MapCursorMode::Keyboard) {
    m_tileCursor.update(m_mapScale, m_map->width, m_map->height, m_parent->system().tileSize, win);
    handleEventDrag();
    m_scaleChanged = false;
  }

  if (m_parent->editMode() == EditMode::Event && m_tileCursor.mode() == MapCursorMode::Keyboard &&
      (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) &&
      (!m_movingEvent || !m_selectedEvent)) {
    m_selectedEvent = m_map->eventAt(m_tileCursor.tileX(), m_tileCursor.tileY());
    m_movingEvent = m_selectedEvent;
    if (m_movingEvent) {
      m_movingEventX = m_movingEvent->x;
      m_movingEventY = m_movingEvent->y;
    }
  } else if (m_parent->editMode() == EditMode::Event && m_tileCursor.mode() == MapCursorMode::Keyboard &&
             (ImGui::IsKeyReleased(ImGuiKey_LeftShift) || ImGui::IsKeyReleased(ImGuiKey_RightShift))) {
    if (m_movingEvent) {
      m_movingEvent->x = m_tileCursor.tileX();
      m_movingEvent->y = m_tileCursor.tileY();
    }
  }

  if (m_selectedEvent && !m_movingEvent && m_selectedEvent->x != m_tileCursor.tileX() &&
      m_selectedEvent->y != m_tileCursor.tileY()) {
    m_selectedEvent = nullptr;
  }

  if ((ImGui::IsMouseClicked(ImGuiMouseButton_Left) && m_parent->editMode() == EditMode::Event &&
       (m_movingEvent || m_selectedEvent))) {
    auto event = m_map->eventAt(m_tileCursor.tileX(), m_tileCursor.tileY());
    if (event == nullptr) {
      m_movingEvent = nullptr;
      m_selectedEvent = nullptr;
    } else {
      m_selectedEvent = event;
    }
    m_hasScrolled = true;
  }

  if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered() &&
      m_parent->editMode() == EditMode::Event && m_movingEvent == nullptr) {
    m_selectedEvent = m_map->eventAt(m_tileCursor.tileX(), m_tileCursor.tileY());
    m_movingEvent = m_selectedEvent;
    if (m_movingEvent) {
      m_movingEventX = m_movingEvent->x;
      m_movingEventY = m_movingEvent->y;
    }
    m_hasScrolled = true;
  }

  if ((ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) || ImGui::IsKeyPressed(ImGuiKey_Enter) ||
       ImGui::IsKeyPressed(ImGuiKey_KeypadEnter)) &&
      m_parent->editMode() == EditMode::Event) {

    if (m_selectedEvent != nullptr) {
      auto it = std::find_if(m_eventEditors.begin(), m_eventEditors.end(),
                             [&](const EventEditor& editor) { return editor.event()->id == m_selectedEvent->id; });
      if (it == m_eventEditors.end()) {
        m_eventEditors.emplace_back(m_parent, m_selectedEvent);
      }
    } else if (m_eventEditors.empty() && ImGui::IsWindowFocused()) {
      auto ev = m_map->createNewEvent();
      ev->x = tileCellX();
      ev->y = tileCellY();
      m_eventEditors.emplace_back(m_parent, ev);
    }
  }

  if (((m_tileCursor.mode() == MapCursorMode::Keyboard && (ImGui::IsKeyReleased(ImGuiKey_LeftShift)) ||
        ImGui::IsKeyReleased(ImGuiKey_RightShift)) ||
       ImGui::IsMouseReleased(ImGuiMouseButton_Left)) &&
      m_parent->editMode() == EditMode::Event) {
    /* If we have a selected actor and it's no longer in it's original location, push it onto the undo stack
     * as an operation
     */
    if (m_movingEvent != nullptr && (m_movingEvent->x != m_movingEventX || m_movingEvent->y != m_movingEventY)) {
      m_parent->addUndo(std::make_shared<EventMoveUndoCommand>(m_movingEvent, m_movingEventX, m_movingEventY));
    }
    m_movingEvent = nullptr;
  }
}

void MapEditor::handleKeyboardShortcuts() {
  /* General */
  return;
  if (ImGui::IsKeyPressed(ImGuiKey_Delete) || ImGui::IsKeyPressed(ImGuiKey_Backspace)) {
    // TODO: Undo command
    if (ImGui::IsWindowFocused() && selectedEvent()) {
      m_map->deleteEvent(selectedEvent()->id);
      setSelectedEvent(nullptr);
    }
  }
}
void MapEditor::renderLayerTex(ImGuiWindow* win, const MapRenderer::TileLayer& tLayer) {
  if (!tLayer.tex) {
    return;
  }

  for (const TileRect& tile : tLayer.rects) {
    const float x0 = tile.x;
    const float x1 = tile.x + tile.tileWidth;
    const float y0 = tile.y;
    const float y1 = tile.y + tile.tileHeight;
    const float u0 = tile.u / tLayer.tex.width();
    const float u1 = (tile.u + tile.tileWidth) / tLayer.tex.width();
    const float v0 = tile.v / tLayer.tex.height();
    const float v1 = (tile.v + tile.tileHeight) / tLayer.tex.height();
    win->DrawList->AddImage(tLayer.tex.get(), win->ContentRegionRect.Min + (ImVec2{x0, y0} * m_mapScale),
                            win->ContentRegionRect.Min + (ImVec2{x1, y1} * m_mapScale), ImVec2{u0, v0}, ImVec2{u1, v1});
  }
}
void MapEditor::renderLayer(ImGuiWindow* win, const MapRenderer::MapLayer& layer) {
  renderLayerTex(win, layer.tileLayers[0]); // A1
  renderLayerTex(win, layer.tileLayers[1]); // A2
  renderLayerTex(win, layer.tileLayers[2]); // A3
  renderLayerTex(win, layer.tileLayers[3]); // A4
  renderLayerTex(win, layer.tileLayers[4]); // A5
  renderLayerTex(win, layer.tileLayers[5]); // B
  renderLayerTex(win, layer.tileLayers[8]); // E
  renderLayerTex(win, layer.tileLayers[7]); // D
  renderLayerTex(win, layer.tileLayers[6]); // C
}
void MapEditor::draw() {
  // Keep mapScale to a quarter step
  if (ImGui::IsKeyDown(ImGuiKey_MouseWheelY) && ImGui::IsKeyDown(ImGuiKey_LeftCtrl)) {
    m_mapScale += ImGui::GetIO().MouseWheel * 0.25f;
    m_scaleChanged = true;
  }

  m_mapScale = roundToNearestQuarter(m_mapScale);
  m_mapScale = std::clamp(m_mapScale, .25f, 4.f);

  std::erase_if(m_eventEditors, [](EventEditor& editor) { return !editor.draw(); });

  if (ImGui::Begin("Map Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar)) {
    ImGui::BeginChild("##mapcontents", ImVec2(0, ImGui::GetContentRegionAvail().y - App::DPIHandler::scale_value(45.f)),
                      ImGuiChildFlags_Border, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoNav);

    if (m_map) {
      ImGui::Dummy(ImVec2{m_map->width * (m_parent->system().tileSize * m_mapScale),
                          m_map->height * (m_parent->system().tileSize * m_mapScale)});

      ImGuiWindow* win = ImGui::GetCurrentWindow();
      handleMouseInput(win);

      if (m_tileCursor.mode() == MapCursorMode::Keyboard && !ImGui::IsWindowFocused()) {
        ImGui::FocusWindow(win, ImGuiFocusRequestFlags_UnlessBelowModal | ImGuiFocusRequestFlags_RestoreFocusedChild);
      }

      if (m_initialScrollSet) {
        ImGui::SetScrollX(m_initialScrollX);
        ImGui::SetScrollY(m_initialScrollY);
        m_initialScrollX = m_initialScrollY = 0.0;
        m_initialScrollSet = false;
      }

      if (ImGui::GetScrollX() != m_mapInfo->scrollX || ImGui::GetScrollY() != m_mapInfo->scrollY) {
        m_tileCursor.setVisible(false);
      }
      m_mapInfo->scrollX = ImGui::GetScrollX();
      m_mapInfo->scrollY = ImGui::GetScrollY();

      m_mapRenderer.update();

      renderLayer(win, m_mapRenderer.m_lowerLayer);
      renderLayer(win, m_mapRenderer.m_upperLayer);

      if (ImGui::IsWindowHovered() || m_parent->editMode() == EditMode::Event) {
        m_tileCursor.draw(win);
      }

      drawGrid(win);
      auto sortedEvents = m_map->getSorted();
      for (auto& event : sortedEvents) {
        if (!event) {
          continue;
        }
        auto realEvent = m_map->event(event->id);
        if (m_selectedEvent == realEvent && !m_hasScrolled) {
          ImGui::SetScrollX((win->ContentRegionRect.Min.x / 2) +
                            (((event->x * tileSize()) * m_mapScale) - (win->ContentRegionRect.Max.x / 2)));
          ImGui::SetScrollY((win->ContentRegionRect.Min.y / 2) +
                            (((event->y * tileSize()) * m_mapScale) - (win->ContentRegionRect.Max.y / 2)));
          m_hasScrolled = true;
        }
        bool isHovered = event->x == tileCellX() && event->y == tileCellY();
        MapEvent mapEvent(this, &event.value());
        mapEvent.draw(m_mapScale, isHovered, m_selectedEvent == realEvent, m_parent->editMode() != EditMode::Event,
                      win);
      }
    }
    ImGui::EndChild();
    ImGui::Text("Scale:");
    ImGui::SameLine();
    ImGui::SliderFloat("##map_scale", &m_mapScale, 0.25f, 4.f);
    ImGui::SameLine();
    std::string fmt =
        std::format("Tile {}, ({}, {})", m_mapRenderer.tileId(m_tileCursor.tileX(), m_tileCursor.tileY(), 0),
                    m_tileCursor.tileX(), m_tileCursor.tileY());
    if (m_map) {
      auto ev = std::find_if(m_map->events.begin(), m_map->events.end(), [&](const std::optional<Event>& e) {
        return e && m_tileCursor.tileX() == e->x && m_tileCursor.tileY() == e->y;
      });
      if (ev != m_map->events.end()) {
        fmt += std::format(" {} ({:03})", (*ev)->name, (*ev)->id);
      }
    }
    ImGui::Text("%s", fmt.c_str());
  }
  ImGui::End();
  drawMapProperties();

  // if (ImGui::BeginPopupContextWindow()) {
  //   if (ImGui::BeginPopupContextItem(""))
  // }
}

void MapEditor::drawMapProperties() {
  if (ImGui::Begin("Map Properties")) {
    char buf[4096]{};
    if (m_map) {
      ImGui::BeginGroupPanel();
      {
        ImGui::SeparatorText("General Settings");
        ImGui::BeginGroup();
        {
          ImGui::Text("Name - ID: %.03i", m_mapInfo->id);
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15));
          strncpy(buf, m_mapInfo->name.c_str(), 4096);
          if (ImGui::InputText("##map_name", buf, 4096)) {
            m_mapInfo->name = buf;
          }
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Display Name");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15));
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::InputText("##map_display_name", buf, 4096)) {
            m_map->displayName = buf;
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImGui::Text("Tileset");
          strncpy(buf, m_parent->currentMapInfo()->name.c_str(), 4096);
          std::string text = m_parent->tileset(m_map->tilesetId)->name.empty()
                                 ? "##map_tileset_button_empty"
                                 : m_parent->tileset(m_map->tilesetId)->name;
          ImGui::PushID("##map_tileset_button");
          if (ImGui::Button(text.c_str(),
                            ImVec2{ImGui::GetContentRegionMax().x / 2 - App::DPIHandler::scale_value(15), 0})) {}
          ImGui::PopID();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Width");
          ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - App::DPIHandler::scale_value(15));
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::DragInt("##map_width", &m_map->width, 0, 0, 256)) {
            m_map->displayName = buf;
          }
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Height");
          ImGui::SetNextItemWidth(((ImGui::GetContentRegionMax().x / 2) / 2) - App::DPIHandler::scale_value(15));
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::DragInt("##map_height", &m_map->height, 0, 0, 256)) {
            m_map->displayName = buf;
          }
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Scroll Type");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15));
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::BeginCombo("##map_scroll_type",
                                DecodeEnumName(magic_enum::enum_name(m_map->scrollType)).c_str())) {
            for (const auto& e : magic_enum::enum_values<ScrollType>()) {
              if (ImGui::Selectable(DecodeEnumName(magic_enum::enum_name(e)).c_str(), e == m_map->scrollType)) {
                m_map->scrollType = e;
              }
              if (e == m_map->scrollType) {
                ImGui::SetItemDefaultFocus();
              }
            }
            ImGui::EndCombo();
          }
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Enc. Steps");
          ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15));
          strncpy(buf, m_map->displayName.c_str(), 4096);
          if (ImGui::DragInt("##map_enc_steps", &m_map->encounterStep, 0, 0, 999)) {
            m_map->displayName = buf;
          }
        }
        ImGui::EndGroup();
      }
      ImGui::Separator();
      {
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Autoplay BGM", &m_map->autoPlayBgm)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->autoPlayBgm);
          {
            ImGui::PushID("##map_bgm_button");
            ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - 30);
            std::string text = m_map->bgm.name.empty() ? "##map_bgm_button_empty" : m_map->bgm.name;
            if (ImGui::Button(text.c_str(),
                              ImVec2{(ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15), 0})) {}
            ImGui::PopID();
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Autoplay BGS", &m_map->autoPlayBgs)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->autoPlayBgs);
          {
            ImGui::PushID("##map_bgs_button");
            std::string text = m_map->bgs.name.empty() ? "##map_bgs_button_empty" : m_map->bgs.name;
            if (ImGui::Button(text.c_str(),
                              ImVec2{(ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15), 0})) {}
            ImGui::PopID();
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Battleback", &m_map->specifyBattleBack)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->specifyBattleBack);
          {
            ImGui::PushID("##map_battleback_button");
            // TODO: Combine battleBack1Name and battleBack2Name
            std::string text = m_map->bgs.name.empty() ? "##map_battleback_button_empty" : m_map->battleBack1Name;
            if (ImGui::Button(text.c_str(),
                              ImVec2{(ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15), 0})) {}
            ImGui::PopID();
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Disable Dashing", &m_map->disableDashing)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->disableDashing);
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
      }
      ImGui::Separator();
      {
        ImGui::BeginGroup();
        {
          ImGui::Text("Parallax Background");
          ImGui::PushID("##map_parallax_button");
          std::string text = m_map->bgs.name.empty() ? "##map_parallax_button_empty" : m_map->parallaxName;
          if (ImGui::Button(text.c_str(),
                            ImVec2{(ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15), 0})) {}
          ImGui::PopID();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Loop Horizontally", &m_map->parallaxLoopX)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->parallaxLoopX);
          {
            ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15));
            if (ImGui::DragInt("##map_parallax_Sx", &m_map->parallaxSx, 0, 0, 999)) {}
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Loop Vertically", &m_map->parallaxLoopY)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->parallaxLoopY);
          {
            ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - App::DPIHandler::scale_value(15));
            if (ImGui::DragInt("##map_parallax_Sy", &m_map->parallaxSy, 0, 0, 999)) {}
          }
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          if (ImGui::Checkbox("Show in Editor", &m_map->parallaxShow)) {}
          ImGui::PushItemFlag(ImGuiItemFlags_Disabled, !m_map->parallaxShow);
          ImGui::PopItemFlag();
        }
        ImGui::EndGroup();
        ImGui::BeginGroup();
        {
          ImVec2 cursorPos = ImGui::GetCursorPos();
          ImGuiInputTextFlags flags = ImGuiInputTextFlags_CtrlEnterForNewLine;
          // Move back up a couple couple pixels
          cursorPos.y -= 4.f;
          ImGui::SetCursorPos(cursorPos);
          ImGui::Text("Note");

          strncpy(buf, m_map->note.c_str(), 4096);
          if (ImGui::InputTextMultiline("##map_note", buf, 2048,
                                        ImVec2(ImGui::GetContentRegionMax().x - App::DPIHandler::scale_value(15),
                                               App::DPIHandler::scale_value(400)),
                                        flags)) {
            m_map->note = buf;
          }
        }
        ImGui::Dummy(ImVec2(0.0f, 15.0f));
        ImGui::EndGroup();
      }
      ImGui::EndGroupPanel();
    }
  }
  ImGui::End();
}