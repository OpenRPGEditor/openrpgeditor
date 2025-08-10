#include "Core/EventCommands/MovementRoute/Dialog_MovementChangeImage.hpp"
#include "Core/Application.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_MovementChangeImage::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###MovementChangeImage");
  }
  const auto imageSize = ImGui::GetDPIScaledSize(FaceSheet::faceWidth(), FaceSheet::faceHeight());
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(imageSize.x * 2) + (ImGui::GetStyle().FramePadding.x * 2), imageSize.y + (ImGui::GetFrameHeightWithSpacing() * 4) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});

  m_actorButton.setSize(imageSize.x, imageSize.y);

  if (!m_characterSheet && !m_image.empty() && !m_actorButton.hasCompositeTextures()) {
    m_characterSheet = CharacterSheet(m_image);
    auto [min, max] = m_characterSheet.value().getRectForCharacter(m_character, 1);

    m_actorButton.setTexturesToComposite({
        {
            m_characterSheet->texture(),
            {m_characterSheet->characterWidth(), m_characterSheet->characterHeight()},
            {static_cast<int>(min.x() * m_characterSheet->texture().width()), static_cast<int>(min.y() * m_characterSheet->texture().height())},
        },
    });
  }

  if (ImGui::BeginPopupModal(std::format("{}###MovementChangeImage", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    drawPickers();
    ImGui::BeginVertical("##movement_change_image_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox imageGroup(trNOOP("Image"), "##movement_change_image_image_group", {-1, 0});
      if (imageGroup.begin()) {
        ImGui::BeginHorizontal("##movement_route_change_image_image_layout", ImGui::GetContentRegionAvail(), 0);
        {
          ImGui::Spring(0.5f);
          if (ImGui::ImageButtonEx(ImGui::GetID("##event_image"), m_actorButton.get(), m_actorButton.size(), {0.f, 0.f}, {1.f, 1.f}, {}, {1.f, 1.f, 1.f, 1.f}, ImGuiButtonFlags_PressedOnDoubleClick)) {
            m_characterPicker.setCharacterInfo(m_image, m_character);
            m_characterPicker.setOpen(true);
          }
          ImGui::Spring(0.5f);
        }
        ImGui::EndHorizontal();
      }
      imageGroup.end();

      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##buttons_character_picker", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##ok_cancel_character_picker", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->image = m_image;
          m_command->character = m_character;
          ImGui::CloseCurrentPopup();
          setOpen(false);
        } else if (ret == 1) {
          ImGui::CloseCurrentPopup();
          setOpen(false);
        }
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();

    ImGui::EndPopup();
  }

  return std::make_tuple(!m_open, m_confirmed);
}
void Dialog_MovementChangeImage::drawPickers() {
  if (const auto [closed, confirmed] = m_characterPicker.draw(); closed) {
    if (confirmed) {
      m_characterPicker.accept();
      m_character = m_characterPicker.character();
      m_image = m_characterPicker.selectedSheet();
      m_characterSheet.reset();
      m_actorButton.clear();
    }
  }
}