#include "Core/ReferenceSearch.hpp"

#include "MainWindow.hpp"
ReferenceSearch::ReferenceSearch(MainWindow* parent) : m_parent(parent) {}
void ReferenceSearch::findAllReferences(int targetId, SearchType type) {
  m_results.clear();
  for (auto& mapInfo : m_parent->database().mapInfos.mapInfos()) {
    if (mapInfo->map()) {
      for (auto& event : mapInfo->map()->events()) {
        if (event) {
          for (const auto& ev : event.value().getConditionReferences(targetId, type)) {
            m_results.emplace_back(mapInfo->id(), ev);
          }
        }
      }
    }
  }
  searchCommonByVariable(targetId, type);
}
void ReferenceSearch::findAllReferences(std::string text, SearchType type) {
  m_list.clear();
  for (auto& mapInfo : m_parent->database().mapInfos.mapInfos()) {
    if (mapInfo->map()) {
      for (auto& event : mapInfo->map()->events()) {
        if (event) {
          for (const auto& ev : event.value().getListReferences(text, type)) {
            m_results.emplace_back(mapInfo->id(), ev);
          }
        }
      }
    }
  }
  searchCommonByVariable(text, type);
}
void ReferenceSearch::searchCommonByVariable(int targetId, SearchType type) {
  m_common.clear();
  int index{0};
  for (auto& common : m_parent->database().commonEvents.events()) {
    if (common.has_value()) {
      for (auto& cmd : common.value().commands()) {
        if (cmd->hasReference(targetId, type)) {
          m_common.emplace_back(&common, index);
        }
        index++;
      }
    }
  }
}
void ReferenceSearch::searchCommonByVariable(std::string text, SearchType type) {
  m_common.clear();
  int index{0};
  for (auto& common : m_parent->database().commonEvents.events()) {
    if (common.has_value()) {
      bool resultFound{false};
      for (auto& cmd : common.value().commands()) {
        if (cmd->hasStringReference(text, type)) {
          m_common.emplace_back(&common, index);
        }
        index++;
      }
    }
  }
}
