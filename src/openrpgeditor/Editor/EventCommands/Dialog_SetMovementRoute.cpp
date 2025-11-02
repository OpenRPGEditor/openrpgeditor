#include "Editor/EventCommands/Dialog_SetMovementRoute.hpp"
#include "Database/Database.hpp"
#include "Database/EventCommands/MovementRoute/DirectionFixOFF.hpp"
#include "Database/EventCommands/MovementRoute/DirectionFixON.hpp"
#include "Database/EventCommands/MovementRoute/Move1StepBackward.hpp"
#include "Database/EventCommands/MovementRoute/Move1StepForward.hpp"
#include "Database/EventCommands/MovementRoute/MoveAtRandom.hpp"
#include "Database/EventCommands/MovementRoute/MoveAwayFromPlayer.hpp"
#include "Database/EventCommands/MovementRoute/MoveDown.hpp"
#include "Database/EventCommands/MovementRoute/MoveLeft.hpp"
#include "Database/EventCommands/MovementRoute/MoveLowerLeft.hpp"
#include "Database/EventCommands/MovementRoute/MoveLowerRight.hpp"
#include "Database/EventCommands/MovementRoute/MoveRight.hpp"
#include "Database/EventCommands/MovementRoute/MoveTowardPlayer.hpp"
#include "Database/EventCommands/MovementRoute/MoveUp.hpp"
#include "Database/EventCommands/MovementRoute/MoveUpperLeft.hpp"
#include "Database/EventCommands/MovementRoute/MoveUpperRight.hpp"
#include "Database/EventCommands/MovementRoute/PlaySE.hpp"
#include "Database/EventCommands/MovementRoute/SteppingAnimationOFF.hpp"
#include "Database/EventCommands/MovementRoute/SteppingAnimationON.hpp"
#include "Database/EventCommands/MovementRoute/ThroughOFF.hpp"
#include "Database/EventCommands/MovementRoute/ThroughON.hpp"
#include "Database/EventCommands/MovementRoute/TransparentOFF.hpp"
#include "Database/EventCommands/MovementRoute/TransparentON.hpp"
#include "Database/EventCommands/MovementRoute/Turn180Deg.hpp"
#include "Database/EventCommands/MovementRoute/Turn90DegLeft.hpp"
#include "Database/EventCommands/MovementRoute/Turn90DegLeftOrRight.hpp"
#include "Database/EventCommands/MovementRoute/Turn90DegRight.hpp"
#include "Database/EventCommands/MovementRoute/TurnAtRandom.hpp"
#include "Database/EventCommands/MovementRoute/TurnAwayFromPlayer.hpp"
#include "Database/EventCommands/MovementRoute/TurnDown.hpp"
#include "Database/EventCommands/MovementRoute/TurnLeft.hpp"
#include "Database/EventCommands/MovementRoute/TurnRight.hpp"
#include "Database/EventCommands/MovementRoute/TurnTowardPlayer.hpp"
#include "Database/EventCommands/MovementRoute/TurnUp.hpp"
#include "Database/EventCommands/MovementRoute/WalkingAnimationOFF.hpp"
#include "Database/EventCommands/MovementRoute/WalkingAnimationON.hpp"
#include "Editor/EventCommands/MovementRoute/Dialog_MovementChangeBlendMode.hpp"
#include "Editor/EventCommands/MovementRoute/Dialog_MovementChangeFrequency.hpp"
#include "Editor/EventCommands/MovementRoute/Dialog_MovementChangeImage.hpp"
#include "Editor/EventCommands/MovementRoute/Dialog_MovementChangeOpacity.hpp"
#include "Editor/EventCommands/MovementRoute/Dialog_MovementChangeSpeed.hpp"
#include "Editor/EventCommands/MovementRoute/Dialog_MovementJump.hpp"
#include "Editor/EventCommands/MovementRoute/Dialog_MovementScript.hpp"
#include "Editor/EventCommands/MovementRoute/Dialog_MovementSwitchOFF.hpp"
#include "Editor/EventCommands/MovementRoute/Dialog_MovementSwitchON.hpp"
#include "Editor/EventCommands/MovementRoute/Dialog_MovementWait.hpp"
#include "Editor/ImGuiExt/ImGuiUtils.hpp"
#include "imgui.h"
#include "MovementRoute/Dialogue_MovementPlaySE.hpp"

#include <clip.h>
#include <tuple>

static clip::format OREMovementCommand = -1;
std::tuple<bool, bool> Dialog_SetMovementRoute::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize({ImGui::GetMainViewport()->Size.x / 2, ImGui::GetMainViewport()->Size.y / 2}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar)) {
    const auto buttonsSize = ImGui::CalcTextSize("OKCANCEL");
    // Character Selection
    ImGui::BeginChild("##movementroute_top_child", {0, ImGui::GetContentRegionAvail().y - (buttonsSize.y + (ImGui::GetStyle().FramePadding.y * 2) + ImGui::GetStyle().ItemSpacing.y)});
    {
      m_hasFocus = ImGui::IsWindowFocused(ImGuiFocusedFlags_ChildWindows | ImGuiFocusedFlags_NoPopupHierarchy);
      const auto calc = ImGui::CalcTextSize("#######################");
      ImGui::BeginChild("##movementroute_inner_left_child", ImVec2{calc.x, 0}, ImGuiChildFlags_ResizeX | ImGuiChildFlags_Borders);
      {
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
        ImGui::BeginDisabled(m_isEventRoute);
        if (ImGui::BeginCombo("##showroute_character", m_isEventRoute ? "" : Database::instance()->eventNameOrId(m_character).c_str())) {

          if (ImGui::Selectable(trNOOP("Player"), m_character == -1)) {
            m_character = -1;
            ImGui::SetItemDefaultFocus();
          }
          if (ImGui::Selectable(trNOOP("This Event"), m_character == 0)) {
            m_character = 0;
            ImGui::SetItemDefaultFocus();
          }

          for (auto& dataSource : Database::instance()->mapInfos->currentMap()->map()->events()) {
            if (!dataSource)
              continue;

            bool is_selected = m_character == dataSource->id();
            if (ImGui::Selectable(Database::instance()->instance()->eventNameOrId(dataSource->id()).c_str(), is_selected)) {
              m_character = dataSource->id();
              if (is_selected)
                ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
        ImGui::EndDisabled();

        if (ImGui::BeginTable("##route_commands", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY,
                              {ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x, (calc.y + ImGui::GetStyle().ItemSpacing.y) * 16})) {
          ImGui::TableSetupScrollFreeze(1, 0);
          ImGui::TableSetupColumn(trNOOP("Step"));
          ImGui::TableNextRow();
          ImGui::TableNextColumn();

          for (int n = 0; n < m_route->list().size(); n++) {
            ImGui::TableNextColumn();
            const bool isSelected = m_selected == n || (m_selectedEnd != -1 && n > m_selected && n <= m_selectedEnd);
            char text[4096];
            sprintf(text, "%s##cmd%i", m_route->list().at(n)->stringRep(*Database::instance()).c_str(), n);
            if (ImGui::SelectableWithBorder(text, isSelected, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
              if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                if (m_route->list().at(m_selected)->code() == EventCode::Jump) {
                  m_isNewEntry = false;
                  auto movementCmd = std::dynamic_pointer_cast<MovementJumpCommand>(m_route->list().at(m_selected));
                  m_movementRouteDialog = std::make_shared<Dialog_MovementJump>(trNOOP("Jump"), movementCmd);
                  m_movementRouteDialog->setOpen(true);
                }
                if (m_route->list().at(m_selected)->code() == EventCode::Wait_del_Movement) {
                  m_isNewEntry = false;
                  auto movementCmd = std::dynamic_pointer_cast<MovementWaitCommand>(m_route->list().at(m_selected));
                  m_movementRouteDialog = std::make_shared<Dialog_MovementWait>(trNOOP("Wait"), movementCmd);
                  m_movementRouteDialog->setOpen(true);
                }
                if (m_route->list().at(m_selected)->code() == EventCode::Switch_ON) {
                  m_isNewEntry = false;
                  auto movementCmd = std::dynamic_pointer_cast<MovementSwitchONCommand>(m_route->list().at(m_selected));
                  m_movementRouteDialog = std::make_shared<Dialog_MovementSwitchON>(trNOOP("Switch ON"), movementCmd);
                  m_movementRouteDialog->setOpen(true);
                }
                if (m_route->list().at(m_selected)->code() == EventCode::Switch_OFF) {
                  m_isNewEntry = false;
                  auto movementCmd = std::dynamic_pointer_cast<MovementSwitchOFFCommand>(m_route->list().at(m_selected));
                  m_movementRouteDialog = std::make_shared<Dialog_MovementSwitchOFF>(trNOOP("Switch OFF"), movementCmd);
                  m_movementRouteDialog->setOpen(true);
                }
                if (m_route->list().at(m_selected)->code() == EventCode::Speed) {
                  m_isNewEntry = false;
                  auto movementCmd = std::dynamic_pointer_cast<MovementSpeedCommand>(m_route->list().at(m_selected));
                  m_movementRouteDialog = std::make_shared<Dialog_MovementChangeSpeed>(trNOOP("Change Speed"), movementCmd);
                  m_movementRouteDialog->setOpen(true);
                }
                if (m_route->list().at(m_selected)->code() == EventCode::Frequency) {
                  m_isNewEntry = false;
                  auto movementCmd = std::dynamic_pointer_cast<MovementFrequencyCommand>(m_route->list().at(m_selected));
                  m_movementRouteDialog = std::make_shared<Dialog_MovementChangeFrequency>(trNOOP("Change Frequency"), movementCmd);
                  m_movementRouteDialog->setOpen(true);
                }
                if (m_route->list().at(m_selected)->code() == EventCode::Change_Image) {
                  m_isNewEntry = false;
                  auto movementCmd = std::dynamic_pointer_cast<MovementChangeImageCommand>(m_route->list().at(m_selected));
                  m_movementRouteDialog = std::make_shared<Dialog_MovementChangeImage>(trNOOP("Image"), movementCmd);
                  m_movementRouteDialog->setOpen(true);
                }
                if (m_route->list().at(m_selected)->code() == EventCode::Change_Opacity) {
                  m_isNewEntry = false;
                  auto movementCmd = std::dynamic_pointer_cast<MovementChangeOpacityCommand>(m_route->list().at(m_selected));
                  m_movementRouteDialog = std::make_shared<Dialog_MovementChangeOpacity>(trNOOP("Opacity"), movementCmd);
                  m_movementRouteDialog->setOpen(true);
                }
                if (m_route->list().at(m_selected)->code() == EventCode::Change_Blend_Mode) {
                  m_isNewEntry = false;
                  auto movementCmd = std::dynamic_pointer_cast<MovementChangeBlendModeCommand>(m_route->list().at(m_selected));
                  m_movementRouteDialog = std::make_shared<Dialog_MovementChangeBlendMode>(trNOOP("Blend"), movementCmd);
                  m_movementRouteDialog->setOpen(true);
                }
                if (m_route->list().at(m_selected)->code() == EventCode::Play_SE_del_Movement) {
                  m_isNewEntry = false;
                  auto movementCmd = std::dynamic_pointer_cast<MovementPlaySECommand>(m_route->list().at(m_selected));
                  m_movementRouteDialog = std::make_shared<Dialog_MovementPlaySE>(trNOOP("Play SE"), movementCmd);
                  m_movementRouteDialog->setOpen(true);
                }
                if (m_route->list().at(m_selected)->code() == EventCode::Script_del_Movement) {
                  m_isNewEntry = false;
                  auto movementCmd = std::dynamic_pointer_cast<MovementScriptCommand>(m_route->list().at(m_selected));
                  m_movementRouteDialog = std::make_shared<Dialog_MovementScript>(trNOOP("Script"), movementCmd);
                  m_movementRouteDialog->setOpen(true);
                }
              }

              if (!ImGui::IsKeyDown(ImGuiKey_LeftShift) && !ImGui::IsKeyDown(ImGuiKey_RightShift)) {
                m_selected = n;
              }

              if (isSelected)
                ImGui::SetItemDefaultFocus();

              if (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) {
                if (m_selected == -1) {
                  m_selected = n;
                }
                if (n < m_selected) {
                  // m_selected = n;
                  if (m_selectedEnd == -1) {
                    m_selectedEnd = m_selected;
                    m_selected = n;
                  } else {
                    m_selected = n;
                  }
                } else {
                  m_selectedEnd = n;
                  // m_selected = m_selected;
                }
              } else {
                m_selectedEnd = -1;
              }
            }
          }
          ImGui::EndTable();
        }
        handleClipboardInteraction();
        if (ImGui::IsKeyPressed((ImGuiKey_Delete)) && m_hasFocus) {
          if (m_route->list().at(m_selected)->code() != EventCode::Event_Dummy) {
            int start = m_selected;
            // int end = m_selectedEnd == -1 ? m_selected + 1 : m_selectedEnd + 1;
            m_route->list().erase(m_route->list().begin() + start, m_route->list().begin() + (start + 1));
          }
        }
        ImGui::TextUnformatted(trNOOP("Options"));
        bool tmp = m_route->repeat();
        if (ImGui::Checkbox(trNOOP("Repeat Movements"), &tmp)) {
          m_route->setRepeat(tmp);
        }
        tmp = m_route->skippable();
        if (ImGui::Checkbox(trNOOP("Skip If Cannot Move"), &tmp)) {
          m_route->setSkippable(tmp);
        }
        tmp = m_route->wait();
        ImGui::BeginDisabled(m_isEventRoute);
        if (ImGui::Checkbox(trNOOP("Wait for Completion"), &tmp)) {
          m_route->setWait(tmp);
        }
        ImGui::EndDisabled();
      }
      ImGui::EndChild();
      ImGui::SameLine();
      ImGui::BeginChild("##movementroute_inner_right_child");
      {
        ImGui::SeparatorText("Movement Commands");
        ImGui::BeginGroup();
        {
          if (ImGui::Button(trNOOP("Move Down"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMoveDownCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Move Left"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMoveLeftCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Move Right"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMoveRightCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Move Up"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMoveUpCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Move Lower Left"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMoveLowerLeftCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Move Lower Right"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMoveLowerRightCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Move Upper Left"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMoveUpperLeftCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Move Upper Right"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMoveUpperRightCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Move at Random"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMoveAtRandomCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Move toward Player"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMoveTowardPlayerCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Move away from Player"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMoveAwayFromPlayerCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("1 Step Forward"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMove1StepForwardCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("1 Step Backward"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementMove1StepBackwardCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Jump..."), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_movementRouteDialog = std::make_shared<Dialog_MovementJump>(trNOOP("Jump"));
            m_movementRouteDialog->setOpen(true);
          }
          if (ImGui::Button(trNOOP("Wait..."), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_movementRouteDialog = std::make_shared<Dialog_MovementWait>(trNOOP("Wait"));
            m_movementRouteDialog->setOpen(true);
          }
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Button(trNOOP("Turn Down"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTurnDownCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Turn Left"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTurnLeftCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Turn Right"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTurnRightCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Turn Up"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTurnUpCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Turn 90° Right"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTurn90DegRightCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Turn 90° Left"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTurn90DegLeftCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Turn 180°"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTurn180DegCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Turn 90° Right or Left"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTurn90DegLeftOrRightCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Turn at Random"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTurnAtRandomCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Turn toward Player"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTurnTowardPlayerCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Turn away from Player"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTurnAwayFromPlayerCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Switch ON..."), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_movementRouteDialog = std::make_shared<Dialog_MovementSwitchON>(trNOOP("Switch ON"));
            m_movementRouteDialog->setOpen(true);
          }
          if (ImGui::Button(trNOOP("Switch OFF..."), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_movementRouteDialog = std::make_shared<Dialog_MovementSwitchOFF>(trNOOP("Switch OFF"));
            m_movementRouteDialog->setOpen(true);
          }
          if (ImGui::Button(trNOOP("Change Speed..."), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_movementRouteDialog = std::make_shared<Dialog_MovementChangeSpeed>(trNOOP("Change Speed"));
            m_movementRouteDialog->setOpen(true);
          }
          if (ImGui::Button(trNOOP("Change Frequency..."), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_movementRouteDialog = std::make_shared<Dialog_MovementChangeFrequency>(trNOOP("Change Frequency"));
            m_movementRouteDialog->setOpen(true);
          }
          ImGui::EndGroup();
        }
        ImGui::SameLine();
        ImGui::BeginGroup();
        {
          if (ImGui::Button(trNOOP("Walking Animation ON"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementWalkingAnimationONCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Walking Animation OFF"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementWalkingAnimationOFFCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Stepping Animation ON"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementSteppingAnimationONCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Stepping Animation OFF"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementSteppingAnimationOFFCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Direction Fix ON"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementDirectionFixONCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Direction Fix OFF"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementDirectionFixOFFCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Through ON"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementThroughONCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Through OFF"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementThroughOFFCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Transparent ON"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTransparentONCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Transparent OFF"), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_selected = m_route->addCommand(std::make_shared<MovementTransparentOFFCommand>(), m_selected);
          }
          if (ImGui::Button(trNOOP("Change Image..."), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_movementRouteDialog = std::make_shared<Dialog_MovementChangeImage>(trNOOP("Image"));
            m_movementRouteDialog->setOpen(true);
          }
          if (ImGui::Button(trNOOP("Change Opacity..."), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_movementRouteDialog = std::make_shared<Dialog_MovementChangeOpacity>(trNOOP("Opacity"));
            m_movementRouteDialog->setOpen(true);
          }
          if (ImGui::Button(trNOOP("Change Blend Mode..."), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_movementRouteDialog = std::make_shared<Dialog_MovementChangeBlendMode>(trNOOP("Blend"));
            m_movementRouteDialog->setOpen(true);
          }
          if (ImGui::Button(trNOOP("Play SE..."), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_movementRouteDialog = std::make_shared<Dialog_MovementPlaySE>(trNOOP("Play SE"));
            m_movementRouteDialog->setOpen(true);
          }
          if (ImGui::Button(trNOOP("Script..."), ImVec2{(ImGui::GetContentRegionMax().x / 3) - ImGui::GetStyle().FramePadding.x, 0})) {
            m_movementRouteDialog = std::make_shared<Dialog_MovementScript>(trNOOP("Script"));
            m_movementRouteDialog->setOpen(true);
          }
          ImGui::EndGroup();
        }
      }
      ImGui::EndChild();
    }
    ImGui::EndChild();
    ImGui::BeginChild("##movementroute_bottom_child");
    {
      ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - (buttonsSize.x + (ImGui::GetStyle().FramePadding.x * 3) + ImGui::GetStyle().ItemSpacing.x));
      if (ImGui::Button(trNOOP("OK"))) {
        m_confirmed = true;
        if (m_isEventRoute) {
          m_page->setMoveRoute(*m_route);
        } else {
          m_command->character = m_character;
          // m_command->route = m_route;
        }
        ImGui::CloseCurrentPopup();
        setOpen(false);
      }
      ImGui::SameLine();
      if (ImGui::Button(trNOOP("Cancel"))) {
        ImGui::CloseCurrentPopup();
        setOpen(false);
      }
    }
    ImGui::EndChild();

    if (m_movementRouteDialog) {
      if (const auto [closed, confirmed] = m_movementRouteDialog->draw(); closed) {
        if (confirmed) {
          if (m_isNewEntry) {
            m_selected = m_route->addCommand(m_movementRouteDialog->getCommand(), m_selected);
          } else {
            m_route->list().at(m_selected) = m_movementRouteDialog->getCommand();
          }
          m_isNewEntry = true;
        }
        m_movementRouteDialog.reset();
      }
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
void Dialog_SetMovementRoute::handleClipboardInteraction() const {
  if (!m_hasFocus) {
    return;
  }
  if (OREMovementCommand == -1) {
    OREMovementCommand = clip::register_format("application/ore-MovementCommand");
  }
  if (ImGui::IsKeyPressed(ImGuiKey_V) && (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))) {

    clip::lock l;
    if (l.is_convertible(OREMovementCommand)) {
      auto len = l.get_data_length(OREMovementCommand);
      std::string cmd;
      cmd.resize(len);
      if (l.get_data(OREMovementCommand, cmd.data(), len)) {
        nlohmann::ordered_json cmdJson = nlohmann::ordered_json::parse(cmd);
        CommandParser parser;
        auto commands = parser.parse(cmdJson);
        int curIndent = m_selected > 0 ? *m_route->list().at(m_selected)->indent() : 0;
        int nestedCount = 0;

        for (auto& command : commands) {
          if (!command->indent()) {
            command->setIndent(0);
          }

          command->setIndent(curIndent);
        }
        m_route->list().insert(m_route->list().begin() + m_selected, commands.begin(), commands.end());
      }
    }

  } else if (ImGui::IsKeyPressed(ImGuiKey_C) && (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))) {
    clip::lock l;
    int start = m_selected;
    int end = m_selectedEnd == -1 ? m_selected + 1 : m_selectedEnd + 1;
    if (m_route->list().at(end - 1)->code() == EventCode::Event_Dummy) {
      end -= 1; // We don't want to copy or cut a dummy
    }
    std::vector<std::shared_ptr<IEventCommand>> commands(m_route->list().begin() + start, m_route->list().begin() + end);
    nlohmann::ordered_json cmdJson;
    CommandParser::serialize(cmdJson, commands);
    auto v = cmdJson.dump();
    l.set_data(OREMovementCommand, v.data(), v.size());
  } else if (ImGui::IsKeyPressed(ImGuiKey_X) && (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl))) {
    if (m_route->list().at(m_selected)->code() != EventCode::Event_Dummy) {
      clip::lock l;
      int start = m_selected;
      int end = m_selectedEnd == -1 ? m_selected + 1 : m_selectedEnd + 1;
      if (m_route->list().at(end - 1)->code() == EventCode::Event_Dummy) {
        end -= 1; // We don't want to copy or cut a dummy
      }
      std::vector<std::shared_ptr<IEventCommand>> commands(m_route->list().begin() + start, m_route->list().begin() + end);
      nlohmann::ordered_json cmdJson;
      CommandParser::serialize(cmdJson, commands);
      auto v = cmdJson.dump();
      l.set_data(OREMovementCommand, v.data(), v.size());
      m_route->list().erase(m_route->list().begin() + start, m_route->list().begin() + end);
    }
  }
}