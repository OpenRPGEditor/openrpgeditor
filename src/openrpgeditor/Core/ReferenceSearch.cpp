#include "Core/ReferenceSearch.hpp"

#include "MainWindow.hpp"
ReferenceSearch::ReferenceSearch() {}
void ReferenceSearch::findAllReferences(int targetId, SearchType type) {
  m_results.clear();
  for (auto& mapInfo : Database::instance()->mapInfos.mapInfos()) {
    if (mapInfo->map()) {
      for (auto& event : mapInfo->map()->events()) {
        if (event) {
          for (auto& ev : event.value().isConditionalReference(targetId, type)) {
            if (ev.second) {
              m_results.emplace_back(mapInfo->id(), event.value().id(), ev.first);
            }
          }
        }
      }
    }
  }
  searchAllListsByTarget(targetId, type);
  searchAllCommonByTarget(targetId, type);
}
void ReferenceSearch::findAllReferences(std::string text, SearchType type) {
  m_results.clear();
  for (auto& result : m_results) {
    auto refEvent = result.getEvent();
    int pageIndex{0};
    for (auto& page : refEvent.pages()) {
      int stepIndex{0};
      for (auto& cmd : page.list()) {
        if (cmd->hasStringReference(text, type)) {
          m_list.emplace_back(result.getMapId(), result.getEventId(), cmd, pageIndex, stepIndex);
        }
        stepIndex++;
      }
      pageIndex++;
    }
  }
  searchAllListsByText(text, type);
  searchAllCommonByText(text, type);
}
void ReferenceSearch::searchAllListsByTarget(int targetId, SearchType type) {
  m_list.clear();
  for (auto& mapInfo : Database::instance()->mapInfos.mapInfos()) {
    if (mapInfo->map()) {
      for (auto& event : mapInfo->map()->events()) {
        if (event) {
          int pageIndex{0};
          for (auto& page : event.value().pages()) {
            int stepIndex{0};
            for (auto& cmd : page.list()) {
              if (cmd->hasReference(targetId, type)) {
                m_list.emplace_back(mapInfo->id(), event.value().id(), cmd, pageIndex, stepIndex);
              }
              stepIndex++;
            }
            pageIndex++;
          }
        }
      }
    }
  }
}
void ReferenceSearch::searchAllListsByText(std::string text, SearchType type) {
  m_list.clear();
  for (auto& mapInfo : Database::instance()->mapInfos.mapInfos()) {
    if (mapInfo->map()) {
      for (auto& event : mapInfo->map()->events()) {
        if (event) {
          int pageIndex{0};
          for (auto& page : event.value().pages()) {
            int stepIndex{0};
            for (auto& cmd : page.list()) {
              if (cmd->hasStringReference(text, type)) {
                m_list.emplace_back(mapInfo->id(), event.value().id(), cmd, pageIndex, stepIndex);
              }
              stepIndex++;
            }
            pageIndex++;
          }
        }
      }
    }
  }
}
void ReferenceSearch::searchAllCommonByTarget(int targetId, SearchType type) {
  m_common.clear();
  for (auto& common : Database::instance()->commonEvents.events()) {
    if (common.has_value()) {
      int index{0};
      for (auto& cmd : common.value().commands()) {
        if (cmd->hasReference(targetId, type)) {
          m_common.emplace_back(common.value().id(), cmd, index);
        }
        index++;
      }
    }
  }
}
void ReferenceSearch::searchAllCommonByText(std::string text, SearchType type) {
  m_common.clear();
  int index{0};
  for (auto& common : Database::instance()->commonEvents.events()) {
    if (common.has_value()) {
      bool resultFound{false};
      for (auto& cmd : common.value().commands()) {
        if (cmd->hasStringReference(text, type)) {
          m_common.emplace_back(common.value().id(), cmd, index);
        }
        index++;
      }
    }
  }
}
