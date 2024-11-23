#include "Core/FirstBootWizard.hpp"

#include "DPIHandler.hpp"
#include "imgui.h"

bool FirstBootWizard::draw() {
  if (m_pages.empty()) {
    return true;
  }

  bool ret = false;

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2{640, 480} * App::DPIHandler::get_ui_scale(), ImGuiCond_Appearing);
  ImGui::Begin((std::string(m_pages[m_currentPage]->pageTitle()) + " - First Boot Wizard").c_str(), nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
  {
    m_pages[m_currentPage]->draw();
    const auto textSize = ImGui::CalcTextSize("ABCDEFGHIJK");
    ImGui::SetCursorPosY(ImGui::GetContentRegionMax().y -
                         (textSize.y + (ImGui::GetStyle().FramePadding.y * 2) + ImGui::GetStyle().ItemSpacing.y));
    ImGui::Separator();
    ImGui::SetCursorPosX(ImGui::GetContentRegionMax().x - (textSize.x + ImGui::GetStyle().FramePadding.x * 2));
    ImGui::BeginDisabled(m_currentPage == 0);
    {
      if (ImGui::Button("Back")) {
        m_currentPage--;
        if (m_currentPage < 0) {
          m_currentPage = 0;
        }
      }
    }
    ImGui::EndDisabled();
    ImGui::SameLine();
    ImGui::BeginDisabled(!m_pages[m_currentPage]->allRequiredFieldsAreSet());
    {
      if (ImGui::Button(m_currentPage < m_pages.size() - 1 ? "Next  " : "Finish") &&
          m_pages[m_currentPage]->allRequiredFieldsAreSet()) {
        m_currentPage++;
        if (m_currentPage >= m_pages.size()) {
          ret = true;
        }
      }
    }
    ImGui::EndDisabled();
  }
  ImGui::End();

  return ret;
}

void FirstBootWizard::addPage(const std::shared_ptr<IWizardPage>& page) { m_pages.push_back(page); }
