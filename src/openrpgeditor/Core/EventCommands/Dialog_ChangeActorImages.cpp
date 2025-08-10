#include "Core/EventCommands/Dialog_ChangeActorImages.hpp"
#include "Core/Application.hpp"

#include "Core/CommonUI/GroupBox.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Database.hpp"
#include <imgui.h>
#include <imgui_internal.h>

#include <tuple>

std::tuple<bool, bool> Dialog_ChangeActorImages::draw() {
  if (isOpen()) {
    ImGui::OpenPopup("###ChangeActorImages");
  }
  const auto imageSize = ImGui::GetDPIScaledSize(FaceSheet::faceWidth(), FaceSheet::faceHeight());
  ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  const auto maxSize = ImVec2{(imageSize.x * 4) + (ImGui::GetStyle().FramePadding.x * 2), imageSize.y + (ImGui::GetFrameHeightWithSpacing() * 8) + (ImGui::GetStyle().FramePadding.y * 2)};
  ImGui::SetNextWindowSize(maxSize, ImGuiCond_Appearing);
  ImGui::SetNextWindowSizeConstraints(maxSize, {FLT_MAX, FLT_MAX});
  m_faceButton.setSize(imageSize.x, imageSize.y);
  m_characterButton.setSize(imageSize.x, imageSize.y);
  m_battlerButton.setSize(imageSize.x, imageSize.y);

  if (ImGui::BeginPopupModal(std::format("{}###ChangeActorImages", m_name).c_str(), &m_open, ImGuiWindowFlags_NoResize)) {
    drawPickers();
    if (!m_faceSheet && !m_facePicture.empty() && !m_faceButton.hasCompositeTextures()) {
      m_faceSheet.emplace(m_facePicture);
      auto offset = m_faceSheet->getFaceRect(m_faceIndex).min;
      offset.xr() *= m_faceSheet->texture().width();
      offset.yr() *= m_faceSheet->texture().height();
      m_faceButton.setTexturesToComposite({
          {
              m_faceSheet->texture(),
              Point{m_faceSheet->faceWidth(), m_faceSheet->faceHeight()},
              offset.toPoint(),
              true,
          },
      });
    }

    if (!m_characterSheet && !m_charPicture.empty() && !m_characterButton.hasCompositeTextures()) {
      m_characterSheet.emplace(m_charPicture);
      const auto [min, max] = m_characterSheet->getRectForCharacter(m_charIndex, 1);
      m_characterButton.setTexturesToComposite({{m_characterSheet->texture(),
                                                 {m_characterSheet->characterWidth(), m_characterSheet->characterHeight()},
                                                 {static_cast<int>(min.x() * m_characterSheet->texture().width()), static_cast<int>(min.y() * m_characterSheet->texture().height())}}});
    }
    if (!m_battlerSheet && !m_battlerPicture.empty() && !m_battlerButton.hasCompositeTextures()) {
      m_battlerSheet.emplace(m_battlerPicture);
      const auto [_, frames] = m_battlerSheet->getAction(SideViewActionType::StepForward);
      m_battlerButton.setTexturesToComposite({{m_battlerSheet->texture(),
                                               {m_battlerSheet->characterWidth(), m_battlerSheet->characterHeight()},
                                               {static_cast<int>(frames[1].min.x() * m_battlerSheet->texture().width()), static_cast<int>(frames[1].min.y() * m_battlerSheet->texture().height())}}});
    }

    ImGui::BeginVertical("##change_actor_images_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      GroupBox actorGroupBox(trNOOP("Actor"), "##change_actor_images_actor_group", {-1, 0});
      if (actorGroupBox.begin()) {
        if (ImGui::EllipsesButton(std::format("{}##change_actor_iamges_actor", Database::instance()->actorNameAndId(m_actor)).c_str(), {-1, 0})) {
          m_actorPicker = ObjectPicker(trNOOP("Actors"), Database::instance()->actors.actorList(), m_actor);
          m_actorPicker->setOpen(true);
        }
      }
      actorGroupBox.end();
      GroupBox imagesGroup(trNOOP("Images"), "##change_actor_images_images_group", {-1, 0});
      if (imagesGroup.begin()) {
        ImGui::BeginHorizontal("##change_actor_images_images_layout", {-1, 0}, 0);
        {
          GroupBox faceGroup(trNOOP("Face"), "##change_actor_images_face_group");
          if (faceGroup.begin()) {
            if (ImGui::ImageButtonEx(ImGui::GetID("##face_image"), m_faceButton.get(), imageSize, {0.f, 0.f}, {1.f, 1.f}, {0.f, 0.f, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f},
                                     ImGuiButtonFlags_PressedOnDoubleClick)) {
              m_facePicker.setFaceInfo(m_facePicture, m_faceIndex);
              m_facePicker.setOpen(true);
            }
          }
          faceGroup.end();
          GroupBox characterGroup(trNOOP("Character"), "##change_actor_images_character_group");
          if (characterGroup.begin()) {
            if (ImGui::ImageButtonEx(ImGui::GetID("##char_image"), m_characterButton.get(), imageSize, {0.f, 0.f}, {1.f, 1.f}, {0.f, 0.f, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f},
                                     ImGuiButtonFlags_PressedOnDoubleClick)) {
              m_characterPicker.setCharacterInfo(m_facePicture, m_charIndex);
              m_characterPicker.setOpen(true);
            }
          }
          characterGroup.end();
          GroupBox battlerGroup(trNOOP("[SV] Battler"), "##change_actor_images_battler_group");
          if (battlerGroup.begin()) {
            if (ImGui::ImageButtonEx(ImGui::GetID("##battler_image"), m_battlerButton.get(), imageSize, {0.f, 0.f}, {1.f, 1.f}, {0.f, 0.f, 0.f, 0.f}, {1.f, 1.f, 1.f, 1.f},
                                     ImGuiButtonFlags_PressedOnDoubleClick)) {
              m_battlerPicker.setImageInfo(m_battlerPicture);
              m_battlerPicker.setOpen(true);
            }
          }
          battlerGroup.end();
        }
        ImGui::EndHorizontal();
      }
      imagesGroup.end();
      ImGui::Spring();
      ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal, ImGui::GetDPIScaledValue(1.5f));
      ImGui::BeginHorizontal("##change_actor_images_button_layout", {-1, 0}, 0);
      {
        ImGui::Spring();
        if (const auto ret = ImGui::ButtonGroup("##scroll_map_buttons", {trNOOP("OK"), trNOOP("Cancel")}); ret == 0) {
          m_confirmed = true;
          m_command->actor = m_actor;
          m_command->charPicture = m_charPicture;
          m_command->charIndex = m_charIndex;
          m_command->facePicture = m_facePicture;
          m_command->faceIndex = m_faceIndex;
          m_command->battlerPicture = m_battlerPicture;
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

void Dialog_ChangeActorImages::drawPickers() {
  if (const auto [closed, confirmed] = m_facePicker.draw(); closed) {
    if (confirmed) {
      m_facePicker.accept();
      m_faceIndex = m_characterPicker.character();
      m_facePicture = m_characterPicker.selectedSheet();
      m_faceSheet.reset();
      m_faceButton.clear();
    }
  }
  if (const auto [closed, confirmed] = m_characterPicker.draw(); closed) {
    if (confirmed) {
      m_characterPicker.accept();
      if (m_imageSelection == 0) {
        m_charIndex = m_characterPicker.character();
        m_charPicture = m_characterPicker.selectedSheet();
        m_characterSheet.reset();
        m_characterButton.clear();
      }
    }
  }

  if (const auto [closed, confirmed] = m_battlerPicker.draw(); closed) {
    if (confirmed) {
      m_battlerPicker.accept();
      m_battlerPicture = m_battlerPicker.selectedImage();
      m_battlerSheet.reset();
      m_battlerButton.clear();
    }
  }
  if (m_actorPicker) {

    if (const auto [closed, confirmed] = m_actorPicker->draw(); closed) {
      if (confirmed) {
        if (m_actor != m_actorPicker->selection()) {
          m_actor = m_actorPicker->selection();
          const auto actor = Database::instance()->actors.actor(m_actor);
          m_charPicture = actor->characterName();
          m_charIndex = actor->characterIndex();
          m_facePicture = actor->faceName();
          m_faceIndex = actor->faceIndex();
          m_battlerPicture = actor->battlerName();
          m_faceSheet.reset();
          m_faceButton.clear();
          m_characterSheet.reset();
          m_characterButton.clear();
          m_battlerSheet.reset();
          m_battlerButton.clear();
        }
      }
      m_actorPicker.reset();
    }
  }
}