#include "Core/EventCommands/Dialog_ChangeVehicleImage.hpp"
#include "Core/Application.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeVehicleImage::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeVehicleImage");
  }
  const auto imageSize = ImGui::GetDPIScaledSize(FaceSheet::faceWidth(), FaceSheet::faceHeight());
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(imageSize.x * 2) + (ImGui::GetStyle().FramePadding.x * 2), imageSize.y + (ImGui::GetFrameHeightWithSpacing() * 6) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  m_characterButton.setSize(imageSize.x, imageSize.y);
  if (!m_characterSheet && !m_image.empty() && !m_characterButton.hasCompositeTextures()) {
    m_characterSheet.emplace(m_image);
    const auto [min, max] = m_characterSheet->getRectForCharacter(m_character, 1);
    m_characterButton.setTexturesToComposite({
        {
            m_characterSheet->texture(),
            {m_characterSheet->characterWidth(), m_characterSheet->characterHeight()},
            {static_cast<int>(min.x() * m_characterSheet->texture().width()), static_cast<int>(min.y() * m_characterSheet->texture().height())},
        },
    });
  }

  if (ImGui::BeginPopupModal(std::format("{}###ChangeVehicleImage", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    drawPickers();
    ImGui::BeginVertical("##change_vehicle_image_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox vehicleGroup(trNOOP("Vehicle"), "##change_vehicle_image_vehicle_group", {-1, 0});
      if (vehicleGroup.begin()) {
        ImGui::SetNextItemWidth(-1);
        if (ImGui::BeginCombo("##change_vehicle_image_vehicle_selection", DecodeEnumName(static_cast<VehicleType>(m_vehicle)).c_str())) {
          for (auto& vehicle : magic_enum::enum_values<VehicleType>()) {
            bool selected = m_vehicle == static_cast<int>(vehicle);
            if (ImGui::Selectable(DecodeEnumName(vehicle).c_str(), selected)) {
              if (m_vehicle != static_cast<int>(vehicle)) {
                const Vehicle* v = nullptr;
                switch (vehicle) {
                case VehicleType::Boat:
                  v = &Database::instance()->system->boat();
                  break;
                case VehicleType::Airship:
                  v = &Database::instance()->system->airship();
                  break;
                case VehicleType::Ship:
                  v = &Database::instance()->system->ship();
                  break;
                default:;
                }
                
                m_image = v->characterName();
                m_character = v->characterIndex();
                m_characterSheet.reset();
                m_characterButton.clear();
              }
              m_vehicle = static_cast<int>(vehicle);
            }
            if (selected) {
              ImGui::SetItemDefaultFocus();
            }
          }
          ImGui::EndCombo();
        }
      }
      vehicleGroup.end();
      GroupBox imageGroup(trNOOP("Image"), "##change_vehicle_image_image_group", {-1, 0});
      if (imageGroup.begin()) {
        ImGui::BeginHorizontal("##change_vehicle_image_image_layout", {ImGui::GetContentRegionAvail().x, 0}, 0);
        {
          ImGui::Spring(0.5f);
          if (ImGui::ImageButtonEx(ImGui::GetID("##char_image"), m_characterButton.get(), imageSize, {0.f, 0.f}, {1.f, 1.f}, {0.f, 0.f, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f},
                                   ImGuiButtonFlags_PressedOnDoubleClick)) {
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
      ImGui::BeginHorizontal("##change_actor_images_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##scroll_map_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->vehicle = static_cast<VehicleType>(m_vehicle);
          m_command->picture = m_image;
          m_command->pictureIndex = m_character;
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

void Dialog_ChangeVehicleImage::drawPickers() {
  if (const auto [closed, confirmed] = m_characterPicker.draw(); closed) {
    if (confirmed) {
      m_characterPicker.accept();
      m_character = m_characterPicker.character();
      m_image = m_characterPicker.selectedSheet();
      m_characterSheet.reset();
      m_characterButton.clear();
    }
  }
}