#include "Core/ReferenceSearch.hpp"

#include "MainWindow.hpp"
ReferenceSearch::ReferenceSearch(MainWindow* parent) : m_parent(parent) {}
void ReferenceSearch::findAllReferences(int targetId, SearchType type) {
  m_event.clear();
  m_list.clear();
  for (auto& mapInfo : m_parent->database().mapInfos.mapInfos()) {
    if (mapInfo->map()) {
      for (auto& event : mapInfo->map()->events()) {
        if (event) {
          for (const auto& eventId : event.value().getConditionReferences(targetId, type)) {
            m_event[mapInfo->id()].push_back(eventId);
          }
          for (const auto& eventId : event.value().getListReferences(targetId, type)) {
            m_list[mapInfo->id()].push_back(eventId);
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
          for (const auto& eventId : event.value().getListReferences(text, type)) {
            m_list[mapInfo->id()].push_back(eventId);
          }
        }
      }
    }
  }
  searchCommonByVariable(text, type);
}
void ReferenceSearch::searchCommonByVariable(int targetId, SearchType type) {
  m_common.clear();
  for (auto& common : m_parent->database().commonEvents.events()) {
    if (common.has_value()) {
      bool resultFound{false};
      for (auto& cmd : common.value().commands()) {
        if (!resultFound) {
          if (cmd->hasReference(targetId, type)) {
            m_common.push_back(common->id());
            resultFound = true;
          }
        }
      }
    }
  }
}
void ReferenceSearch::searchCommonByVariable(std::string text, SearchType type) {
  m_common.clear();
  for (auto& common : m_parent->database().commonEvents.events()) {
    if (common.has_value()) {
      bool resultFound{false};
      for (auto& cmd : common.value().commands()) {
        if (!resultFound) {
          if (cmd->hasStringReference(text, type)) {
            m_common.push_back(common->id());
            resultFound = true;
          }
        }
      }
    }
  }
}
