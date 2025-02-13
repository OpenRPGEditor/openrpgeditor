#include "Core/ReferenceSearch.hpp"

#include "MainWindow.hpp"
ReferenceSearch::ReferenceSearch(MainWindow* parent) : m_parent(parent) {}
void ReferenceSearch::findAllReferences(int targetId, SearchType type) {
  if (type == SearchType::Switch) {
    for (auto& mapInfo : m_parent->database().mapInfos.mapInfos()) {
      if (mapInfo->map()) {
        for (auto& event : mapInfo->map()->events()) {
          if (event) {
            std::vector<std::shared_ptr<const IModifiable>> conditionVector = event.value().getConditionReferences(targetId, SearchType::Switch);
            std::vector<std::shared_ptr<const IModifiable>> listVector = event.value().getListReferences(targetId, SearchType::Switch);
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
  } else if (type == SearchType::Variable) {
    for (auto& mapInfo : m_parent->database().mapInfos.mapInfos()) {
      if (mapInfo->map()) {
        for (auto& event : mapInfo->map()->events()) {
          if (event) {
            std::vector<std::shared_ptr<const IModifiable>> conditionVector = event.value().getConditionReferences(targetId, SearchType::Variable);
            std::vector<std::shared_ptr<const IModifiable>> listVector = event.value().getListReferences(targetId, SearchType::Variable);
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
}
void ReferenceSearch::findAllReferences(std::string text, SearchType type) {
  if (type == SearchType::EventName) {}
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