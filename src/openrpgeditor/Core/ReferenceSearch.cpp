#include "Core/ReferenceSearch.hpp"

#include "MainWindow.hpp"
ReferenceSearch::ReferenceSearch(MainWindow* parent) : m_parent(parent) {}
void ReferenceSearch::findAllReferences(int targetId, SearchType type) {
  for (auto& mapInfo : m_parent->database().mapInfos.mapInfos()) {
    if (mapInfo->map()) {
      for (auto& event : mapInfo->map()->events()) {
        if (event) {
          std::vector<std::shared_ptr<const IModifiable>> conditionVector = event.value().getConditionReferences(targetId, type);
          std::vector<std::shared_ptr<const IModifiable>> listVector = event.value().getListReferences(targetId, type);
          for (const auto& search : conditionVector) {
            m_events[mapInfo->id()].push_back(search);
          }
          for (const auto& search : listVector) {
            m_listEvents[mapInfo->id()].push_back(search);
          }
        }
      }
    }
  }
  searchCommonByVariable(targetId, type);
}
void ReferenceSearch::findAllReferences(std::string text, SearchType type) {
  for (auto& mapInfo : m_parent->database().mapInfos.mapInfos()) {
    if (mapInfo->map()) {
      for (auto& event : mapInfo->map()->events()) {
        if (event) {
          std::vector<std::shared_ptr<const IModifiable>> listVector = event.value().getListReferences(text, type);
          for (const auto& search : listVector) {
            m_listEvents[mapInfo->id()].push_back(search);
          }
        }
      }
    }
  }
  searchCommonByVariable(text, type);
}
void ReferenceSearch::searchCommonByVariable(int targetId, SearchType type) {
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
