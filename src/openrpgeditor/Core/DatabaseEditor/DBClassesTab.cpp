#include "Core/DatabaseEditor/DBClassesTab.hpp"

#include "Core/Application.hpp"

#include "Core/ImGuiExt/ImGuiParsedText.hpp"
#include "Core/ImGuiExt/ImGuiUtils.hpp"
#include "Database/Class.hpp"
#include "Database/RPGEquations.hpp"
#include "imgui_internal.h"
#include "implot.h"

#include <format>
#include <imgui.h>

constexpr ImVec2 ParameterGraphSize{128, 128};

DBClassesTab::DBClassesTab(Classes& classes, DatabaseEditor* parent)
: IDBEditorTab(parent)
, m_classes(classes) {
  m_selectedClass = m_classes.classType(1);
  if (m_selectedClass) {
    m_traitsEditor.setTraits(&m_selectedClass->traits());
  }
}

void DBClassesTab::drawExperienceGraph(const ExperienceGraphMode mode) const {
  if (!m_selectedClass) {
    return;
  }

  std::array<uint32_t, 99> levels;
  std::array<uint32_t, 99> totalExp;
  std::array<uint32_t, 99> nextExp;
  for (int i = 0; i < 99; ++i) {
    const int level = i + 1;
    levels[i] = level;
    totalExp[i] = rpg::equations::experienceForLevel(level, m_expWorkValues[0], m_expWorkValues[1], m_expWorkValues[2], m_expWorkValues[3]);
    if (level < 99) {
      nextExp[i] = rpg::equations::experienceForLevel(level + 1, m_expWorkValues[0], m_expWorkValues[1], m_expWorkValues[2], m_expWorkValues[3]) - totalExp[i];
    }
  }

  ImGui::BeginGroup();
  {
    const auto& array = mode == ExperienceGraphMode::Total ? totalExp : nextExp;
    const int maxLoops = (mode == ExperienceGraphMode::Total ? 19 : 18);
    constexpr std::string_view red = "&push-color=255,0,0;";
    constexpr std::string_view green = "&push-color=0,255,0;";
    const std::string_view color = mode == ExperienceGraphMode::Total ? red : green;
    ImGui::PushFont(App::APP->getMonoFont());
    for (int i = 0; i < 20; ++i) {
      std::string labelText = std::format("L{:2}: {}{:7}&pop-color; L{:2}: {}{:7}&pop-color; L{:2}: {}{:7}&pop-color; L{:2}: {}{:7}&pop-color;", levels[i + 0], color, array[i + 0], levels[i + 20],
                                          color, array[i + 20], levels[i + 40], color, array[i + 40], levels[i + 60], color, array[i + 60]);

      if (i < maxLoops) {
        labelText += std::format(" L{:2}: {}{:7}&pop-color;", levels[i + 80], color, array[i + 80]);
      }
      ImGui::TextParsed("%s", labelText.c_str());
    }
    ImGui::PopFont();
  }
  ImGui::EndGroup();

  if (ImPlot::BeginPlot("EXP Curve", ImVec2{-1, 0}, ImPlotFlags_CanvasOnly)) {
    ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
    ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoDecorations);
    ImPlot::SetupAxesLimits(0, mode == ExperienceGraphMode::Next ? 98 : 99, 0, mode == ExperienceGraphMode::Next ? 180000 : 7800000, ImPlotCond_Always);
    if (mode == ExperienceGraphMode::Next) {
      ImPlot::PlotShaded("##Next", levels.data(), nextExp.data(), 98);
    } else {
      ImPlot::PlotShaded("##Total", levels.data(), totalExp.data(), 99);
    }
    ImPlot::EndPlot();
  }
}

void DBClassesTab::drawExpPopup() {
  ImGui::SetNextWindowSize(ImVec2{720, 878}, ImGuiCond_Once);
  if (ImGui::BeginPopupModal("EXP Curve##curve_dialog", nullptr,
                             ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize |
                                 ImGuiWindowFlags_NoScrollbar)) {
    if (ImGui::BeginTabBar("##exp_graph_tabbar")) {
      if (ImGui::BeginTabItem("To Next Level")) {
        drawExperienceGraph(ExperienceGraphMode::Next);
        ImGui::EndTabItem();
      }
      if (ImGui::BeginTabItem("Total")) {
        drawExperienceGraph(ExperienceGraphMode::Total);
        ImGui::EndTabItem();
      }
      ImGui::EndTabBar();
    }
    float cursorY = ImGui::GetCursorPosY();
    ImGui::BeginGroup();
    {
      ImGui::BeginGroup();
      {
        ImGui::Text("Base Value:");
        ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - ImGui::GetStyle().FramePadding.x);
        ImGui::SliderInt("##exp_base_value_drag", &m_expWorkValues[0], 10, 50);
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        ImGui::Text("Acceleration A:");
        ImGui::SetNextItemWidth((ImGui::GetContentRegionMax().x / 2) - ImGui::GetStyle().FramePadding.x);
        ImGui::SliderInt("##exp_accel_a_value_drag", &m_expWorkValues[2], 10, 50);
      }
      ImGui::EndGroup();
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::SetCursorPosY(cursorY - 4);
    ImGui::BeginGroup();
    {
      ImGui::BeginGroup();
      {
        ImGui::Text("Extra Value:");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
        ImGui::SliderInt("##exp_extra_value_drag", &m_expWorkValues[1], 0, 40);
      }
      ImGui::EndGroup();
      ImGui::BeginGroup();
      {
        ImGui::Text("Acceleration B:");
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - ImGui::GetStyle().FramePadding.x);
        ImGui::SliderInt("##exp_accel_b_value_drag", &m_expWorkValues[3], 10, 50);
      }
      ImGui::EndGroup();
    }
    ImGui::EndGroup();
    ImGui::Separator();
    ImGui::BeginGroup();
    {
      if (ImGui::Button("OK")) {
        m_selectedClass->setExpParams(m_expWorkValues);
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        ImGui::CloseCurrentPopup();
      }
    }
    ImGui::EndGroup();
    ImGui::EndPopup();
  }
}
void DBClassesTab::draw() {
  ImGui::BeginChild("#orpg_classes_editor");
  {
    const auto calc = ImGui::CalcTextSize("ABCDEFGHIJKLMNOPQRSTUV");
    ImGui::BeginChild("##orpg_classes_editor_classes", ImVec2{calc.x + (ImGui::GetStyle().ItemSpacing.x * 2), 0}, ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX, ImGuiWindowFlags_NoScrollbar);
    {
      ImGui::BeginGroup();
      {
        ImGui::SeparatorText("Classes");
        ImGui::BeginChild("##orpg_classes_editor_class_list", ImVec2{0, ImGui::GetContentRegionMax().y - ((calc.y + ImGui::GetStyle().ItemSpacing.y) * 4)});
        {
          ImGui::BeginGroup();
          {
            for (auto& class_ : m_classes.classes()) {
              if (class_.id() == 0) {
                continue;
              }

              if (ImGui::Selectable(Database::instance()->classNameAndId(class_.id()).c_str(), &class_ == m_selectedClass) || (ImGui::IsItemFocused() && m_selectedClass != &class_)) {

                m_selectedClass = &class_;
                m_traitsEditor.setTraits(&m_selectedClass->traits());
              }
            }
          }
          ImGui::EndGroup();
        }
        ImGui::EndChild();
        char str[4096];
        snprintf(str, 4096, "Max Classes %i", m_classes.count());
        ImGui::SeparatorText(str);
        if (ImGui::Button("Change Max", ImVec2{ImGui::GetContentRegionMax().x - (8), 0})) {
          m_changeIntDialogOpen = true;
          m_editMaxClasses = m_classes.count();
        }
      }
      ImGui::EndGroup();
    }
    ImGui::EndChild();
    ImGui::SameLine();
    ImGui::BeginChild("##orpg_classes_editor_classes_class_properties");
    {
      if (m_selectedClass) {
        ImGui::BeginChild("##orpg_classes_class_panel_left", ImVec2{ImGui::GetContentRegionMax().x / 2, 0.f}, ImGuiChildFlags_Border | ImGuiChildFlags_ResizeX, ImGuiWindowFlags_HorizontalScrollbar);
        {
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("General Settings");
            char name[4096];
            strncpy(name, m_selectedClass->name().c_str(), 4096);
            if (ImGui::LabelOverLineEdit("##orpg_classes_editor_actors_actor_name", "Name:", name, 4096, (ImGui::GetContentRegionMax().x / 2) - 16)) {
              m_selectedClass->setName(name);
            }
            ImGui::SameLine();
            ImGui::BeginGroup();
            {
              ImGui::Text("EXP Curve:");
              char curvePreview[4096];
              snprintf(curvePreview, 4096, "[%i, %i, %i, %i]##exp_curve_button", m_selectedClass->expParams()[0], m_selectedClass->expParams()[1], m_selectedClass->expParams()[2],
                       m_selectedClass->expParams()[3]);
              if (ImGui::Button(curvePreview)) {
                ImGui::OpenPopup("EXP Curve##curve_dialog");
                m_expWorkValues = m_selectedClass->expParams();
              }
            }
            ImGui::EndGroup();
          }
          ImGui::EndGroup();
          ImGui::BeginChild("##orpg_classes_class_parameter_curves", ImVec2(), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_AlwaysAutoResize, ImGuiWindowFlags_HorizontalScrollbar);
          {
            ImGui::SeparatorText("Parameter Curves");
            if (ImPlot::BeginPlot("Max HP", ParameterGraphSize, ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText)) {
              ImPlot::SetNextFillStyle(ImVec4{.81f, .5f, .37f, 1.f});
              ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
              ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoDecorations);
              ImPlot::SetupAxesLimits(0.f, 99.f, 0.f, 9999.f, ImPlotCond_Always);
              if (ImGui::IsItemClicked() && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {}
              ImPlot::PlotShaded("##MaxHP", m_selectedClass->params()[0].data(), m_selectedClass->params()[0].size());
              ImPlot::EndPlot();
            }
            ImGui::SameLine();
            if (ImPlot::BeginPlot("Max MP", ParameterGraphSize, ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText)) {

              ImPlot::SetNextFillStyle(ImVec4{.31f, .5f, .87f, 1.f});
              ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
              ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoDecorations);
              ImPlot::SetupAxesLimits(0.f, 99.f, 0.f, 9999.f, ImPlotCond_Always);
              if (ImGui::IsItemClicked() && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {}
              ImPlot::PlotShaded("##MaxMP", m_selectedClass->params()[1].data(), m_selectedClass->params()[1].size());
              ImPlot::EndPlot();
            }
            ImGui::SameLine();
            if (ImPlot::BeginPlot("Attack", ParameterGraphSize, ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText)) {
              ImPlot::SetNextFillStyle(ImVec4{.75f, .25f, .37f, 1.f});
              ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
              ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoDecorations);
              ImPlot::SetupAxesLimits(0.f, 99.f, 0.f, 999.f, ImPlotCond_Always);
              if (ImGui::IsItemClicked() && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {}
              ImPlot::PlotShaded("##Attack", m_selectedClass->params()[2].data(), m_selectedClass->params()[2].size());
              ImPlot::EndPlot();
            }
            ImGui::SameLine();
            if (ImPlot::BeginPlot("Defense", ParameterGraphSize, ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText)) {
              ImPlot::SetNextFillStyle(ImVec4{.56f, .69f, .37f, 1.f});
              ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
              ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoDecorations);
              ImPlot::SetupAxesLimits(0.f, 99.f, 0.f, 999.f, ImPlotCond_Always);
              if (ImGui::IsItemClicked() && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {}
              ImPlot::PlotShaded("##Defense", m_selectedClass->params()[3].data(), m_selectedClass->params()[3].size());
              ImPlot::EndPlot();
            }
            if (ImPlot::BeginPlot("M.Attack", ParameterGraphSize, ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText)) {
              ImPlot::SetNextFillStyle(ImVec4{.75f, .31f, .69f, 1.f});
              ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
              ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoDecorations);
              ImPlot::SetupAxesLimits(0.f, 99.f, 0.f, 999.f, ImPlotCond_Always);
              if (ImGui::IsItemClicked() && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {}
              ImPlot::PlotShaded("##M.Attack", m_selectedClass->params()[4].data(), m_selectedClass->params()[4].size());
              ImPlot::EndPlot();
            }
            ImGui::SameLine();
            if (ImPlot::BeginPlot("M.Defense", ParameterGraphSize, ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText)) {
              ImPlot::SetNextFillStyle(ImVec4{.25f, .62f, .25f, 1.f});
              ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
              ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoDecorations);
              ImPlot::SetupAxesLimits(0.f, 99.f, 0.f, 999.f, ImPlotCond_Always);
              if (ImGui::IsItemClicked() && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {}
              ImPlot::PlotShaded("##M.Defense", m_selectedClass->params()[5].data(), m_selectedClass->params()[5].size());
              ImPlot::EndPlot();
            }
            ImGui::SameLine();
            if (ImPlot::BeginPlot("Agility", ParameterGraphSize, ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText)) {
              ImPlot::SetNextFillStyle(ImVec4{.31f, .69f, .87f, 1.f});
              ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
              ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoDecorations);
              ImPlot::SetupAxesLimits(0.f, 99.f, 0.f, 999.f, ImPlotCond_Always);
              if (ImGui::IsItemClicked() && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {}
              ImPlot::PlotShaded("##Agility", m_selectedClass->params()[6].data(), m_selectedClass->params()[6].size());
              ImPlot::EndPlot();
            }
            ImGui::SameLine();
            if (ImPlot::BeginPlot("Luck", ParameterGraphSize, ImPlotFlags_NoLegend | ImPlotFlags_NoMenus | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText)) {
              ImPlot::SetNextFillStyle(ImVec4{.81f, .69f, .25f, 1.f});
              ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoDecorations | ImPlotAxisFlags_AutoFit);
              ImPlot::SetupAxis(ImAxis_Y1, nullptr, ImPlotAxisFlags_NoDecorations);
              ImPlot::SetupAxesLimits(0.f, 99.f, 0.f, 999.f, ImPlotCond_Always);
              if (ImGui::IsItemClicked() && ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {}
              ImPlot::PlotShaded("##Luck", m_selectedClass->params()[7].data(), m_selectedClass->params()[7].size());
              ImPlot::EndPlot();
            }
          }
          ImGui::EndChild();
          ImGui::BeginGroup();
          {
            ImGui::SeparatorText("Skills to Learn");
            if (ImGui::BeginTable("##orpg_class_skills_list", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_ScrollY)) {
              ImGui::TableSetupColumn("Level");
              ImGui::TableSetupColumn("Skill");
              ImGui::TableSetupColumn("Note");
              ImGui::TableHeadersRow();

              for (int i = 0; auto& learning : m_selectedClass->learnings()) {
                const auto skill = Database::instance()->skills.skill(learning.skillId());
                if (!skill) {
                  continue;
                }
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                if (ImGui::Selectable(std::format("Lv {:3}##{}", learning.level(), i).c_str(), false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                  if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                    // open learning skill modal
                  }
                }
                ImGui::TableNextColumn();
                ImGui::Text("%s", skill->name().c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", learning.note().c_str());
                ++i;
              }

              ImGui::TableNextRow();
              if (ImGui::TableNextColumn()) {
                if (ImGui::Selectable("##classes_editor_learning_dummy", false, ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick)) {
                  if (ImGui::GetMouseClickedCount(ImGuiMouseButton_Left) >= 2) {
                    // m_tempTrait = Trait();
                    // m_isNewEntry = true;
                    // m_updateTab = true;
                    // m_tempTraitAssigned = false;
                    // m_selectedTrait = &m_tempTrait;
                    // ImGui::OpenPopup(TraitsEditorPopupId.data());
                  }
                }
              }
              ImGui::EndTable();
            }
          }
          ImGui::EndGroup();
        }
        drawExpPopup();
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::BeginChild("##orpg_classes_class_panel_right");
        {
          ImGui::BeginChild("##traits_editor", {0, ImGui::GetContentRegionAvail().y / 2}, ImGuiChildFlags_ResizeY);
          m_traitsEditor.draw(m_parent);
          ImGui::EndChild();
          ImGui::BeginChild("##note");
          {
            ImGui::SeparatorText("Note:");
            char note[8192];
            strncpy(note, m_selectedClass->note().c_str(), IM_ARRAYSIZE(note));
            if (ImGui::InputTextMultiline("##orpg_classes_note", note, IM_ARRAYSIZE(note), ImVec2{ImGui::GetContentRegionMax().x - 16, ImGui::GetContentRegionAvail().y - 16})) {
              m_selectedClass->setNote(note);
            }
          }
          ImGui::EndChild();
        }
        ImGui::EndChild();
      }
    }
    ImGui::EndChild();
  }
  ImGui::EndChild();

  if (m_changeIntDialogOpen) {
    if (ImGui::Begin("Change Max Classes", &m_changeIntDialogOpen,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
      ImGui::Text(
          "Specify an amount to resize the class list to\n"
          "This cannot be undone!");
      ImGui::InputInt("##value_input", &m_editMaxClasses);
      ImGui::SameLine();
      if (ImGui::Button("Accept")) {
        m_changeConfirmDialogOpen = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("Cancel")) {
        m_changeIntDialogOpen = false;
      }
    }
    ImGui::End();

    if (m_changeConfirmDialogOpen) {
      if (ImGui::Begin("Confirm Change", &m_changeConfirmDialogOpen,
                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking)) {
        ImGui::Text("Are you sure?");
        if (ImGui::Button("Yes")) {
          const int tmpId = m_selectedClass->id();
          m_classes.resize(m_editMaxClasses);
          m_selectedClass = m_classes.classType(tmpId);
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
          m_changeIntDialogOpen = false;
          m_changeConfirmDialogOpen = false;
        }
      }
      ImGui::End();
    }
  }
}
