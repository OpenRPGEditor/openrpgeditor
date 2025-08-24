#include "Core/FirstBootWizard.hpp"

#include "imgui.h"

bool FirstBootWizard::draw() {
  if (m_pages.empty()) {
    return true;
  }

  bool ret = false;

  const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImGui::GetMainViewport()->Size / 2, ImGuiCond_Once);
  ImGui::Begin((m_pages[m_currentPage]->pageTitle() + tr(" - First Boot Wizard")).c_str(), nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);
  {
    ImGui::BeginVertical("##wizard_main_layout", ImGui::GetContentRegionAvail(), 0);
    {
      m_pages[m_currentPage]->draw();
      ImGui::Spring();
      const auto buttonText = std::vector{tr("Back"), tr("Next"), tr("Finish")};
      ImVec2 textSize; // First get the largest button size
      for (const auto& btn : buttonText) {
        if (const auto sz = ImGui::CalcTextSize(btn.c_str(), nullptr, true); sz.x > textSize.x || sz.y > textSize.y) {
          textSize = sz;
        }
      }
      // Make sure the button size can fit the text
      textSize += ImGui::GetStyle().FramePadding * 2.f;
      ImGui::Separator();
      ImGui::BeginHorizontal("##wizard_button_layout", {-1, 0});
      {
        ImGui::Spring();
        ImGui::BeginDisabled(m_currentPage == 0);
        {
          if (ImGui::Button("Back", {textSize.x + ImGui::GetStyle().FramePadding.x * 2.f, 0})) {
            m_currentPage--;
            if (m_currentPage < 0) {
              m_currentPage = 0;
            }
          }
        }
        ImGui::EndDisabled();
        ImGui::BeginDisabled(!m_pages[m_currentPage]->allRequiredFieldsAreSet());
        {
          if (ImGui::Button(m_currentPage < m_pages.size() - 1 ? "Next" : "Finish", {textSize.x + ImGui::GetStyle().FramePadding.x * 2.f, 0}) && m_pages[m_currentPage]->allRequiredFieldsAreSet()) {
            m_currentPage++;
            if (m_currentPage >= m_pages.size()) {
              ret = true;
            }
          }
        }
        ImGui::EndDisabled();
      }
      ImGui::EndHorizontal();
    }
    ImGui::EndVertical();
  }
  ImGui::End();

  return ret;
}

void FirstBootWizard::addPage(const std::shared_ptr<IWizardPage>& page) { m_pages.push_back(page); }
