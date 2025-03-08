#include "Core/EventCommands/Dialog_SetMovementRoute.hpp"

#include "Core/EventCommands/Dialog_PlaySE.hpp"
#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeBlendMode.hpp"
#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeFrequency.hpp"
#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeImage.hpp"
#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeOpacity.hpp"
#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeSpeed.hpp"
#include "Core/EventCommands/MovementRoute/Dialog_MovementJump.hpp"
#include "Core/EventCommands/MovementRoute/Dialog_MovementScript.hpp"
#include "Core/EventCommands/MovementRoute/Dialog_MovementSwitchOFF.hpp"
#include "Core/EventCommands/MovementRoute/Dialog_MovementSwitchON.hpp"
#include "Core/EventCommands/MovementRoute/Dialog_MovementWait.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"

#include "Database/Database.hpp"
#include "Database/EventCommands/MovementRoute/DirectionFixOFF.hpp"
#include "Database/EventCommands/MovementRoute/DirectionFixON.hpp"
// #include "Database/EventCommands/MovementRoute/Jump.hpp"
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
// #include "Database/EventCommands/MovementRoute/Wait.hpp"
#include "Database/EventCommands/MovementRoute/WalkingAnimationOFF.hpp"
#include "Database/EventCommands/MovementRoute/WalkingAnimationON.hpp"
#include "imgui.h"
#include <tuple>

std::tuple<bool, bool> Dialog_SetMovementRoute::draw() {
  if (isOpen()) {
    ImGui::OpenPopup(m_name.c_str());
  }
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{818, 395}, ImGuiCond_Appearing);
  if (ImGui::BeginPopupModal(m_name.c_str(), &m_open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize)) {

    // Character Selection
    const ImVec2 cursorPos = ImGui::GetCursorPos();
    ImGui::PushItemWidth(160);
    ImGui::BeginDisabled(m_isEventRoute);
    if (ImGui::BeginCombo("##showroute_character", m_isEventRoute ? "" : Database::instance()->eventNameOrId(m_character).c_str())) {

      if (ImGui::Selectable("Player", m_character == -1)) {
        m_character = -1;
        ImGui::SetItemDefaultFocus();
      }
      if (ImGui::Selectable("This Event", m_character == 0)) {
        m_character = 0;
        ImGui::SetItemDefaultFocus();
      }

      for (auto& dataSource : Database::instance()->mapInfos.currentMap()->map()->events()) {
        if (!dataSource.has_value())
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

    // Step Table
    auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNO");
    calc.y *= 16;
    calc.y += ImGui::GetStyle().ItemSpacing.y * 32;
    if (ImGui::BeginTable("##bgm_audio_contents", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY, calc)) {

      ImGui::TableSetupScrollFreeze(1, 0);
      ImGui::TableSetupColumn("File");
      ImGui::TableNextRow();
      ImGui::TableNextColumn();

      for (int n = 0; n < m_route.list().size(); n++) {
        ImGui::TableNextColumn();
        const bool isSelected = m_selected == n;
        char text[4096];
        sprintf(text, "%s##cmd%i", m_route.list().at(n)->stringRep(*Database::instance()).c_str(), n);
        if (ImGui::SelectableWithBorder(text, isSelected, ImGuiSelectableFlags_AllowOverlap | ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
          if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
            // Edit node
          }
          m_selected = n;
          if (isSelected)
            ImGui::SetItemDefaultFocus();
        }
      }
      ImGui::EndTable();
    }
    ImGui::Text("Options");
    bool tmp = m_route.repeat();
    if (ImGui::Checkbox("Repeat Movements", &tmp)) {
      m_route.setRepeat(tmp);
    }
    tmp = m_route.skippable();
    if (ImGui::Checkbox("Skip If Cannot Move", &tmp)) {
      m_route.setSkippable(tmp);
    }
    tmp = m_route.wait();
    ImGui::BeginDisabled(m_isEventRoute);
    if (ImGui::Checkbox("Wait for Completion", &tmp)) {
      m_route.setWait(tmp);
    }
    ImGui::EndDisabled();
    ImGui::SetCursorPos(ImVec2{cursorPos.x + 180, cursorPos.y - 10});
    ImGui::SeparatorText("Movement Commands");
    ImGui::SetCursorPos(ImVec2{cursorPos.x + 180, cursorPos.y + 10});
    ImGui::BeginGroup();
    {
      if (ImGui::Button("Move Down", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMoveDownCommand>(), m_selected);
      }
      if (ImGui::Button("Move Left", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMoveLeftCommand>(), m_selected);
      }
      if (ImGui::Button("Move Right", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMoveRightCommand>(), m_selected);
      }
      if (ImGui::Button("Move Up", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMoveUpCommand>(), m_selected);
      }
      if (ImGui::Button("Move Lower Left", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMoveLowerLeftCommand>(), m_selected);
      }
      if (ImGui::Button("Move Lower Right", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMoveLowerRightCommand>(), m_selected);
      }
      if (ImGui::Button("Move Upper Left", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMoveUpperLeftCommand>(), m_selected);
      }
      if (ImGui::Button("Move Upper Right", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMoveUpperRightCommand>(), m_selected);
      }
      if (ImGui::Button("Move at Random", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMoveAtRandomCommand>(), m_selected);
      }
      if (ImGui::Button("Move toward Player", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMoveTowardPlayerCommand>(), m_selected);
      }
      if (ImGui::Button("Move away from Player", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMoveAwayFromPlayerCommand>(), m_selected);
      }
      if (ImGui::Button("1 Step Forward", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMove1StepForwardCommand>(), m_selected);
      }
      if (ImGui::Button("1 Step Backward", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementMove1StepBackwardCommand>(), m_selected);
      }
      if (ImGui::Button("Jump...", ImVec2{200, 0})) {
        movementRouteDialog = std::make_shared<Dialog_MovementJump>("Jump");
        movementRouteDialog->setOpen(true);
      }
      if (ImGui::Button("Wait...", ImVec2{200, 0})) {
        movementRouteDialog = std::make_shared<Dialog_MovementWait>("Wait");
        movementRouteDialog->setOpen(true);
      }
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("Turn Down", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTurnDownCommand>(), m_selected);
      }
      if (ImGui::Button("Turn Left", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTurnLeftCommand>(), m_selected);
      }
      if (ImGui::Button("Turn Right", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTurnRightCommand>(), m_selected);
      }
      if (ImGui::Button("Turn Up", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTurnUpCommand>(), m_selected);
      }
      if (ImGui::Button("Turn 90° Right", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTurn90DegRightCommand>(), m_selected);
      }
      if (ImGui::Button("Turn 90° Left", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTurn90DegLeftCommand>(), m_selected);
      }
      if (ImGui::Button("Turn 180°", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTurn180DegCommand>(), m_selected);
      }
      if (ImGui::Button("Turn 90° Right or Left", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTurn90DegLeftOrRightCommand>(), m_selected);
      }
      if (ImGui::Button("Turn at Random", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTurnAtRandomCommand>(), m_selected);
      }
      if (ImGui::Button("Turn toward Player", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTurnTowardPlayerCommand>(), m_selected);
      }
      if (ImGui::Button("Turn away from Player", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTurnAwayFromPlayerCommand>(), m_selected);
      }
      if (ImGui::Button("Switch ON...", ImVec2{200, 0})) {
        movementRouteDialog = std::make_shared<Dialog_MovementSwitchON>("Script");
        movementRouteDialog->setOpen(true);
      }
      if (ImGui::Button("Switch OFF...", ImVec2{200, 0})) {
        movementRouteDialog = std::make_shared<Dialog_MovementSwitchOFF>("Script");
        movementRouteDialog->setOpen(true);
      }
      if (ImGui::Button("Change Speed...", ImVec2{200, 0})) {
        movementRouteDialog = std::make_shared<Dialog_MovementChangeSpeed>("Script");
        movementRouteDialog->setOpen(true);
      }
      if (ImGui::Button("Change Frequency...", ImVec2{200, 0})) {
        movementRouteDialog = std::make_shared<Dialog_MovementChangeFrequency>("Script");
        movementRouteDialog->setOpen(true);
      }
      ImGui::EndGroup();
    }
    ImGui::SameLine();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("Walking Animation ON", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementWalkingAnimationONCommand>(), m_selected);
      }
      if (ImGui::Button("Walking Animation OFF", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementWalkingAnimationOFFCommand>(), m_selected);
      }
      if (ImGui::Button("Stepping Animation ON", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementSteppingAnimationONCommand>(), m_selected);
      }
      if (ImGui::Button("Stepping Animation OFF", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementSteppingAnimationOFFCommand>(), m_selected);
      }
      if (ImGui::Button("Direction Fix ON", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementDirectionFixONCommand>(), m_selected);
      }
      if (ImGui::Button("Direction Fix OFF", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementDirectionFixOFFCommand>(), m_selected);
      }
      if (ImGui::Button("Through ON", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementThroughONCommand>(), m_selected);
      }
      if (ImGui::Button("Through OFF", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementThroughOFFCommand>(), m_selected);
      }
      if (ImGui::Button("Transparent ON", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTransparentONCommand>(), m_selected);
      }
      if (ImGui::Button("Transparent OFF", ImVec2{200, 0})) {
        m_selected = m_route.addCommand(std::make_shared<MovementTransparentOFFCommand>(), m_selected);
      }
      if (ImGui::Button("Change Image...", ImVec2{200, 0})) {
        movementRouteDialog = std::make_shared<Dialog_MovementChangeImage>("Image");
        movementRouteDialog->setOpen(true);
      }
      if (ImGui::Button("Change Opacity...", ImVec2{200, 0})) {
        movementRouteDialog = std::make_shared<Dialog_MovementChangeOpacity>("Opacity");
        movementRouteDialog->setOpen(true);
      }
      if (ImGui::Button("Change Blend Mode...", ImVec2{200, 0})) {
        movementRouteDialog = std::make_shared<Dialog_MovementChangeBlendMode>("Blend");
        movementRouteDialog->setOpen(true);
      }
      if (ImGui::Button("Play SE...", ImVec2{200, 0})) {
        movementRouteDialog = std::make_shared<Dialog_PlaySE>("Play SE");
        movementRouteDialog->setOpen(true);
      }
      if (ImGui::Button("Script...", ImVec2{200, 0})) {
        movementRouteDialog = std::make_shared<Dialog_MovementScript>("Script");
        movementRouteDialog->setOpen(true);
      }
      ImGui::EndGroup();
    }
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 712);
    if (ImGui::Button("OK")) {
      m_confirmed = true;
      if (m_isEventRoute) {
        m_page.setMoveRoute(m_route);
      } else {
        command->character = m_character;
        command->route = m_route;
        for (const auto& cmd : m_route.list()) {
          if (cmd->code() != EventCode::Event_Dummy) {
            command->editNodes.push_back(std::make_shared<MovementRouteStepCommand>());
            command->editNodes.back()->step = cmd;
          }
        }
      }
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel")) {
      ImGui::CloseCurrentPopup();
      setOpen(false);
    }

    if (movementRouteDialog) {
      if (const auto [closed, confirmed] = movementRouteDialog->draw(); closed) {
        if (confirmed) {
          m_selected = m_route.addCommand(movementRouteDialog->getCommand(), m_selected);
        }
        movementRouteDialog.reset();
      }
    }
    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
